#!/bin/sh

graphalgo=./bin/graphalgo
index=1

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

assert_success() {
	if [ $? -eq 0 ]; then
		echo -e "[test $index] ${GREEN}passed${NC} $1"
	else
		echo -e "[test $index] ${RED}failed${NC} $1"
	fi
	index=$((index + 1))
}

assert_fail() {
	if [ $? -ne 0 ]; then
		echo -e "[test $index] ${GREEN}passed${NC} $1"
	else
		echo -e "[test $index] ${RED}failed${NC} $1"
	fi
	index=$((index + 1))
}
# =========================
./bin/test_dijkstra 1>/dev/null 
assert_fail Dijkstra  

# =========================
./bin/test_pq 1>/dev/null 
assert_fail VertexPriorityQueue  

# =========================
./bin/test_bfs 1> /dev/null
assert_success bfs tests

# =========================
$graphalgo -g 2> /dev/null
assert_fail missing flags 

# =========================
$graphalgo -g -g 2> /dev/null
assert_fail 2x the same flag

# =========================
$graphalgo -g -r4 -c4 -n0 -x1 -s0 > out1
$graphalgo --generate --rows=4 --cols=4 --min=0 --max=1 --seed=0 > out2
diff out1 out2
assert_success
rm out1 out2

# =========================
$graphalgo -l 2> /dev/null
assert_fail

# =========================
$graphalgo -g -r-2 -c4 -n1 -x2 2> /dev/null
assert_fail "rows < 1"

# =========================
$graphalgo -g -r2 -c-4 -n1 -x2 2> /dev/null
assert_fail  "cols < 1"

# =========================
$graphalgo -g -r2 -c4 -n3 -x2 2> /dev/null
assert_fail "min > max"

# =========================
echo -e "\n" | $graphalgo -b -1 0 2> /dev/null
assert_fail "no graph size provided"

# =========================
echo -e "1\n" | $graphalgo -b -1 0 2> /dev/null
assert_fail "only rows provided"

# =========================
echo "1 -1" | $graphalgo -b -1 0 2> /dev/null
assert_fail "graph size < 1"

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | head -n3 | $graphalgo -b -1 0 2> /dev/null
assert_fail "incomplete file: missing verticies"

# =========================
echo -e "2 2\n1:" | $graphalgo -b -1 0 2> /dev/null
assert_fail "incomplete file: missing weight"

# =========================
echo -e "2 2\n111: 1" | $graphalgo -b -1 0 2> /dev/null
assert_fail "invalid vertex index"

# =========================
echo -e "2 2\n: 1" | $graphalgo -b -1 0 2> /dev/null
assert_fail "incomplete file: missing vertex"

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '2,3s/.*//' | $graphalgo -b -1 0 > /dev/null
assert_success "directed graph from stdin 1"

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '4,5s/.*//' | $graphalgo -b -1 0 > /dev/null
assert_success "directed graph from stdin 2"


