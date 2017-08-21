#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Install (copy) git hooks
"""

import os
import glob
import shutil
from utils import setup_working_dir


def uninstall_hooks(hooks_dir):
    print 'Deleting existing pre-commit hooks from {}'.format(hooks_dir)
    files = glob.glob(os.path.join(hooks_dir, 'pre-commit*'))
    for item in files:
        os.remove(item)


def install_hooks(src_dir, dst_dir):
    print 'Installing pre-commit hooks'
    src_files = glob.glob(os.path.join(src_dir, 'pre-commit*'))
    for item in src_files:
        shutil.copy(item, dst_dir)


def main():
    ''' Main logic '''
    setup_working_dir()
    print 'Current working dir is {}'.format(os.getcwd())
    hooks_src_dir = os.path.join(
        os.getcwd(), 'tools', 'infrastructure', 'git-hooks')
    hooks_dst_dir = os.path.join(os.getcwd(), '.git', 'hooks')
    uninstall_hooks(hooks_dst_dir)
    install_hooks(hooks_src_dir, hooks_dst_dir)


if __name__ == '__main__':
    main()
