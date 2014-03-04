#!/usr/bin/env python
import argparse
import email.utils
import sys

import gittools
import HTML

# Subdirectory where result files are kept
kResultsDir = ""

def MakeListItem(commiter_info):
  report_file_name = commiter_info.email.split('@')[0] + ".txt";
  return HTML.link(commiter_info.name, kResultsDir + report_file_name)


# Parse args
parser = argparse.ArgumentParser(
  description='Generate html file with links to commiter reports.')
parser.add_argument('destfile', metavar='destfile', type=argparse.FileType('w'),
                   default=sys.stdout, nargs='?',
                   help='html file with links')
args = parser.parse_args()

commiter_infos = gittools.BranchCommiters()

# Produce HTML
commiters_link_list = map(MakeListItem, commiter_infos)
commiters_link_list.append(HTML.link("Total", kResultsDir + "total.txt"))
args.destfile.write("<H1>Flexelint reports for users</H1>")
args.destfile.write(HTML.list(commiters_link_list))
