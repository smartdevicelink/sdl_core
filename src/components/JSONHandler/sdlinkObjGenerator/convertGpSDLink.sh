x=`ls ../src/SDLRPCObjectsImpl/V2/*.gp`
echo $x
for fx in $x
do
	filename="${fx%.*}"
	echo $filename
	gperf $fx --output-file=$filename.inc
done
