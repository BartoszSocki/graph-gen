#!/bin/sh

graphalgo=./bin/graphalgo
index=1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

test_result_print() {
	if [ $? -eq 0 ]; then
		echo -e "[test $index] ${GREEN}passed${NC} $1"
	else
		echo -e "[test $index] ${RED}failed${NC} $1"
	fi
	index=$((index + 1))
}

# test 1
$graphalgo -g 2> out1
echo "graphalgo: not enough options passed" > out2
diff out1 out2
test_result_print 
rm out1 out2

# test 2
$graphalgo -g -g 2> out1
echo "graphalgo: -g, --generate passed multiple times" > out2
diff out1 out2
test_result_print
rm out1 out2

# test 3
$graphalgo -g -r4 -c4 -n0 -x1 -s0 > out1
$graphalgo --generate --rows=4 --cols=4 --min=0 --max=1 --seed=0 > out2
diff out1 out2
test_result_print
rm out1 out2

