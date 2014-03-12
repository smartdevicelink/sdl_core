# A simple Makefile to build the documentation in HTML format.
#
# Author: Eugene Nikolsky

MAIN_FILE = README.html
IMAGES = $(wildcard images/*)

default: html

html: ${MAIN_FILE}

%.html: %.asciidoc ${IMAGES}
	asciidoc -b html $<

