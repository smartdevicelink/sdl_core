#!/usr/bin/python
# -*- mode: python; coding: utf-8 -*-
# 
# Calculates number of lines, number of commented lines and commens coverage
# metric.
# 
# Change applink_source tuple to set directories to scan
#

import os
import string

# Scan following directories (recursive)
applink_source = ('src/appMain', 'src/components')
# Scan files which have following extensions
source_exts = 'c cc cpp cxx'.split()
header_exts = 'h hpp'.split()

#
# Returns file exension
#
def getFileExt(file):
    (root, ext) = os.path.splitext(file)
    if len(ext) > 0:
        return ext[1:]
    return ''

#
# Returns list of c++ files in specified directory (recursively)
#
def getFiles(dir):
    fileList = [[], []]
    for root, dirs, files in os.walk(dir):
        for file in files:
            fileExt = getFileExt(file)
            if fileExt in source_exts:
                fileList[1].append(root + '/' + file)
            if fileExt in header_exts:
                fileList[0].append(root + '/' + file)
    return fileList

#
# Checks if comment is not empty
#
def isCommentHasContent(comment):
    alnum = set(string.letters + string.digits)
    return len(set(comment) & alnum) > 0

#
# Proceed file
#
def proceedFile(file):
    countLines = 0
    countComments = 0
        
    f = open(file, 'r')
        
    incomment = False
        
    for line in f:
        line = line.strip()
        if len(line) == 0:
            continue
        # comment is opened with /*
        if incomment:
            end = line.find('*/')
            if end < 0:
                if isCommentHasContent(line):
                    countComments += 1
                continue
            else:
                if isCommentHasContent(line[:end]):
                    countComments += 1
                incomment = False
                line = line[end + 2:].strip()
                if len(line) < 1:
                    continue

        ind = line.find('/*')
        if ind >= 0:
            incomment = True
            ind2 = line[ind + 2:].find('*/')
            #TODO: Lines as "/* ... */ /* ... */" won't be handled correctly
            if ind2 >= 0:
                incomment = False
                if isCommentHasContent(line[ind + 2:ind + 2 + ind2]):
                    countComments += 1
                if (ind > 0) or (ind2 + 2 < len(line)):
                    countLines += 1
            else:
                if isCommentHasContent(line[ind + 2:]):
                    countComments += 1
                if ind > 0:
                    countLines += 1
            continue
        else:
            ind = line.find('//')
            if ind >= 0:
                if isCommentHasContent(line[ind + 2:]):
                    countComments += 1
            if ind == 0:
                continue
        countLines += 1
    return [countLines, countComments]

#
# Proceed files in the specified directory
#
def proceedDir(dir):
    hLines = 0
    cLines = 0
    hCLines = 0
    cCLines = 0
    fileList = getFiles(dir)
    for f in fileList[0]:
        lines = proceedFile(f)
        hLines += lines[0]
        hCLines += lines[1]
    for f in fileList[1]:
        lines = proceedFile(f)
        cLines += lines[0]
        cCLines += lines[1]

    print 'Directory: ' + dir
    print 'Number of C++ header files: %d' % len(fileList[0])
    print 'Number of C++ source files: %d' % len(fileList[1])
    print 'Number of source code lines: %d (%d lines in header files, %d lines in source files)' % (hLines + cLines, hLines, cLines)
    print 'Number of comment lines: %d (%d lines in header files, %d lines in source files)' % (hCLines + cCLines, hCLines, cCLines)
    print '----'

    pt = 0
    ph = 0
    pc = 0
    if (hLines + cLines) > 0:
        pt = (float)(hCLines + cCLines)/(float)(hLines + cLines) * 100.0
    if (hLines + hLines) > 0:
        ph = (float)(hCLines)/(float)(hLines) * 100.0
    if (hLines + cLines) > 0:
        pc = (float)(cCLines)/(float)(cLines) * 100.0
    print 'Comments code coverage: %f%% (%f%% of headers, %f%% of sources)' % (pt, ph, pc)
    print 
    
    return [hCLines + cCLines, hLines + cLines]

#
# main 
#
summary_lines = 0
summary_comments = 0
coverage = 0.0
for dir in applink_source:
    lines = proceedDir(dir)
    summary_comments += lines[0]
    summary_lines += lines[1]
if summary_lines > 0:
    coverage = (float)(summary_comments) / (float)(summary_lines) * 100.0 

print 
print '--- SUMMARY ---'
print 'Comments code coverage: %f%%' % (coverage)