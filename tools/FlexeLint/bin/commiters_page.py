#!/usr/bin/env python
import argparse
import email.utils
import subprocess
import sys
import HTML

class CommiterInfo(object):
  """ Commiter description structure """
  def __init__(self, shortlog_line):
    fields = shortlog_line.split('\t')
    self.commit_count = int(fields[0])
    name_email = email.utils.parseaddr(fields[1])
    self.name = name_email[0]
    self.email = name_email[1]

def MakeListItem(commiter_info):
  report_file_name = commiter_info.email.split('@')[0] + ".txt";
  return HTML.link(commiter_info.name,
                   kResultsDir + report_file_name.lower())

# Subdirectory where result files are kept
kResultsDir = ""

# Parse args
parser = argparse.ArgumentParser(
  description='Generate html file with links to commiter reports.')

parser.add_argument('destfile', metavar='destfile', type=argparse.FileType('w'),
                   default=sys.stdout, nargs='?',
                   help='html file with links')
args = parser.parse_args()

# Call git and parse it's output
git_shortlog = subprocess.check_output(["git", "shortlog", "-sne"],
                                       stderr=subprocess.PIPE)
commiter_infos = map(lambda s: CommiterInfo(s), git_shortlog.splitlines())

# Produce HTML
commiters_link_list = map(MakeListItem, commiter_infos)
commiters_link_list.append(HTML.link("Total", kResultsDir + "total.txt"))
args.destfile.write("<H1>Flexelint reports for users</H1>")
args.destfile.write(HTML.list(commiters_link_list))
