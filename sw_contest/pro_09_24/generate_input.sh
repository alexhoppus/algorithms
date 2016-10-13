TEST_MAX=1000
N_MIN=5
N_MAX=25
count=1
FN="ginput"

T=$((1 + $RANDOM % $TEST_MAX))
echo $T > $FN
while [ "$count" -le $T ]
do
	N=$((N_MIN + $RANDOM % $(($N_MAX - $N_MIN))))
	SR1=$((1 + $RANDOM % $(($(($N - 1))))))
	SC1=$((1 + $RANDOM % $(($(($N - 1))))))
	SR2=$((1 + $RANDOM % $(($(($N - 1))))))
	SC2=$((1 + $RANDOM % $(($(($N - 1))))))
	GR1=$((1 + $RANDOM % $(($(($N - 1))))))
	GC1=$((1 + $RANDOM % $(($(($N - 1))))))
	GR2=$((1 + $RANDOM % $(($(($N - 1))))))
	GC2=$((1 + $RANDOM % $(($(($N - 1))))))
	echo $N >> $FN
	echo -n $SR1 $SC1 $GR1 $GC1 >> $FN
	echo >> $FN
	echo -n $SR2 $SC2 $GR2 $GC2 >> $FN
	echo >> $FN
	for i in `seq 1 $N`
	do
		for j in `seq 1 $N`
		do
			prob=$(($RANDOM % 100))
			val=0
			if [ $prob -gt 80 ]; then
				val=1
			fi
			echo -n $val" " >> $FN
		done
		echo >> $FN
	done
	let "count += 1"  # Increment count.
done
