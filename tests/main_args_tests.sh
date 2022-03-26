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

# =========================
$graphalgo -g 2> out1
echo "graphalgo: unfamiliar combination of program options" > out2
diff out1 out2
test_result_print 
rm out1 out2

# =========================
$graphalgo -g -g 2> out1
echo "graphalgo: -g, --generate passed multiple times" > out2
diff out1 out2
test_result_print 
rm out1 out2

# =========================
$graphalgo -g -r4 -c4 -n0 -x1 -s0 > out1
$graphalgo --generate --rows=4 --cols=4 --min=0 --max=1 --seed=0 > out2
diff out1 out2
test_result_print
rm out1 out2

# =========================
$graphalgo -l 2> out1
echo "graphalgo: invalid flag" > out2
diff out1 out2
test_result_print "invalid flag test"
rm out1 out2

# =========================
$graphalgo -g -r-2 -c4 -n1 -x2 2> out1
echo "graphalgo: rows and cols cannot be smaller than 1" > out2
diff out1 out2
test_result_print "rows < 1"
rm out1 out2

# =========================
$graphalgo -g -r2 -c-4 -n1 -x2 2> out1
echo "graphalgo: rows and cols cannot be smaller than 1" > out2
diff out1 out2
test_result_print "cols < 1"
rm out1 out2

# =========================
$graphalgo -g -r2 -c4 -n3 -x2 2> out1
echo "graphalgo: min cannot be greater than max" > out2
diff out1 out2
test_result_print "min > max"
rm out1 out2

# =========================
echo -e "\n" | $graphalgo -b -1 0 2> out1
echo "graphalgo: invalid number of graph dimensions" > out2
diff out1 out2
test_result_print "no graph size provided"
rm out1 out2

# =========================
echo -e "1\n" | $graphalgo -b -1 0 2> out1
echo "graphalgo: invalid number of graph dimensions" > out2
diff out1 out2
test_result_print "only rows provided"
rm out1 out2

# =========================
echo "1 -1" | $graphalgo -b -1 0 2> out1
echo "graphalgo: invalid values of graph dimensions" > out2
diff out1 out2
test_result_print "graph size < 1"
rm out1 out2

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | head -n3 | $graphalgo -b -1 0 2> out1
echo "graphalgo: file incomplete" > out2
diff out1 out2
test_result_print "incomplete file: missing verticies"
rm out1 out2

# =========================
echo -e "2 2\n1:" | $graphalgo -b -1 0 2> out1
echo "graphalgo: file incomplete" > out2
diff out1 out2
test_result_print "incomplete file: missing weight"
rm out1 out2

# =========================
echo -e "2 2\n111: 1" | $graphalgo -b -1 0 2> out1
echo "graphalgo: invalid vertex index" > out2
diff out1 out2
test_result_print "invalid vertex index"
rm out1 out2

# =========================
echo -e "2 2\n: 1" | $graphalgo -b -1 0 2> out1
echo "graphalgo: file incomplete" > out2
diff out1 out2
test_result_print "incomplete file: missing vertex"
rm out1 out2

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '2,3s/.*//' | $graphalgo -b -1 0 > /dev/null
test_result_print "directed graph from stdin 1"

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '4,5s/.*//' | $graphalgo -b -1 0 > /dev/null
test_result_print "directed graph from stdin 2"
