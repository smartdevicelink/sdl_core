x=`ls ../src/ALRPCObjectsImpl/*.gp`
echo $x
for fx in $x
do
	filename="${fx%.*}"
	echo $filename
	gperf $fx --output-file=$filename.inc
done
