while true
do
cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $[ 1 + $[ RANDOM % 999 ]] | head -n 1 > /tmp/test_seq
cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w $[ 1 + $[ RANDOM % 999 ]] | head -n 1 >> /tmp/test_seq
ans1=$(./main1 < /tmp/test_seq)
ans2=$(./main2 < /tmp/test_seq)
echo $ans1 $ans2
if [ $ans1 != $ans2 ]
then
	echo "Error!";
	exit;
fi
done

