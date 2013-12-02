#!/usr/bin/env bash

awk '/  Error / || /  Warning / || /  Info /' ./flexelint_result.txt > err_warn_inf.tmp
awk '!/Error 309: #error/' ./err_warn_inf.tmp > ./err_warn_inf_filtered1.tmp
awk '!/.lnt/' ./err_warn_inf_filtered1.tmp > ./err_warn_inf_filtered.tmp
awk '{system("./get_code_author.sh " $1 " " $2); print " " $0}' ./err_warn_inf_filtered.tmp > ./err_warn_inf_names.tmp

# Print list of commiters who have Warnings/Errors etc.
awk '{print $1}' ./err_warn_inf_names.tmp | awk '!x[$0]++'

# @TODO Check from names_and_aliases.txt if new aliases appeared

rm ./*.tmp 

