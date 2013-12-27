#!/usr/bin/env python
import re
import fileinput

patch_file_pattern_old = re.compile(r'^--- (.*)')
patch_file_pattern_new = re.compile(r'^\+\+\+ (.*)')
hunk_info_pattern = re.compile(r'^@@ -\d+(?:,\d+)? \+(\d+)(?:,(\d+))? @@')
hunk_line_pattern = re.compile(r'^[+ -]')

class PatchParser:
	def __init__(self, patchInput):
		self._filePatchInfo = {}
		self._input = patchInput
		self._lastLine = None
	def _getLine(self):
		if self._lastLine is not None:
			lastLine = self._lastLine
			self._lastLine = None
			return lastLine
		else:
			return self._input.readline()
	def _ungetLine(self, line):
		if self._lastLine is not None:
			raise RuntimeError("Only one line can be unget")
		else:
			self._lastLine = line
	def addedLinesCount(self):
		while self._findFilePatch(): self._readFilePatch()
		return self._filePatchInfo
	def _findFilePatch(self):
		while True:
			line = self._getLine()
			if not line: return False
			if patch_file_pattern_old.match(line): return True
	def _readFilePatch(self):
		match = patch_file_pattern_new.match(self._getLine())
		if not match:
			raise RuntimeError("Invalid patch, new file name is not found")
		self._filePatchInfo[match.group(1)] = self._addedLinesInHunks()
	def _addedLinesInHunks(self):
		hunk = self._addedLinesInHunk()
		if hunk:
			return hunk | self._addedLinesInHunks()
		else:
			return hunk
	def _addedLinesInHunk(self):
		line = self._getLine()
		match = hunk_info_pattern.match(line)
		if match:
			header = match.groups(1)
			return self._countAddedLines(*header)
		else:
			self._ungetLine(line)
			return set()
	def _countAddedLines(self, firstLine, totalLines):
		addedLines = set()
		currentLine = int(firstLine)
		lastLine = currentLine + int(totalLines)
		while currentLine != lastLine:
			line = self._getLine()
			if not hunk_line_pattern.match(line):
				raise RuntimeError("Invalid patch line: " + line)
			if line[0] != '-':
				if line[0] == '+':
					addedLines.add(currentLine)
				currentLine += 1
		return addedLines



print PatchParser(fileinput.input()).addedLinesCount()
