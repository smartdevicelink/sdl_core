#!/usr/bin/env bash

# Variable to store total amount of disabled unit tests
total_disabled=0

echo ""
echo "===== DISABLED Unit-tests amount ===== "
echo ""

# Getting the list of all UT executables
for test_file in $(ctest -N | awk '{print $3}'); do
        full_path=`find . -name $test_file -type f 2>/dev/null`
	if [ $full_path ] ; then
        # Count amount of disabled tests cases
		count_of_disabled=$(${full_path} --gtest_list_tests | grep DISABLED | wc -l)
		if [ 0 != $count_of_disabled ] ; then
		let "total_disabled = $total_disabled + $count_of_disabled"
			echo $count_of_disabled " : " $test_file;
		fi
	fi
done
echo ""
echo "TOTAL AMOUNT OF DISABLED TESTS : " $total_disabled;

exit 0


