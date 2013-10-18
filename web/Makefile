# This Makefile:
# 1. minimizes all .js files in src/ (except in lib/ and MCS-related files) and
# stores them in a parallel hierarchy in src.min/
# 2. minimizes .css files in src/, as specified in index.html (except mcs.css
# and commented lines), and compresses into one in src.min/css/
#
# Author: Eugene Nikolsky

root_dir = src
min_suffix = .min

JS_TARGETS = $(shell find $(root_dir) -type f -name '*.js' ! \( -path '*/lib/*' -or -path '*MCS*' \))
JS_MIN_TARGETS = $(JS_TARGETS:$(root_dir)/%=$(root_dir)$(min_suffix)/%)

CSS_TARGETS = $(shell sed -nE '/\.css/ { /(mcs\.css|<!--.*-->)/ { d; }; s/.*href="([^"]+)".*/$(root_dir)\/\1/; p; }' $(root_dir)/index.html)
CSS_MIN_TARGET = $(root_dir)$(min_suffix)/css/all.min.css

BIN_PATH = ../bin

JS_COMP = java -jar $(BIN_PATH)/js_compiler.jar
JS_COMP_FLAGS = --charset UTF-8 --jscomp_off internetExplorerChecks

CSS_COMP = java -jar $(BIN_PATH)/yuicompressor.jar
CSS_COMP_FLAGS = --type css


default: js css

.PHONY: js css

js: $(JS_MIN_TARGETS)

dir_guard = @mkdir -p $(@D)

$(root_dir)$(min_suffix)/%.js: $(root_dir)/%.js
	$(dir_guard)
	$(JS_COMP) $(JS_COMP_FLAGS) --js_output_file $@ $<

css: $(CSS_MIN_TARGET)

$(CSS_MIN_TARGET): $(CSS_TARGETS)
	$(dir_guard)
	@cat $^ | $(CSS_COMP) $(CSS_COMP_FLAGS) -v -o $@

clean:
	$(RM) -rf $(root_dir)$(min_suffix)

