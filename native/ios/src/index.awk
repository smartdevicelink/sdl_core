# Creates minified version of index.html
# Author: Eugene Nikolsky

# called at the beginning of the program
BEGIN {
    # whether minimal css has been added
    mincssadded=0
}
# called at every line
{
    # whether further processing of the current line is required
    process=1
    # whether current line is about css
    cssline=0
}
# remove lines with comments only
/[[:space:]]*<!--.*-->[[:space:]]*/ {
    process=0
}
# remove lines mentioning MCS
/MCS/ {
    process=0
}
# current line adds css
/<link rel="stylesheet"/ {
    process=0
    cssline=1
}
# if it's the first css line, and minimal css has been added yet,
# add it
cssline==1 && mincssadded==0 {
    print "\t\t<link rel='stylesheet' type='text/css' href='css/all.min.css' />"
    mincssadded=1
}
# remove empty lines
process==1 && NF==0 {
    process=0
}
# print what's left
process==1 {
    print $0
}
