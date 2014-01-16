#!/usr/bin/env python
# -*- coding: utf-8

from __future__ import division
from __future__ import absolute_import
from __future__ import print_function

import os.path
import os
import ConfigParser
import sys
import argparse
import logging
import json
import urllib
import urllib2
import urlparse
import base64
import subprocess
import re
import getpass
import traceback
import locale

MIN_CRUCIBLE_VERSION = '3.0.0'
SCRIPT_NAME = os.path.basename(__file__)
SCRIPT_VERSION='faefac2e8d32053719c807c4cd3dfaf3'


### subprocess wrappers
def check_output(*popenargs, **kwargs):
    """Run command with arguments and return its output as a byte string."""
    if 'stdout' in kwargs or 'stderr' in kwargs:
        raise ValueError('stdout argument not allowed, it will be overridden.')
    logging.debug('Trying to execute %s', popenargs)
    process = subprocess.Popen(stdout=subprocess.PIPE, stderr=subprocess.PIPE, *popenargs, **kwargs)
    output, err = process.communicate()
    retcode = process.poll()
    if retcode:
        cmd = kwargs.get("args")
        if cmd is None:
            cmd = popenargs[0]
        logging.debug('Error executing, exit code %s\nstdout=%s\nstderr=%s', retcode, output, err)
        raise CalledProcessError(retcode, cmd, output=output, error=err)
    logging.debug('Finished executing, exit code 0\nstdout=%s\nstderr=%s', output, err)
    return output


class CalledProcessError(subprocess.CalledProcessError):
    def __init__(self, returncode, cmd, output=None, error=None):
        super(CalledProcessError, self).__init__(returncode, cmd)
        self.output = output
        self.error = error


class Console:
    NO_ANSI = sys.platform == 'win32' or not sys.stdout.isatty()
    GREEN = '\033[92m'
    RED = '\033[91m'
    ESCAPE = '\033[0m'

    @staticmethod
    def print(s, color=None):
        if Console.NO_ANSI or not color:
            print(s)
        else:
            print('%s%s%s' % (color, s, Console.ESCAPE))

    @staticmethod
    def error(s):
        Console.print(s, color=Console.RED)

    @staticmethod
    def success(s):
        Console.print(s, color=Console.GREEN)


class HTTPRedirectHandler(urllib2.HTTPRedirectHandler):
    """ Override HTTPRedirectHandler to make sure the request data is preserved on redirect """
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        m = req.get_method()
        if code in (301, 302, 303, 307) and m in ("GET", "HEAD") or code in (301, 302, 303) and m == "POST":
            newurl = newurl.replace(' ', '%20')
            newheaders = dict((k, v) for k, v in req.headers.items() if k.lower() not in 'content-length')
            return urllib2.Request(newurl,
                                   headers=newheaders,
                                   data=req.data,
                                   origin_req_host=req.get_origin_req_host(),
                                   unverifiable=True)
        else:
            raise urllib2.HTTPError(req.get_full_url(), code, msg, headers, fp)


class Configuration(object):
    """Represents the configuration of the current execution of the script"""
    def __init__(self):
        self._url = None
        self.username = None
        self.password = None
        self.authtoken = None
        self._id = None
        self.title = None
        self.reviewers = []
        self.moderator = None
        self.executables = {
            'hg': 'hg',
            'svn': 'svn',
            'git': 'git',
            'p4': 'p4',
            'cvs': 'cvs',
        }
        self.repository = None
        self.last_project = None
        self.initial_fill = True
        self.no_anchor = False
        self.encoding = None
        self.diff_file = None
        self.patch_source = None
        self.new_patch_source = False

    @property
    def url(self):
        return self._url

    @url.setter
    def url(self, value):
        self._url = value.rstrip('/').strip() if value else None

    @property
    def id(self):
        return self._id

    @id.setter
    def id(self, value):
        self._id = value.strip() if value else None

    def fill_from_defaults(self):
        self.url, self.username = self.url or 'https://adc.luxoft.com/fisheye', self.username or 'akutsan'
        return self

    def fill_from_config_file(self, config_file):
        if not self.url:
            self.url = config_file.get_default_url()

        if not self.password:
            stored_token, stored_user = config_file.get_token(self.url)
            if not self.username or self.username == stored_user:
                self.authtoken, self.username = stored_token, stored_user

        return self

    def fill_from_args(self, args):
        self.url = args.server or self.url
        self.username = args.user or self.username
        self.password = args.password or self.password
        self.title = args.title or self.title

        args_dict = vars(args)
        self.id = args_dict[str('project/review')] or self.id
        if '@reviewer' in args_dict:
            for reviewer in args_dict[str('@reviewer')]:
                self.reviewers.append(reviewer.lstrip('@').strip())
                logging.debug('Parsed reviewers: %s', self.reviewers)
        self.moderator = args.moderator.lstrip('@').strip() if args.moderator else self.moderator
        self.repository = args.repository or self.repository
        self.no_anchor = args.noanchor or self.no_anchor
        self.encoding = args.encoding or self.encoding
        self.diff_file = args.file or self.diff_file
        self.new_patch_source = args.newpatch or self.new_patch_source

        return self

    def validate(self, check_title=False, check_id=False):
        """Checks if all the required server options are set"""
        if self.url and (self.authtoken or (self.username and self.password)) and (not check_id or self.id) and (not check_title or self.title):
            return True

        if not self.url:
            Console.error('ERROR: Please specify a Crucible server')
        if not self.authtoken and not self.username:
            Console.error('ERROR: Please specify a Crucible username')
        if not self.authtoken and not self.password:
            Console.error('ERROR: Please specify a Crucible password')
        if check_id and not self.id:
            Console.error('ERROR: Please specify a project or review id')
        if check_title and not self.title:
            Console.error('ERROR: Please specify a review title')
        print(CommandLine().help_blurb())
        sys.exit(1)

    def fill_interactively(self, get_title=False, get_id=False, get_reviewers=False):
        """Fills the required parameters by prompting the user if they're not specified"""
        if not sys.stdin.isatty():
            logging.debug('Not prompting for parameters interactively because stdin is not a tty')
        else:
            if not self.url:
                self.url = raw_input('Please enter the Crucible server URL: ')
            elif self.initial_fill:
                print('Crucible server: %s' % self.url)

            if not self.username:
                self.username = raw_input('Please enter your Crucible username: ')
            elif self.initial_fill:
                print("Crucible username: %s" % self.username)

            if not self.password and not self.authtoken:
                self.password = getpass.getpass(str('Please enter your Crucible password: '))

            if get_id:
                while not self.id:
                    last_project_prompt = ' [%s]' % self.last_project if self.last_project else ''
                    self.id = raw_input('Please specify a project to create the review in or an existing review id to add to%s: ' % last_project_prompt).strip()
                    self.id = self.id or self.last_project

            if get_title:
                while not self.title:
                    self.title = raw_input('Please specify the review title: ')

            if get_reviewers and not self.reviewers:
                while True:
                    reviewer = raw_input('Please specify a reviewer to be added to the review, or press Enter to continue: ').strip()
                    if not reviewer:
                        break
                    if not reviewer in self.reviewers:
                        self.reviewers.append(reviewer)

        self.validate(check_title=get_title, check_id=get_id)
        self.initial_fill = False

        return self

    def choose_anchor(self, repositories):
        """Allows choosing an anchor repository if none detected"""
        if not sys.stdin.isatty():
            logging.debug('Not prompting for anchor interactively because stdin is not a tty')
            return

        repository_names = [str(repository.get('name'))
                            for repository in filter(lambda repository: repository.get('enabled'), repositories)]
        while not self.repository and not self.no_anchor:
            repository = raw_input("Please choose a repository to anchor to, or press Enter to skip anchoring: ")
            if not repository:
                self.no_anchor = True
            elif repository in repository_names:
                self.repository = repository
            else:
                print('The repository doesn\'t exist or is disabled')

    def choose_source(self, matching_patch_groups):
        """Allows choosing a source from a list of matching ones"""
        if len(matching_patch_groups) == 1:
            print('Adding patch to existing one: %s. Use --newpatch to add as a new patch instead.' % matching_patch_groups[0]['displayName'])
            self.patch_source = matching_patch_groups[0]['sourceName']
        elif len(matching_patch_groups) > 1:
            if not sys.stdin.isatty():
                logging.debug('Not prompting for source interactively because stdin is not a tty')
                return

            print('Found %s patches to add to:' % len(matching_patch_groups))
            i = 1
            print('0. Create a new patch')
            for patch_group in matching_patch_groups:
                print('%s. Add to %s' % (i, patch_group['displayName']))
                i += 1

            while not self.patch_source and not self.new_patch_source:
                try:
                    choice = int(raw_input('Pick patch to add to [0-%s]: ' % len(matching_patch_groups)))
                    if 0 < choice <= len(matching_patch_groups):
                        self.new_patch_source = False
                        self.patch_source = matching_patch_groups[choice - 1]['sourceName']
                    elif choice == 0:
                        self.new_patch_source = True
                        self.patch_source = None
                except ValueError:
                    pass



class ConfigFile:
    """Handles reading and storing settings from the config file"""
    userConfigPath = os.path.expanduser('~/.atlassian/crucible.conf')
    DEFAULT_SECTION = 'DEFAULT'
    EXECUTABLES_SECTION = 'executables'
    URL = 'url'
    TOKEN = 'authtoken'
    USER = 'user'

    def __init__(self):
        self.config_parser = ConfigParser.RawConfigParser()
        self.config_parser.read([self.userConfigPath])

    def _get(self, section, key):
        if not self.config_parser.has_option(section, key):
            return None
        return self.config_parser.get(section, key)

    def get_default_url(self):
        return self._get(self.DEFAULT_SECTION, self.URL)

    def store_configuration(self, configuration):
        if self._get(self.DEFAULT_SECTION, self.URL) != configuration.url:
            self.config_parser.set(self.DEFAULT_SECTION, self.URL, configuration.url)
            print('Saved the default server URL %s to %s' % (configuration.url, self.userConfigPath))

        self.store_token(configuration.url, configuration.username, configuration.authtoken)
        self.save()

    def get_token(self, url):
        if not self.config_parser.has_section(url):
            return None, None
        return self._get(url, self.TOKEN), self._get(url, self.USER)

    def store_token(self, url, user, token):
        if not self.get_token(url) == (token, user):
            if not self.config_parser.has_section(url):
                self.config_parser.add_section(url)
            self.config_parser.set(url, self.TOKEN, token)
            self.config_parser.set(url, self.USER, user)
            print ('Saved an authentication token for %s to %s' % (url, self.userConfigPath))

    def save(self):
        try:
            if not os.path.exists(os.path.dirname(self.userConfigPath)):
                os.makedirs(os.path.dirname(self.userConfigPath))

            with os.fdopen(os.open(self.userConfigPath, os.O_WRONLY | os.O_CREAT | os.O_TRUNC, 0o600), 'wb') as configfile:
                self.config_parser.write(configfile)
        except IOError as e:
            print('Error saving the configuration file %s - %s' % (self.userConfigPath, e))
            logging.debug(traceback.format_exc())


class CommandLine:
    """Handles parsing the commandline parameters"""
    def __init__(self):
        self.parser = argparse.ArgumentParser(
            formatter_class=argparse.RawTextHelpFormatter,
            description='Creates reviews in Atlassian Crucible from the command line',
            add_help=False,
            epilog='''
EXAMPLE USAGE:
 %(executable)s
                        run interactively, try to get a patch from SCM, prompt for review details and create a new review\n
 cat diff | %(executable)s CR -m "Review title"
                        take the patch from the output of first command, and create a review in project CR with title "Review title"\n
 %(executable)s CR @ted @matt --moderator @john
                        try to get a patch from SCM, create a review in project CR add ted and matt as reviewers, and john as moderator\n
 %(executable)s CR-120
                        update the review CR-120, adding the current patch from SCM\n
 %(executable)s -r repository1
                        create a review and anchor it to repository1, instead of trying to detect the repository automatically\n
            ''' % {'executable': SCRIPT_NAME})

        class HelpAction(argparse._HelpAction):
            def __call__(self, parser, namespace, values, option_string=None):
                print(parser.format_help()
                .replace('usage:', 'USAGE:')
                .replace('positional arguments:', 'POSITIONAL ARGUMENTS:')
                .replace('optional arguments:', 'OPTIONAL ARGUMENTS:'))
                parser.exit()

        self.parser.add_argument('project/review', type=str, nargs='?', default=None, help='the name of the project to create the review in or the id of a review to update\n\n')
        self.parser.add_argument('@reviewer', type=str, nargs='*', default=[], help='the usernames of the reviewers to be added')
        self.parser.add_argument('-h', '--help', action=HelpAction, help='show this help message and exit\n\n')
        self.parser.add_argument('-m', '--title', type=str, help="the title for the review\n\n")
        self.parser.add_argument('-M', '--moderator', type=str, help="the moderator for the review\n\n")
        self.parser.add_argument('-r', '--repository', type=str, help='the repository to anchor to\n\n')
        self.parser.add_argument('-f', '--file', type=str, help='get the diff from the specified file\n\n')
        self.parser.add_argument('-s', '--server', type=str, help="the url of the Crucible server to connect to\n\n")
        self.parser.add_argument('-u', '--user', type=str, help="the Crucible username to create the review as\n\n")
        self.parser.add_argument('-p', '--password', type=str, help="the Crucible user password\n\n")
        self.parser.add_argument('-n', '--noanchor', action='store_const', const=True, help='don\'t try to detect the repository to anchor the patch\n\n')
        self.parser.add_argument('-N', '--newpatch', action='store_const', const=True, help='add as a new patch instead of trying to an existing one\n\n')
        self.parser.add_argument('-e', '--encoding', type=str, help='the name of the encoding to use, see http://docs.python.org/2/library/codecs.html#standard-encodings\n\n')
        self.parser.add_argument('-d', '--debug', action='store_const', const=True, help="print debugging information\n\n")
        self.parser.add_argument('-v', '--version', action='version', version=('%s %s' % (SCRIPT_NAME, SCRIPT_VERSION)))

    def parse_args(self):
        args = self.parser.parse_args()
        if args.debug:
            logging.basicConfig(stream=sys.stderr, level=logging.DEBUG)
        if args.noanchor and args.repository:
            Console.error('Please choose either --noanchor or --repository')
            print(self.help_blurb())
            sys.exit(1)

        return args

    def help_blurb(self):
        return 'Try \'%s --help\' for more information.' % os.path.basename(sys.argv[0])

class CrucibleRest:
    """Encapsulates Crucible REST endopoints"""
    timeout = 30000
    code_anchor_failed = 'PatchAnchorFailed'
    code_content_too_large = 'ChangeSetContentTooLarge'
    code_review_content_too_large = 'ReviewContentTooLarge'

    http_handlers_none_on_errors = {
        400: lambda http_error, error_body: None,
        404: lambda http_error, error_body: None,
    }

    def __init__(self, configuration):
        self.configuration = configuration
        self.headers = {
            'Content-Type': 'application/json',
            'Accept': 'application/json',
        }
        self.repositories = None

    def __build_headers(self, custom_headers, use_token):
        headers = {}
        for key, value in self.headers.items():
            headers[key] = value
        if not use_token and self.configuration.username and self.configuration.password:
            headers['Authorization'] = 'Basic ' + base64.b64encode(
                str('%s:%s') % (self.configuration.username, self.configuration.password)).strip(),
        for key, value in custom_headers.items():
            headers[key] = value
        return headers

    def __build_payload(self, data):
        if not data:
            return None

        encodings = []
        if self.configuration.encoding:
            encodings.append(self.configuration.encoding)
        else:
            encodings.append('UTF-8')
            default_encoding = locale.getdefaultlocale()[1]
            if default_encoding and default_encoding not in encodings:
                encodings.append(default_encoding)
            if sys.stdin.encoding not in encodings:
                encodings.append(sys.stdin.encoding)

        payload = None
        for encoding in encodings:
            try:
                if isinstance(data, unicode):
                    payload = data
                if isinstance(data, str):
                    logging.debug('Trying to encode str as %s', encoding)
                    payload = unicode(data, encoding=encoding).encode('utf-8')
                else:
                    logging.debug('Trying to encode json as %s', encoding)
                    payload = json.dumps(data, encoding=encoding)
                break
            except ValueError:
                logging.debug('Encoding failed: %s', traceback.format_exc())
        if not payload:
            Console.error(
                'Error encoding the request (tried %s), please specify an --encoding parameter' % ', '.join(
                    encodings))
            sys.exit(1)

        return payload

    def __build_url(self, url, use_token):
        joined_url = self.configuration.url + url
        if not use_token:
            return joined_url
        else:
            scheme, netloc, path, query, fragment = urlparse.urlsplit(joined_url)
            query_dict = urlparse.parse_qs(query)
            query_dict['FEAUTH'] = [self.configuration.authtoken]
            return urlparse.urlunsplit((scheme, netloc, path, urllib.urlencode(query_dict, doseq=True), fragment))

    def _log_request(self, headers, payload, resourceUrl):
        resourceUrl = re.sub(r'FEAUTH=(.*)%3A([0-9]+)%3A([0-9a-f]+)', r'FEAUTH=\1%3A\2%3A++SANITIZED++', resourceUrl)
        if payload:
            payload = re.sub(r'password=(.*)([\s&]?)', r'password=++SANITIZED++\2', payload)
        if headers and headers.get('Authorization'):
            headers['Authorization'] = '++SANITIZED++'
        logging.debug('RestRequest: %s - %s -  %s', resourceUrl, headers, payload)

    def _log_response(self, response):
        if response:
            response = re.sub(r'"token":"(.*):([0-9]+):([0-9a-f]+)"', r'"token":"\1:\2:++SANITIZED++"', response)
        logging.debug('RestResponse: %s', response)

    def _request(self, url, data=None, http_handlers = {}, use_token=True, custom_headers={}):
        """Executes a REST request the given rest resource, It's a POST if data is set, a GET otherwise"""
        resourceUrl = self.__build_url(url, use_token)
        headers = self.__build_headers(custom_headers, use_token)
        request = urllib2.Request(url=resourceUrl, headers=headers)
        payload = self.__build_payload(data)

        self._log_request(headers, payload, resourceUrl)

        try:
            response = urllib2.build_opener(HTTPRedirectHandler()).open(request, data=payload, timeout=self.timeout).read()
        except urllib2.HTTPError as error:
            logging.debug('RestError: %s', error)
            error_body = error.read()
            try:
                error_body = json.loads(error_body)
            except ValueError:
                pass
            logging.debug('RestErrorBody: %s', error_body)

            if error.code in http_handlers:
                return http_handlers[error.code](error, error_body)

            if error.code == 404:
                error_msg = error_body['message'] if isinstance(error_body, dict) and 'message' in error_body \
                            else 'Please check that %s is a Crucible server and the version is at least %s' % (self.configuration.url, MIN_CRUCIBLE_VERSION)
                Console.error('Error : %s' % error_msg)
            elif error.code == 401:
                Console.error('Authorization error: please check that your username and password are valid, and that you have the correct permissions.')
            elif error.code == 403:
                error_msg = error_body['message'] if isinstance(error_body, dict) and 'message' in error_body else 'You do not have permission to perform this operation'
                permissions_msg = 'Please contact your administrator if you need access'
                Console.error('Error: %s\n%s' % (error_msg, permissions_msg))
            elif error.code == 500:
                Console.error('Server Error: %s' % error_body['message'] if isinstance(error_body, dict) and 'message' in error_body else error_body)
            else:
                Console.error('Received an unexpected response %s. Please check that %s is a Crucible server' % (error, self.configuration.url))
            sys.exit(1)
        except Exception as error:
            logging.debug(traceback.format_exc())
            Console.error('Eror executing request %s - %s' % (resourceUrl, error))
            sys.exit(1)

        self._log_response(response)
        if 200 in http_handlers:
            return http_handlers[200](response)

        try:
            return json.loads(response)
        except ValueError:
            return response

    def check_connection(self):
        """ Makes sure we can connect and authenticate with Crucible"""
        # if there's a token, check that it's still valid
        server_info = None
        if self.configuration.authtoken:
            server_info = self._request(url="/rest-service-fecru/server-v1", http_handlers={
                401: lambda error, error_body: None
            })
            if not server_info:
                print("Your login token has expired, please re-authenticate")
                self.configuration.authtoken = None
                self.configuration.fill_interactively()

        # get a token if none defined at this point
        if not self.configuration.authtoken:
            logging.debug('No authtoken, trying to get one')
            login_success, login_response = self._request(
                url='/rest-service/auth-v1/login',
                data=urllib.urlencode({'userName':self.configuration.username, 'password':self.configuration.password}),
                http_handlers={
                    403: lambda error, error_body: (False, error_body),
                    200: lambda response: (True, json.loads(response))
                },
                use_token=False,
                custom_headers={'Content-Type':'application/x-www-form-urlencoded'}
            )

            if not login_success:
                message = login_response['error'] if isinstance(login_response, dict) and 'error' in login_response else 'Please check that the username and password provided are correct.'
                Console.error('Error authenticating with Crucible. ' + message)
                sys.exit(1)
            self.configuration.authtoken = login_response['token']

        server_info = server_info or self._request(url="/rest-service-fecru/server-v1")
        if not server_info['isCrucible']:
            Console.error('Connected successfully to %s but no Crucible license is present.' % self.configuration.url)
            sys.exit(1)
        logging.debug('Connected successfully to %s - Crucible version %s', self.configuration.url, server_info['version']['releaseNumber'])

    def add_reviewers(self, review_id, reviewers):
        """Adds the configuration.reviewers to the review with the given id. Returns True if any reviewers were added"""
        if not reviewers: return False

        logging.debug('Adding reviewers: %s', ', '.join(reviewers))
        reviewers_added = False
        for reviewer in reviewers:
            if reviewer:
                reviewer_added, reviewer_msg = self._request("/rest-service/reviews-v1/%s/reviewers" % str(review_id), data=reviewer,
                    http_handlers={
                        404: lambda http_error, error_body: (False, 'No user \'%s\' found - not adding as a reviewer' % reviewer),
                        400: lambda http_error, error_body: (False, error_body['message'] if 'message' in error_body else 'Error adding reviewer %s' % reviewer ),
                        200: lambda response: (True, '')
                    })
                if not reviewer_added:
                    print(reviewer_msg)
                else:
                    reviewers_added = True
        return reviewers_added

    def handle_anchor_error(self, http_error, error_body):
        if error_body.get('code') == CrucibleRest.code_anchor_failed:
            Console.error('Error: Failed to anchor the patch to repository %s, please check that the repository is the correct one' % self.configuration.repository)
            sys.exit(1)
        elif error_body.get('code') == CrucibleRest.code_content_too_large or error_body.get('code') == CrucibleRest.code_review_content_too_large:
            Console.error('Error: The patch you\'re trying to upload is too large. ' + error_body.get('message'))
            sys.exit(1)
        else:
            Console.error('Received a 409 Conflict response: %s' % error_body['message'])

    def create_review(self, patch, project):
        """Creates a new review from the patch, in the given project"""
        logging.debug('Creating new review in project %s', project['key'])
        review_data = {"reviewData": {
            "projectKey": project['key'],
            "name": self.configuration.title,
            "description": project['defaultObjectives'] if 'defaultObjectives' in project else None,
            },
        }

        review_data = self.add_patch_data(patch, request_dict=review_data)

        if self.configuration.moderator:
            if not project['moderatorEnabled']:
                print('Project %s doesn\'t have a moderator role enabled, not setting a moderator' % project['key'])
            else:
                review_data['reviewData']['moderator'] = {'userName':self.configuration.moderator}


        create_response = self._request("/rest-service/reviews-v1", review_data, http_handlers={
            409: self.handle_anchor_error
        })
        review_id = create_response['permaId']['id']
        review_state = create_response['state']

        reviewers_added = self.add_reviewers(review_id, self.configuration.reviewers)

        if reviewers_added or project['defaultReviewers']:
            logging.debug('Starting review')
            approve_response = self._request('/rest-service/reviews-v1/%s/transition?action=action:approveReview' % review_id, data=' ', http_handlers={
                401: lambda http_error, error_body: print('You don\'t have permission to approve the review')
            })
            if approve_response:
                review_state = approve_response['state']
        else:
            print('No reviewers added, review will be left in Draft state')

        Console.success('Created review %(id)s (state: %(state)s) - %(url)s/cru/%(id)s'
              % ({'id': review_id, 'state': review_state,'url':self.configuration.url}))

    def add_patch_data(self, patch, request_dict={}):
        request_dict['patch'] = patch
        if self.configuration.repository:
            request_dict['anchor'] = {'anchorRepository' : self.configuration.repository}
        return request_dict

    def get_iterable_patchgroups(self, review_id, repository):
        patch_groups = self._request('/rest-service/reviews-v1/%s/patch' % review_id)['patchGroup']
        matching_repo = lambda patch_group: 'anchor' in patch_group['patches'][0] and \
                                            patch_group['patches'][0]['anchor'].get('anchorRepository') == repository
        return filter(matching_repo, patch_groups)

    def add_to_review(self, patch, review):
        """Adds the patch and reviewers to the given review"""
        review_id = review['permaId']['id']
        data = self.add_patch_data(patch)

        if 'anchor' in data and not self.configuration.new_patch_source:
            matching_patch_groups = self.get_iterable_patchgroups(review_id, data['anchor']['anchorRepository'])
            self.configuration.choose_source(matching_patch_groups)
            if self.configuration.patch_source:
                data['source'] = self.configuration.patch_source

        logging.debug('Adding patch to review %s' % review_id)

        patch_response = self._request('/rest-service/reviews-v1/%s/patch' % review_id, data=data, http_handlers={
            409: self.handle_anchor_error
        })
        self.add_reviewers(review_id, self.configuration.reviewers)

        Console.success('Updated review %(id)s (state: %(state)s) - %(url)s/cru/%(id)s'
              % ({'id': review_id, 'state': patch_response['state'],'url':self.configuration.url}))

    def get_review(self, id):
        """A ReviewData json if the id represents an existing review, None otherwise"""
        try:
            return self._request('/rest-service/reviews-v1/%s' % id, http_handlers=CrucibleRest.http_handlers_none_on_errors)
        except StandardError:
            return None

    def get_project(self, id):
        """A ProjectDat json if the id represents an existing project, None otherwise"""
        try:
            return self._request('/rest-service/projects-v1/%s?excludeAllowedReviewers=true' % id, http_handlers=CrucibleRest.http_handlers_none_on_errors)
        except StandardError:
            return None

    def get_last_project(self):
        """Retrieves the most recent project, or None if it fails"""
        try:
            recent_projects = self._request('/rest-service-fecru/recently-visited-v1/projects')['project']
            if(len(recent_projects)) == 0: return None
            return recent_projects[0]['entityId']
        except StandardError:
            logging.debug('Error getting last project: %s', traceback.format_exc())
            return None

    def find_repository(self, source):
        """Tries to find the anchor repository for the source given"""
        try:
            for repository in self.get_repositories():
                logging.debug('Matching remote repository %s', repository['name'])
                if repository['enabled'] and source.matches_repository(repository):
                    return repository['name']
        except StandardError:
            logging.debug('Error suggesting anchor repository: %s', traceback.format_exc())

        return None

    def get_repositories(self):
        if not self.repositories:
            self.repositories = self._request('/rest-service/repositories-v1')['repoData']
        return self.repositories

    def is_script_update_available(self):
        return self._request('/rest/review-cli/1.0/version/updateCheck?version=%s' % SCRIPT_VERSION)['isUpdateAvailable']

class PatchSource(object):
    """Base class for different ways to get a patch"""
    def __init__(self, configuration):
        self.configuration = configuration
        self.paths = []

    def is_active(self):
        """Should return True if this is the source can be used"""

    def get_patch(self):
        """Should return the patch content"""

    def get_review(self):
        """A user visible name for the source"""

    def executable(self):
        """The binary to execute commands on the repository"""

    def matches_repository(self, repository_data):
        """Whether the source matches the given RepositoryData json"""

    def load_patch(self):
        """Queries different patch sources for a patch"""
        for source_class in PatchSource.__subclasses__():
            logging.debug('Checking %s.is_active', source_class)
            source = source_class(self.configuration)
            if source.is_active():
                source._validate_executable()
                logging.debug('Getting patch from %s', source)
                patch = source.get_patch()
                if patch:
                    logging.debug('Got %s bytes', len(patch))

                    if not self.configuration.no_anchor and not self.configuration.repository:
                        source.load_paths()
                        logging.debug('Loaded paths: %s', source.paths)

                return patch, source
        return None, None

    def _validate_executable(self):
        try:
            self.validate_executable()
        except (OSError, CalledProcessError) as e:
            Console.error('A %s repository was detected, but there was an error executing \'%s\': %s' % (self, self.configuration.executables[self.executable()], e))
            sys.exit(1)

    def validate_executable(self):
        pass

    def load_paths(self):
        """Loads the potential remote paths for the given repo type to be used when trying to detect a repository to anchor to"""
        pass

    def matches_url(self, url):
        requested_path = urlparse.urlsplit(url)
        logging.debug('Matching remote url: %s', requested_path.__str__())
        for path in self.paths:
            local_path = urlparse.urlsplit(path)
            logging.debug('Matching local url: %s', local_path.__str__())
            if requested_path.hostname == local_path.hostname and requested_path.port == local_path.port and requested_path.path == local_path.path:
                return True
        return False

    def find_metadata_dir(self, dirname):
        """Searches for the specified directory name starting with the current directory and going upwards the directory tree"""
        cwd = os.getcwd()
        if os.path.exists(os.path.join(cwd, dirname)):
            return True
        tmp = os.path.abspath(os.path.join(cwd, os.pardir))
        while tmp is not None and tmp != cwd:
            cwd = tmp
            if os.path.exists(os.path.join(cwd, dirname)):
                return True
            tmp = os.path.abspath(os.path.join(cwd, os.pardir))
        return False

class FileSource(PatchSource):
    """Gets the patch from the file specified by the commandline"""
    def is_active(self):
        return self.configuration.diff_file

    def get_patch(self):
        path = self.configuration.diff_file
        if not os.path.isfile(path):
            print("No such file %s" % path)
            return None

        with file(path) as f:
            return f.read()

    def __str__(self): return 'file %s' % self.configuration.diff_file

class StdInSource(PatchSource):
    """Gets a patch from stdin"""
    def is_active(self):
        return not sys.stdin.isatty()

    def get_patch(self):
        return sys.stdin.read()

    def __str__(self):
        return 'standard input'

class HgSource(PatchSource):
    """Gets a patch from a mercurial repository"""

    def is_active(self):
        return self.find_metadata_dir('.hg')

    def get_patch(self):
        output = check_output([self.configuration.executables[self.executable()], 'diff'])
        return output

    def __str__(self):
        return 'Mercurial'

    def executable(self):
        return 'hg'

    def validate_executable(self):
        check_output([self.configuration.executables[self.executable()], 'help'])

    def load_paths(self):
        paths_output = check_output([self.configuration.executables[self.executable()], 'paths']).splitlines()
        if paths_output:
            for path in paths_output:
                path_split = path.split('=')
                if len(path_split) > 1:
                    self.paths.append(path_split[1].strip())

    def matches_repository(self, repository_data):
        return repository_data['type'] == 'hg' and self.matches_url(repository_data['location'])


class GitSource(PatchSource):
    """Gets a patch from a git repository"""
    def is_active(self):
        return self.find_metadata_dir('.git')

    def get_patch(self):
        output = check_output([self.configuration.executables[self.executable()], 'diff', '--cached', '--no-prefix'])
        if not output:
            output = check_output([self.configuration.executables[self.executable()], 'diff', '--no-prefix'])
            if output:
                print('No staged changes were found in the repository, creating a review for all unstaged changes.')

        return output

    def __str__(self):
        return 'Git'

    def executable(self):
        return 'git'

    def validate_executable(self):
        check_output([self.configuration.executables[self.executable()], 'help'])

    def load_paths(self):
        paths_output = check_output([self.configuration.executables[self.executable()], 'remote', '-v']).splitlines()
        if paths_output:
            for path in paths_output:
                path_split = path.split()
                if len(path_split) > 1:
                    self.paths.append(path_split[1].strip())

    def matches_repository(self, repository_data):
        return repository_data['type'] == 'git' and self.matches_url(repository_data['location'])


class SvnSource(PatchSource):
    """Gets a patch from a subversion repository"""
    def is_active(self):
        return self.find_metadata_dir('.svn')

    def get_patch(self):
        try:
            return check_output([self.configuration.executables[self.executable()], 'diff'])
        except CalledProcessError as e:
            print('svn diff returned error: %s' % e.error.strip())
            return None

    def __str__(self):
        return 'Subversion'

    def executable(self):
        return 'svn'

    def validate_executable(self):
        check_output([self.configuration.executables[self.executable()], 'help'])

    def load_paths(self):
        info_output = check_output([self.configuration.executables[self.executable()], 'info']).splitlines()
        if info_output:
            for line in info_output:
                if line.startswith('URL:'):
                    line_split = line.split()
                    if (len(line_split)) > 1:
                        self.paths.append(line_split[1].strip())
                    break

    def matches_repository(self, repository_data):
        if not repository_data['type'] == 'svn':
            return False
        remote_url = (repository_data['url'].rstrip('/') + '/' + repository_data['path']).rstrip('/')
        logging.debug('Matching remote url %s', remote_url)
        return self.paths[0].startswith(remote_url)


class CvsSource(PatchSource):
    """Gets a patch from a CVS repository"""
    def is_active(self):
        try:
            cmd = [self.configuration.executables[self.executable()], 'status']
            check_output(cmd)
            return True
        except (OSError, CalledProcessError):
            return False

    def get_patch(self):
        try:
            return check_output([self.configuration.executables[self.executable()], 'diff', '-N', '-U', '10000'])
        except CalledProcessError as e:
            return e.output

    def __str__(self):
        return 'CVS'

    def executable(self):
        return 'cvs'

    def load_paths(self):
        try:
            with open('CVS/Root') as root_file:
                self.paths.append(root_file.read().rstrip('/\n'))
        except IOError as e:
            logging.debug('Failed getting file root file: %s', e)

        if 'CVSROOT' in os.environ:
            self.paths.append(os.environ['CVSROOT'].rstrip('/'))

    def matches_repository(self, repository_data):
        return repository_data['type'] == 'cvs' and self.matches_url(repository_data['dir'].rstrip('/'))


class P4Source(PatchSource):
    def __init__(self, configuration):
        super(P4Source, self).__init__(configuration)
        self.p4info = {}

    def is_active(self):
        """Gets a patch from a perforce repository."""
        try:
            check_output([self.configuration.executables[self.executable()]])
        except (OSError, CalledProcessError):
            return False

        self.get_client_info()

        clientRoot = self.p4info['Client root']
        if not clientRoot:
            logging.debug('Cannot find Perforce Client root while executing \'p4 info\'')
            return False

        return (os.path.normcase(os.getcwd())).startswith(os.path.normcase(clientRoot))

    def get_client_info(self):
        info = check_output([self.configuration.executables[self.executable()], 'info']).splitlines()
        for line in info:
            line_split = line.split(':', 1)
            if len(line_split) > 1:
                self.p4info[line_split[0].strip()] = line_split[1].strip()
        logging.debug('p4info: %s ', self.p4info)

    def get_path(self, file):
        output = check_output([self.configuration.executables[self.executable()], 'fstat', '-T', 'clientFile', file])
        return output[len('... clientFile '):].rstrip()

    def get_add(self, file, rev):
        f = open(self.get_path(file))
        lines = f.read().replace('\xc2\x85', '\n').splitlines()
        f.close()

        output = '--- /dev/null\n' + \
                 '+++ ' + file + '\t(revision ' + rev + ')\n' + \
                 '@@ -0,0 +1,' + str(len(lines)) + ' @@\n'
        output += '+' + '\n+'.join(lines)
        return output

    def get_delete(self, file, rev):
        lines = check_output([self.configuration.executables[self.executable()], 'print', '-q', file + '#' + rev]).splitlines()

        output = '--- ' + file + '\t(revision ' + rev + ')\n' + \
                 '+++ /dev/null\n' + \
                 '@@ -1,' + str(len(lines)) + ' +0,0 @@\n'
        output += '-' + '\n-'.join(lines)
        return output

    def get_patch(self):
        p4 = self.configuration.executables[self.executable()]
        output = check_output([p4, 'diff', '-dcu'])

        opened = check_output([p4, 'opened']).splitlines()

        pattern = re.compile('([^#]+)#([\\d]+) - ([^\\s]+)[^\\(](.*)')
        for line in opened:
            match = pattern.match(line)
            if match:
                file = match.group(1)
                rev = match.group(2)
                change = match.group(3)
                fileType = match.group(4)
                if change == 'add':
                    output += '\n' + self.get_add(file, rev)
                elif change == 'delete':
                    output += '\n' + self.get_delete(file, rev)

        return output

    def __str__(self):
        return 'Perforce'

    def executable(self):
        return 'p4'

    def load_paths(self):
        where_output = check_output([self.configuration.executables[self.executable()], 'where']).splitlines()
        for where in where_output:
            where_split = where.split()
            if len(where_split) == 3 and not where_split[0].startswith('-') and where_split[1].startswith('//%s' % self.p4info['Client name']):
                self.paths.append(where_split[0])

    def matches_repository(self, repository_data):
        if not repository_data['type'] == 'p4': return False
        remote_address = '%s:%s' % (repository_data['server'], repository_data['port'])
        logging.debug('Matching remote url: %s', remote_address)
        if self.p4info.get('Server address') == remote_address or self.p4info.get('P4Sandbox broker port') == remote_address:
            for path in self.paths:
                logging.debug('Matching path: %s', path)
                if path.startswith(repository_data['path']):
                    return True

        return False

def main():
    """
    Runs the script to create a review
    """

    # read commandline
    command_line = CommandLine()
    args = command_line.parse_args()

    # parse/fill config
    config_file = ConfigFile()
    configuration = Configuration().fill_from_args(args).fill_from_config_file(config_file).fill_from_defaults().fill_interactively()

    # set up authentication, check for updates
    rest = CrucibleRest(configuration)
    rest.check_connection()
    config_file.store_configuration(configuration)

    if rest.is_script_update_available():
        print('An updated version of this script is available. Visit %s to download it' % (configuration.url + '/plugins/servlet/viewReviewCLI'))

    # get a patch
    patch, source = PatchSource(configuration).load_patch()
    if not patch:
        Console.error('Failed to get a patch%s. Please make sure to call the script from an SCM directory with local changes, or pipe in some input.'
              % (' from %s' % source if source else ''))
        sys.exit(1)

    # create or update the review
    if not configuration.repository and not configuration.no_anchor:
        detected_repository = rest.find_repository(source)
        if detected_repository:
            configuration.repository = detected_repository
            print('Detected Crucible repository %s. Use --repository or --noanchor options to override' % configuration.repository)
        else:
            print('No matching FishEye repository detected')
            configuration.choose_anchor(rest.get_repositories())

    configuration.last_project = rest.get_last_project()
    configuration.fill_interactively(get_id=True)

    review = rest.get_review(configuration.id)
    if review:
        rest.add_to_review(patch, review)
        sys.exit(0)

    project = rest.get_project(configuration.id)
    if project:
        configuration.fill_interactively(get_title=True, get_reviewers=True)
        rest.create_review(patch, project)
        sys.exit(0)

    Console.error('Failed to find a review or project with an id %s, make sure the id is correct.' % configuration.id)
    sys.exit(1)

if __name__ == '__main__':
    main()
