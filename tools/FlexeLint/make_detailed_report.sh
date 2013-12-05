#!/usr/bin/env bash

rm -Rf ./developers
mkdir ./developers

awk '/Error / || /Warning / || /Info / || /Note /' ./flexelint_result.txt > err_warn_inf.tmp
awk '!/0  Note 1960/' ./err_warn_inf.tmp > ./err_warn_inf_filtered1.tmp
awk '!/Error 309: #error/' ./err_warn_inf_filtered1.tmp > ./err_warn_inf_filtered2.tmp
awk '!/.lnt/' ./err_warn_inf_filtered2.tmp > ./err_warn_inf_filtered.tmp
awk '{system("./get_code_author.sh " $1 " " $2); print " " $0}' ./err_warn_inf_filtered.tmp > ./err_warn_inf_names.tmp

touch ./developers/total.txt

while read line    
do
	name=$(echo "$line" | awk '{print $1}') 
		
	for alias in $(echo "$line" | awk '{$1=""; print $0}'); do
		awk -v a=$alias '$1 == a {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers_report.tmp
        done

	error_quantity=$(awk -v val="Error" -v counter=0 '$3==val {counter++} END {print counter}' ./developers_report.tmp)
	warning_quantity=$(awk -v val="Warning" -v counter=0 '$3==val {counter++} END {print counter}' ./developers_report.tmp)
	info_quantity=$(awk -v val="Info" -v counter=0 '$3==val {counter++} END {print counter}' ./developers_report.tmp)
	note_quantity=$(awk -v val="Note" -v counter=0 '$3==val {counter++} END {print counter}' ./developers_report.tmp)

	touch ./developers/$name.txt

	echo 	Summary >> ./developers/$name.txt
	echo >> ./developers/$name.txt
	echo -e ' \t 'Errors:   $error_quantity >> ./developers/$name.txt
	echo -e ' \t 'Warnings: $warning_quantity >> ./developers/$name.txt
	echo -e ' \t 'Infos:    $info_quantity >> ./developers/$name.txt
	echo -e ' \t 'Notes:    $note_quantity >> ./developers/$name.txt
	echo >> ./developers/$name.txt
	echo >> ./developers/$name.txt	

	cat developers_report.tmp >> ./developers/$name.txt
	rm ./developers_report.tmp

done < names_and_aliases.txt

total_error_quantity=$(awk '{print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' | awk -v val="Error" -v counter=0 '$5==val {counter++} END {print counter}')
total_warning_quantity=$(awk '{print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' | awk -v val="Warning" -v counter=0 '$5==val {counter++} END {print counter}')
total_info_quantity=$(awk '{print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' | awk -v val="Info" -v counter=0 '$5==val {counter++} END {print counter}')
total_note_quantity=$(awk '{print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' | awk -v val="Note" -v counter=0 '$5==val {counter++} END {print counter}')

awk '{print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> list_of_err_warn_inf_note.txt

echo 	Summary >> ./developers/total.txt
echo >> ./developers/total.txt
echo -e ' \t 'Errors:   $total_error_quantity >> ./developers/total.txt
echo -e ' \t 'Warnings: $total_warning_quantity >> ./developers/total.txt
echo -e ' \t 'Infos:    $total_info_quantity >> ./developers/total.txt
echo -e ' \t 'Notes:    $total_note_quantity >> ./developers/$name.txt

rm ./*.tmp 

