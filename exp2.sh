#!/bin/bash

FILE=${1}
REP=${2}
N="50000000"

echo -e "#Cores\tparallel_mergesort\tparallel_mergesort4bloques" > ${FILE}
for i in 1 2 4 8 16 32; do
	echo -ne "${i}\t" >> ${FILE}
	./parallel_2_mergesort ${N} ${i} ${REP} >> ${FILE}
	./parallel_2_mergesort4bloques ${N} ${i} ${REP} >> ${FILE}
	echo "" >> ${FILE}
done

echo "Fin."
