g++ ./src/lab1.cpp -o run
echo "***Сompilation completed***"

FILES=./tests/*
for test in $FILES
do
	echo "INPUT:"
	cat $test
	echo "OUTPUT:"
	./run < $test
	echo "========================"
done
    rm run
    pause
    cmd