# ! /usr/bin/python3
__author__ = 'akutsan'

import subprocess
import getopt
import sys
import os

subscribers_file_name = "subscribers.cfg"
messages_filename = "additional_messages.cfg"

#files, that have subscribers
managed_files = {}

#messages, that should be send to subscribers by files
messages = {}
message_body = "Hello, %s \n\n %s \n\nBest regards,\nGit Spirit"
default_message = "Pay attention, that file: %s  was changed\n"
git_dir = ".git"
last_managed_commit_file_name = ".last_managed_commit"


def man():
    #print manual to use script
    print('''
Script send notification by email about changing files to subscribers.
Pay attention, that script use "mail" command.
"mail" should be installed and configured.

You can manage subscriptions in subscribers.cfg
You can manage subscription messages in additional_massages.cfg

Usage:
    -h --help \t\t\t: Show this hint
    -g <ARG> --git-dir <ARG>\t: specify git dir (default is current director
        Note: path should not contain links

Example:
    python3 email_notifier -g /home/sdl/applink/
    ''')


def parse_input():
    #read configuration files
    if not os.path.exists(subscribers_file_name):
        print("Configuration file is missing :", subscribers_file_name)
        exit(1)
    if not os.path.exists(messages_filename):
        print("Subscribers file is missing :", subscribers_file_name)
        exit(1)
    f = open(subscribers_file_name, "r+")
    lines = f.readlines()
    for line in lines:
        array_line = line.split(":")
        if len(array_line) != 2:
            print(subscribers_file_name, '''Has incorrect format "%s" ''' % line)
            exit(1)
        (filename, subscribers) = tuple(array_line)
        subscribers = subscribers.split(",")
        subscribers = list(map(lambda email: email.strip(), subscribers))  # remove whitespaces
        filename = filename.strip()
        managed_files[filename] = subscribers

    f = open(messages_filename, "r+")
    lines = f.readlines()
    for line in lines:
        array_line = line.split(":")
        if len(array_line) != 2:
            print(messages_filename, '''Has incorrect format "%s" ''' % line)
            exit(1)
        (filename, message) = tuple(array_line)
        filename = filename.strip()
        message = message.strip()
        messages[filename] = message

def last_managed_commit():
    if os.path.exists(last_managed_commit_file_name):
        f = open(last_managed_commit_file_name, "r+")
        return f.read().strip()
    return ""

def get_last_commit_info():
    #return information about last commit
    if not os.path.exists(git_dir):
        print(git_dir, ": No such file or directory")
        exit(1)
    proc = subprocess.Popen(['./last_commit.sh', git_dir], stdout=subprocess.PIPE)
    lines = str(proc.stdout.read()).split("\\n")
    commit_info = lines[0].split("|")
    modified_list = lines[1:]
    commit_info = list(map(lambda x: x.strip(), commit_info))  # remove whitespaces
    (author, email, commit_hash, message) = tuple(commit_info)
    res = {"hash": commit_hash, "author": author, "email": email, "message": message, "files": modified_list}
    return res


def send_mail(recipient, file_names, commit_hash=None):
    #sends email to recipient about changing file_names
    list_of_messages_to_send = []
    for file_name in file_names:
        if file_name in messages:
            message = messages[file_name]
        else:
            message = default_message % file_name
        list_of_messages_to_send.append(message)
    message_to_send = ""
    for message in list_of_messages_to_send:
        message_to_send = message_to_send + message + "\n"
    message_to_send = message_body % (recipient, message_to_send)
    cmd_args = ["mail", "-s", "'[smartDeviceLink] File changing notification'", recipient]
    print("$ ", end="")
    for x in cmd_args:
        print(x, end=" ")
    print("")
    print(message_to_send)
    proc = subprocess.Popen(cmd_args, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.communicate(input=message_to_send.encode())


def main():
    parse_input()
    commit_info = get_last_commit_info()
    prev_commit = last_managed_commit()
    if (commit_info["hash"] == prev_commit):
        print("Commit ", prev_commit, " was already managed")
        exit(1)
    recipients = {}  # mapping from user name to list of files
    changed_files = commit_info["files"]
    for changed in changed_files:
        if changed in managed_files.keys():
            subscribers = managed_files[changed]
            for subscriber in subscribers:
                subscriber_files = []
                if subscriber in recipients:
                    subscriber_files = recipients[subscriber]
                else:
                    recipients[subscriber] = subscriber_files
                subscriber_files.append(changed)
    for recipient in recipients:
        send_mail(recipient, recipients[recipient])
    print("Managed commit: ", commit_info["hash"])
    f = open(last_managed_commit_file_name, "w")
    f.write(commit_info["hash"])
    f.close()

options, remainder = getopt.getopt(sys.argv[1:], 'h:g:', ["help", "git-dir="])

if len(sys.argv[1:]) == 0:
    main()
    exit(0)

for opt, arg in options:
    if opt in ("-g", "--git-dir"):
        git_dir = arg + git_dir
        main()
        exit(0)
man()
exit(0)
