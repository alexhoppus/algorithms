for i in `seq 1 2`
do
echo "Solution $i"
g++ -o main$i ./solution$i.cpp -g -std=c++11

time ./main$i < input
done

