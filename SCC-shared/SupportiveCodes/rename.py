#python script to rename edgelist and write in file
# usage python3 rename.py original_graph_name
#output
'''
n m
1 2
3 4
'''
import networkx as nx
import sys

# Renames the graph file to make it 0-vertexed and continuing name of nodes
filename = sys.argv[1]
data = open(filename,"r")
graph = nx.parse_edgelist(data,delimiter = ' ', create_using=nx.DiGraph(), nodetype = int)
reindexed_graph = nx.relabel.convert_node_labels_to_integers(graph, first_label=0, ordering='default')
n = graph.number_of_nodes()
m = graph.number_of_edges()
with open(filename+"_renamed", 'w') as f:
    f.write(str(n)+" "+str(m)+"\n")
    for a,b in reindexed_graph.edges:
        f.write(str(a)+" "+str(b)+"\n")
#nx.write_edgelist(reindexed_graph, filename+"_renamed",data = False)
