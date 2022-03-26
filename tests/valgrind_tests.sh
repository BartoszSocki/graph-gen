graphalgo="./bin/graphalgo"
valgrind="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --quiet"

# =========================
$valgrind $graphalgo -g 2> /dev/null

# =========================
$valgrind $graphalgo -g -g 2> /dev/null

# =========================
$valgrind $graphalgo -g -r4 -c4 -n0 -x1 -s0 > /dev/null

# =========================
$valgrind $graphalgo -l 2> /dev/null

# =========================
$valgrind $graphalgo -g -r-2 -c4 -n1 -x2 2> /dev/null

# =========================
$valgrind $graphalgo -g -r2 -c-4 -n1 -x2 2> /dev/null

# =========================
$valgrind $graphalgo -g -r2 -c4 -n3 -x2 2> /dev/null

# =========================
echo -e "\n" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
echo -e "1\n" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
echo "1 -1" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | head -n3 > out
cat out | $valgrind $graphalgo -b -1 0 2> /dev/null
rm out

# =========================
echo -e "2 2\n1:" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
echo -e "2 2\n111: 1" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
echo -e "2 2\n: 1" | $valgrind $graphalgo -b -1 0 2> /dev/null

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '2,3s/.*//' > out
cat out | $valgrind $graphalgo -b -1 0 > /dev/null
rm out

# =========================
$graphalgo -g -r2 -c2 -n0 -x1 -s0 | sed '4,5s/.*//' > out
cat out | $valgrind $graphalgo -b -1 0 > /dev/null
