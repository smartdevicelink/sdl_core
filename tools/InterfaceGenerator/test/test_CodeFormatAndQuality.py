import subprocess
import unittest
import flake8.main
import pep257
import os.path
import fnmatch
import os
import pylint.lint
import sys

class TestCodeFormatAndQuality(unittest.TestCase):

    def setUp(self):
        self.projectRootDir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
        projectTestsDir = os.path.join(self.projectRootDir, "test")
        self.filesToAnalyze = []
        for root, dirnames, filenames in os.walk(self.projectRootDir):
            if root.startswith(projectTestsDir):
                continue # Currently we skipping test files
            for filename in fnmatch.filter(filenames, '*.py'):
                fullFileName = os.path.join(root, filename)
                relativeFileName = os.path.relpath(fullFileName, self.projectRootDir)
                self.filesToAnalyze.append(relativeFileName)

    def test_pep8_conformance(self):
        maxCyclomaticComplexity = 10
        errors = 0
        for file in self.filesToAnalyze:
            errors = errors + flake8.main.check_file(file, None, maxCyclomaticComplexity)

        self.assertEqual(errors, 0, "Found code style errors or warnings.")

    def test_pep257_conformance(self):
        errors = []

        for filePath in self.filesToAnalyze:
            print("Processing file: {0}".format(filePath))
            result = pep257.check_files([filePath])
            if result:
                errors.extend(result)
                for error in result:
                    print(error)
                print
        self.assertEqual(len(errors), 0, "Found Docstring Conventions violations.")

    def test_pylint_conformance(self):
        print
        self.assertEqual(0,
                         subprocess.call(
                             ["pylint",
                              '--rcfile=pylint.cfg',
                              'generator',
                              'Generator.py']
                         ), "Found Pylint violations")
        return

if __name__ == '__main__':
    unittest.main()

