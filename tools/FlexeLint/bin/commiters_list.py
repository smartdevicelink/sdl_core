#!/usr/bin/env python
import sys

import gittools

commiter_infos = gittools.BranchCommiters()
for commiter in commiter_infos:
  print commiter.email.split('@')[0] + "\t" + commiter.name.replace(' ', '')
