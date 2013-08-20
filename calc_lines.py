#!/usr/bin/python
# 
# Calculates number of lines, number of commented lines and commens coverage
# metric.
# 
# Change sdl_source tuple to set directories to scan
#

import os
import string

# Scan following directories (recursive)
sdl_source = ('src/appMain', 'src/components')

# Scan files which have following extensions
source_exts = ('.c', '.cc', '.cpp', '.cxx')
header_exts = ('.h', '.hpp')

#
# Returns list of c++ files in specified directory (recursively)
#
def getFiles(dir):
    fileList = [[], []]
    for root, dirs, files in os.walk(dir):
        for file in files:
            (fileName, fileExt) = os.path.splitext(file)
            if fileExt in source_exts:
                fileList[1].append(root + '/' + file)
            if fileExt in header_exts:
                fileList[0].append(root + '/' + file)
    return fileList

#
# Checks if comment is not empty: contains any alphanumeric characters
#
def hasContent(comment):
    alnum = set(string.letters + string.digits)
    return len(set(comment) & alnum) > 0

#
# Proceed file
#
def proceedFile(file):
    countLines = 0
    countComments = 0
        
    countSize = os.path.getsize(file)
    f = open(file, 'r')
        
    inComment = False
        
    for line in f:
        line = line.strip()
        if len(line) == 0:
            continue
        
        # comment is opened with /*
        if inComment:
            indEnd = line.find('*/')
            if indEnd < 0:
                if hasContent(line):
                    countComments += 1
                continue
            else:
                if hasContent(line[:indEnd]):
                    countComments += 1
                inComment = False
                line = line[indEnd + 2:].strip()
                if len(line) < 1:
                    continue

        indStart = line.find('/*')
        if indStart >= 0:
            inComment = True
            indEnd = line[indStart + 2:].find('*/')
            #TODO: Lines as "/* ... */ /* ... */" won't be handled correctly
            if indEnd >= 0:
                inComment = False
                if hasContent(line[indStart + 2:indStart + indEnd + 2]):
                    countComments += 1
                if (indStart > 0) or (indEnd + 2 < len(line)):
                    countLines += 1
            else:
                if hasContent(line[indStart + 2:]):
                    countComments += 1
                if indStart > 0:
                    countLines += 1
            continue
        else:
            indStart = line.find('//')
            if indStart >= 0:
                if hasContent(line[indStart + 2:]):
                    countComments += 1
            if indStart == 0:
                continue
        countLines += 1
    return [countLines, countComments, countSize]

#
# Proceed files in the specified directory
#
def proceedDir(dir):
    linesHeaderCode = 0
    linesHeaderComments = 0
    sizeHeaders = 0
    linesSourceCode = 0
    linesSourceComments = 0
    sizeSources = 0
    (listHeaders, listSources) = getFiles(dir)
    for f in listHeaders:
        (sources, comments, size) = proceedFile(f)
        linesHeaderCode += sources
        linesHeaderComments += comments
        sizeHeaders += size
    for f in listSources:
        (sources, comments, size) = proceedFile(f)
        linesSourceCode += sources
        linesSourceComments += comments
        sizeSources += size

    linesCode = linesHeaderCode + linesSourceCode
    linesComments = linesHeaderComments + linesSourceComments

    print 'Directory: ' + dir
    print 'Number of scanned C++ header files: %d. Size: %0.1f Kb' \
        % (len(listHeaders), sizeHeaders / 1024.0)
    print 'Number of scanned C++ source files: %d. Size: %0.1f Kb' \
        % (len(listSources), sizeSources / 1024.0)
    print 'Number of source code lines: %d (header files: %d, source files: %d)' \
        % (linesCode, linesHeaderCode, linesSourceCode)
    print 'Number of comment lines: %d (header files: %d, source files: %d)' \
        % (linesComments, linesHeaderComments, linesSourceComments)
    print '----'

    pt = 0
    ph = 0
    pc = 0
    if (linesCode) > 0:
        pt = (float)(linesComments)/(float)(linesCode) * 100.0
    if (linesHeaderCode) > 0:
        ph = (float)(linesHeaderComments)/(float)(linesHeaderCode) * 100.0
    if (linesSourceCode) > 0:
        pc = (float)(linesSourceComments)/(float)(linesSourceCode) * 100.0
    print 'Code coverage by comments: %f%% (header files: %f%%, source files: %f%%)' \
        % (pt, ph, pc)
    print 
    
    return [linesComments, linesCode, sizeHeaders + sizeSources]

#
# main 
#
summary_lines = 0
summary_comments = 0
summary_size = 0
coverage = 0.0
for dir in sdl_source:
    (comments, code, size) = proceedDir(dir)
    summary_comments += comments
    summary_lines += code
    summary_size += size
if summary_lines > 0:
    coverage = (float)(summary_comments) / (float)(summary_lines) * 100.0 

print 
print '--- SUMMARY ---'
print 'Size of source code: %0.1f Kb' % (summary_size / 1024.0)
print 'Comments code coverage: %f%%' % (coverage)