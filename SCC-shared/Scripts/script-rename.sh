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
		python3 /home/users/apandey/Bhowmick_SCC/SCC-shared/SupportiveCodes/rename.py /home/users/apandey/Datasets_SCC/UpdatedGraphs/${1}_updated/${1}_${size_names[$i]}_${percent_insertion[$j]}.edges
        rm /home/users/apandey/Datasets_SCC/UpdatedGraphs/${1}_updated/${1}_${size_names[$i]}_${percent_insertion[$j]}.edges
        #.//home/users/apandey/multistep/scc /home/users/apandey/Datasets_SCC/UpdatedGraphs/${1}_updated/${1}_${size_names[$i]}_${percent_insertion[$j]}.edges_renamed
	done
  #echo "index: $i, value: ${array[$i]}"
done
