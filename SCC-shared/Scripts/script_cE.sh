#!/bin/bash
## declare an array variable
declare -a no_changed_edges=(1000000 2000000 5000000 10000000)
declare -a size_names=("1M" "2M" "5M" "10M")
declare -a percent_insertion=(0 25 50 75 100)


# get length of an array
len_a1=${#no_changed_edges[@]}
len_a2=${#percent_insertion[@]}

# use for loop to read all values and indexes
for (( i=0; i<${len_a1}; i++ ));
do
	for (( j=0; j<${len_a2}; j++));
	do
		./cE-undirected /home/users/apandey/Datasets_SCC/Datasets/${1}_1 $2 ${no_changed_edges[$i]} ${percent_insertion[$j]} > ${1}_ce/${1}_${size_names[$i]}_${percent_insertion[$j]}
	done
  #echo "index: $i, value: ${array[$i]}"
done

#./op_cePR p2p-Gnutella05.txt 8846 10000 50 25 0 > lowOutDegreeCE/CE_p2p-Gnutella05_10k_50_25
#./op_cePR p2p-Gnutella05.txt 8846 20000 50 25 0 > lowOutDegreeCE/CE_p2p-Gnutella05_20k_50_25
#./op_cePR p2p-Gnutella05.txt 8846 30000 50 25 0 > lowOutDegreeCE/CE_p2p-Gnutella05_30k_50_25

