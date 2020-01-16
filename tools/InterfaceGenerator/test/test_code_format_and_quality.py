# pylint: disable=C0103, C0301, C0115, C0116
"""Interface model unit test

"""

import unittest
from os import walk
from os.path import join
from pathlib import Path

from pylint.lint import Run


class CodeFormatAndQuality(unittest.TestCase):
    def setUp(self):
        """Searching for all python files to be checked

        """
        self.list_of_files = []
        root = Path(__file__).absolute().parents[1]
        for (directory, _, filenames) in walk(root.as_posix()):
            self.list_of_files += [join(directory, file) for file in filenames
                                   if file.endswith('.py') and not file.startswith('test')]
        # self.list_of_files.append('--rcfile=../pylint.ini')

    def test_check(self):
        """Performing checks by PyLint

        """
        results = Run(self.list_of_files, do_exit=False)
        score = results.linter.stats['global_note']
        self.assertGreaterEqual(score, 4.3)


if __name__ == '__main__':
    unittest.main()
