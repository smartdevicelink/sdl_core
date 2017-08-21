#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
Format all sources with clang-format. All *.cc and *h in the src dir
are affected. Excluded from formatting sources in the "3rd_party" and
in the "3rd_party-static" dirs. For the formatting used ".clang-format"
in the project root.
"""

import os
from utils import setup_working_dir, walk_dir, run_cmd
import re


INCLUDE_PATTERNS = ['^.*\.cc$', '^.*\.h$', '^.*\.cpp$', '^.*\.hpp$']
EXCLUDE_PATTERNS = ['^.*3rd_party.*$']
FORMAT_CMD = 'clang-format -i -style=file {}'


def main():
    ''' Main logic '''
    setup_working_dir()
    print 'Current working dir is {}'.format(os.getcwd())

    def action(file_path):
        if re.match('|'.join(INCLUDE_PATTERNS), file_path, re.M | re.I):
            if not re.match('|'.join(EXCLUDE_PATTERNS),
                            file_path,
                            re.M | re.I):
                print 'Formatting file {}'.format(file_path)
                run_cmd(FORMAT_CMD.format(file_path))
    walk_dir('src', action)


if __name__ == '__main__':
    main()
