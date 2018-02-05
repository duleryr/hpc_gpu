#!/bin/bash
a="FW_seq.txt"
b="FW_par1_l.txt"
c="FW_par1_c.txt"
d="FW_par2.txt"
e="FW_seq_tiled.txt"
f="FW_par_tiled.txt"
g="FW_seq_tiled_layout.txt"
h="FW_par_tiled_layout.txt"
j="FW_seq_tiled_layout_memPos.txt"
k="FW_seq_tiled_layout2.txt"
l="FW_seq_mem.txt"
m="FW_seq_1d.txt"
n="FW_par_1d.txt"

echo "simple_graph analysis..."
for i in {0..29};
do
    diff "simple_graph$i$a" "simple_graph$i$b"
    diff "simple_graph$i$a" "simple_graph$i$c"
    diff "simple_graph$i$a" "simple_graph$i$e"
    diff "simple_graph$i$a" "simple_graph$i$f"
    diff "simple_graph$i$a" "simple_graph$i$g"
    diff "simple_graph$i$a" "simple_graph$i$h"
    diff "simple_graph$i$a" "simple_graph$i$l"
    diff "simple_graph$i$a" "simple_graph$i$m"
    diff "simple_graph$i$a" "simple_graph$i$n"
done

echo "medium_graph analysis..."
for i in {0..29};
do
    diff "medium_graph$i$a" "medium_graph$i$b"
    diff "medium_graph$i$a" "medium_graph$i$c"
    diff "medium_graph$i$a" "medium_graph$i$e"
    diff "medium_graph$i$a" "medium_graph$i$f"
    diff "medium_graph$i$a" "medium_graph$i$g"
    diff "medium_graph$i$a" "medium_graph$i$h"
    diff "medium_graph$i$a" "medium_graph$i$l"
    diff "medium_graph$i$a" "medium_graph$i$m"
    diff "medium_graph$i$a" "medium_graph$i$n"
done

echo "complex_graph analysis..."
for i in {0..29};
do
    diff "complex_graph$i$a" "complex_graph$i$b"
    diff "complex_graph$i$a" "complex_graph$i$c"
    diff "complex_graph$i$a" "complex_graph$i$e"
    diff "complex_graph$i$a" "complex_graph$i$f"
    diff "complex_graph$i$a" "complex_graph$i$g"
    diff "complex_graph$i$a" "complex_graph$i$h"
    diff "complex_graph$i$a" "complex_graph$i$l"
    diff "complex_graph$i$a" "complex_graph$i$m"
    diff "complex_graph$i$a" "complex_graph$i$n"
done

echo "very_complex_graph analysis..."
for i in {0..29};
do
    diff "very_complex_graph$i$a" "very_complex_graph$i$b"
    diff "very_complex_graph$i$a" "very_complex_graph$i$c"
    diff "very_complex_graph$i$a" "very_complex_graph$i$e"
    diff "very_complex_graph$i$a" "very_complex_graph$i$f"
    diff "very_complex_graph$i$a" "very_complex_graph$i$g"
    diff "very_complex_graph$i$a" "very_complex_graph$i$h"
    diff "very_complex_graph$i$a" "very_complex_graph$i$l"
    diff "very_complex_graph$i$a" "very_complex_graph$i$m"
    diff "very_complex_graph$i$a" "very_complex_graph$i$n"
done
