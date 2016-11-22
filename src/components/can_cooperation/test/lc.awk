#!/usr/bin/awk
#----------------------------------------------------------------------------
#
# Awk program to count lines of code in C source files.
# Has logic to keep track of comments as well.
#
# Assumptions/limitations:
#  - Does *not* undestand inline asm - counted as source lines!
#  - Does not include blank lines in the average
#  - Assumes no code on a line after a "*/" - poss change this?
#  - Counts *any* non-comment line as code eg { or } by themselves, etc
#  - depends on hash order of awk - this is risky! (See END code)
#  - Presently, the output redirects quite badly - command line option?
#  - Overflow if any number >32768 - mawk limitation.
#
# Paul Hubbard phubbard@computer.org written 5/1993
# Apache 2.0 license - enjoy!

#----------------------------------------------------------------------------

BEGIN {		# Init vars at startup
	print ""
}

#----------------------------------------------------------------------------
# Main loop

# Print filename on read of first record
FNR == 1 {
	printf("\r                                                           ")
	printf("\rReading file: %s", FILENAME)
	comments[FILENAME] = 0
	lines[FILENAME] = 0
	fname[FILENAME] = FILENAME
	raw_linecount[FILENAME] = 0
	in_comment = 0
}

# Increment line count as first thing done
raw_linecount[FILENAME]++{}

# Test for "//" in the first record
($1 ~ /\/\//) {		   # single line comment - skip to next rec
	comments[FILENAME]++
	next
}

# Test for "//" in rest of the record (comment after code - counts as both)
/\/\// {
	in_comment = 0
	comments[FILENAME]++	# lines count will be incremented below
}

# This wierd "/\/\*/" junk is awk-ish for the string "/*" - aka comment begin
/\/\*/ {			# Begin multi-line comment
	if($1 ~ /\/\*/) {	# Comment is first record - no code here!
		in_comment = 1
	}
	else {			# "/*" type comment AFTER code - is both
		in_comment = 0
		comments[FILENAME]++	# Lines array inc below
	}
}

# And this is "*/" aka comment end
/\*\// {			# End of multi-line comment
	if(in_comment = 1){	# Multi line comment - can increment
		comments[FILENAME]++
		in_comment = 0	# Reset Boolean
		next		# Assumes no code after "*/" !!
	}
}

# Test for empty lines and skip
(length($0) == 0) {next}

(in_comment > 0) {comments[FILENAME]++}
(in_comment == 0) {lines[FILENAME]++}

#----------------------------------------------------------------------------
# Termination code
END {

	# Kluge clear EOL
	printf("\r                                                          \n")

	CCT = LCT = 0		# init totals
	idx = 0			# init pseudo-index
	maxlen = 0
	name_len = 0

	# Walk thru file names array to get maxlen
	for(i in fname) {
		if((length(fname[i])) > maxlen) {maxlen = length(fname[i])}
	}

	maxlen++
	name_len = maxlen	# save maxlen for formatting header
	if(name_len < 4) {name_len = 4}

	# Build formatting string for sprintf; eg "%15s " to beautify output
	spf_str = "%"
	maxlen = maxlen "s  "	# Convert number to string and add trailer
	spf_str = spf_str maxlen# Add pieces together to build format str

	# Walk thru file names array
	for(i in fname) {
		output[idx] = sprintf(spf_str, fname[i])
		idx++
	}

	idx = 0

	# Walk thru lines array
	for(i in lines) {
		tempstr = sprintf("%5d", lines[i])
		output[idx] = output[idx] tempstr
		LCT += lines[i]
		idx++
	}

	idx = 0

	# Walk thru comments array
	for(i in comments) {
		tempstr = sprintf("%11d", comments[i])
		output[idx] = output[idx] tempstr
		CCT += comments[i]
		idx++
	}

	idx = 0
	raw_lc = 0

	# Walk thru array of raw line counts (includes empty lines)
	for(i in raw_linecount) {
		tempstr = sprintf("%12d", raw_linecount[i])
		output[idx] = output[idx] tempstr
		idx++
		raw_lc += raw_linecount[i]
	}

	# Print header
	for(i = 0; i < (name_len - 4); i++) {printf"-"}
	str = "File---Code---Comments---Raw Lines"
	printf(str)

	j = length(str) + (name_len - 4)

	# Finish off the header line
	printf("\n")
	for(i = j; i < 80; i++) {printf("-")}

	# Dump constructed lines of data
	for(i = 0; i < idx; i++) {print output[i]}

	# Print trailer line
	for(i = 0; i < 80; i++) {printf("-")}

	#Dump totals
	printf("\n Total code: %d", LCT)
	printf("\n Total comments: %d", CCT)
	printf("\n Total raw lines: %d\n", raw_lc)

	if((LCT + CCT) > 0) {
		printf("Comment percentage: %f", (CCT / (LCT + CCT)) * 100)
	}

	print ""
}






































