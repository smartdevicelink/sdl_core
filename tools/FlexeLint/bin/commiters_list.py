#!/usr/bin/env python
import sys
import argparse
import gittools

parser = argparse.ArgumentParser()
parser.add_argument('gitrepo', metavar='gitrepo')
args = parser.parse_args()

commiter_infos = gittools.BranchCommiters(args.gitrepo)
for commiter in commiter_infos:
  print commiter.email.split('@')[0] + "\t" + commiter.name.replace(' ', '')
