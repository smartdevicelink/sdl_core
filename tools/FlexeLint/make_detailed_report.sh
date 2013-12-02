#!/usr/bin/env bash

rm -R ./developers
mkdir ./developers

awk '/Error / || /Warning / || /Info /' ./flexelint_result.txt > err_warn_inf.tmp
awk '!/Error 309: #error/' ./err_warn_inf.tmp > ./err_warn_inf_filtered1.tmp
awk '!/.lnt/' ./err_warn_inf_filtered1.tmp > ./err_warn_inf_filtered.tmp
awk '{system("./get_code_author.sh " $1 " " $2); print " " $0}' ./err_warn_inf_filtered.tmp > ./err_warn_inf_names.tmp

#name="${developers[6]}"
#echo $name
#alias="${!name}"
#echo $alias
#alias1="${!name[3]}"
#echo $alias1

#for i in ${!developers[@]}; do
#	echo ${developers[i]}
#	echo =================================================
#	
#	alias="${!developers[i]}"
#
#	for j in ${alias[@]}; do
#		echo ${!alias[j]}
#	done
#	echo =================================================
#
#	echo
#	echo 
#done

# @TODO make everything in loop
#################################################################################################################################
#touch ./developers/${developers[0]}
#awk '$1 == "Kozyrenko,Igor" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[0]}

#touch ./developers/${developers[1]}
#awk '$1 == "PolinaVyshnevska" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[1]}

#touch ./developers/${developers[2]}
#awk '$1 == "KonstantinKolodiy" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[2]}

#touch ./developers/${developers[3]}
#awk '$1 == "AnatoliyLeshin" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[3]}

#touch ./developers/${developers[4]}
#awk '$1 == "DmitryChmerev" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[4]}

#touch ./developers/${developers[5]}
#awk '$1 == "nkhlopkov" || $1 == "NikolayKhlopkov" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[5]}

#touch ./developers/${developers[6]}
#awk '$1 == "PavelKramchaninov" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[6]}

#touch ./developers/${developers[7]}
#awk '$1 == "DmitryKlimenko" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[7]}

#touch ./developers/${developers[8]}
#awk '$1 == "KandulAlexander" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[8]}

#touch ./developers/${developers[9]}
#awk '$1 == "AndreyOleynik" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[9]}

#touch ./developers/${developers[10]}
#awk '$1 == "DmitriyTrunov" {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/${developers[10]}
#################################################################################################################################



while read line    
do
	name=$(echo "$line" | awk '{print $1}') 
		
	touch ./developers/$name

	for alias in $(echo "$line" | awk '{$1=""; print $0}'); do
		awk -v a=$alias '$1 == a {$1=""; $2=""; print $0}' ./err_warn_inf_names.tmp | awk '!x[$0]++' >> ./developers/$name
        done

done < names_and_aliases.txt

rm ./*.tmp 

