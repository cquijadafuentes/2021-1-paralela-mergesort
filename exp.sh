#!/bin/bash

FILE=${1}
REP=${2}

echo -e "#N\tmergesort\tmergesort4bloques\tparallel_mergesort\tparallel_mergesort4bloques" > ${FILE}
for i in 10000000 20000000 30000000 40000000 50000000 60000000 70000000 80000000 90000000 100000000; do
	echo -ne "${i}\t" >> ${FILE}
	./mergesort ${i} ${2} >> ${FILE}
	./mergesort4bloques ${i} ${2} >> ${FILE}
	./parallel_mergesort ${i} ${2} >> ${FILE}
	./parallel_mergesort4bloques ${i} ${2} >> ${FILE}
	echo "" >> ${FILE}
done

echo "Fin."
