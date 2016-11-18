TEST_MAX=100
N_MAX=1000
N_MIN=1
count=1
FN="ginput"
FNCLONE="/tmp/ginput"
rm -f $FNCLONE
rm -f $FN

T=$((1 + $RANDOM % $TEST_MAX))
echo $T > $FN
while [ "$count" -le $T ]
do
	N=$((N_MIN + $RANDOM % $(($N_MAX - $N_MIN))))
	echo $N >> $FN
	for i in `seq 1 $N`
	do
		t=$((1 + $RANDOM % 4999))
		l=$((1 + $RANDOM % 9))
		echo $t" "$l >> $FNCLONE
	done
	cat $FNCLONE | sort -n -k1 >> $FN
	rm -f $FNCLONE
	let "count += 1"  # Increment count.
done


