#! /usr/bin/python

import sys
import itertools

if len(sys.argv) != 3:
	sys.stderr.write("Usage: {0} ver1 ver2\n".format(sys.argv[0]))
	exit(1)
ver1 = map(lambda x: int("".join(itertools.takewhile(str.isdigit, x))), sys.argv[1].split('.'))
ver2 = map(lambda x: int("".join(itertools.takewhile(str.isdigit, x))), sys.argv[2].split('.'))

while len(ver1) < len(ver2):
	ver1.append(0)

while len(ver2) < len(ver1):
	ver2.append(0)

idx = 0

while idx < len(ver1) and ver1[idx] == ver2[idx]:
	idx += 1

if idx == len(ver1):
	print "equal\n"
elif ver1[idx] > ver2[idx]:
	print "1 > 2\n"
else:
	print "2 > 1\n"
