// INPUT HEADERS
#include "translate_from_input.hpp"
#include "input_to_network.hpp"
#include "structure_defs.hpp"
#include <omp.h>
#include <algorithm>

// OUTPUT HEADERS
#include "printout_network.hpp"
#include "printout_others.hpp"

#include "SP_structure.hpp"
#include "readin_data.hpp"
#include "supporting_functions.hpp"
#include "insert_handler.hpp"
#include "deletes.hpp"
#include "Tarjan.hpp"
#include "TarjanIns.hpp"
#include "metagraph.hpp"
#include "insert.hpp"

using namespace std;

typedef pair<int, int> int_int;

// for implemntation 1 ony
/*
struct MetaNode
{
    int *source;
    int *level;
    int *child;
    int *parent;
};

// implementation 1
void print_metanode(MetaNode *m)
{
    if (m == nullptr)
    {
        cout << "NONE" << endl;
        return;
    }
    else
    {

        cout << "Sources: \n";
        printarr(m->source, 10);
        cout
            << "\nlevel: \n";
        printarr(m->level, 10);
        cout
            << "\nChild: \n";
        printarr(m->child, 10);
        cout
            << "\nParents: \n";
        printarr(m->parent, 10);
    }
}

// implementation 1
void print_metaNetwork(MetaNode **N)
{
    for (int i = 0; i < 10; i++)
    {
        cout << "MetaNode  " << i << endl;
        print_metanode(N[i]);
        cout << endl
             << endl;
    }
}


//Inside Main
    //***INITIALIZE THE ARRAYS FOR METAGRAPH******

        // int metaNodes[10] = {0};               // identifies the metanodes 1 if exists
        MetaNode *MetaNetwork[10] = {nullptr}; // Array of MetaNodes is Meta Network

        // Initialize MetaNetwork
        for (int i = 0; i < 10; i++)
        {
            if (i == SCCx[i])
            {
                MetaNode *m = new MetaNode;
                m->source = (int *)malloc(10 * sizeof(int));
                m->child = (int *)malloc(10 * sizeof(int));
                m->level = (int *)malloc(10 * sizeof(int));
                m->parent = (int *)malloc(10 * sizeof(int));
                MetaNetwork[i] = m;
            }
        }

        // Populate MetaNetwork
        for (int i = 0; i < 10; i++)
        {
            int current_scc = SCCx[i];
            int current_node = i;
            // find the parent and child of currentSCC
            for (int j = 0; j < X[current_node].In_Neigh.size(); j++)
            {
                int current_neighbor = X[current_node].In_Neigh[j];
                int current_neighbor_scc = SCCx[current_neighbor];
                if (current_scc != current_neighbor_scc)
                {
                    MetaNetwork[current_scc]->parent[current_neighbor_scc] = 1;
                }
            }

            for (int k = 0; k < X[current_node].Out_Neigh.size(); k++)
            {
                int current_neighbor = X[current_node].Out_Neigh[k];
                int current_neighbor_scc = SCCx[current_neighbor];
                if (current_scc != current_neighbor_scc)
                {
                    MetaNetwork[current_scc]->child[current_neighbor_scc] = 1;
                }
            }
        }

        print_metaNetwork(MetaNetwork);
    */

int main(int argc, char *argv[])
{
    clock_t q, t;

    // Check if valid input is given
    if (argc < 3)
    {
        cout << "INPUT ERROR:: 3 inputs required. First:  filename \n Second: file with component id \n  Third: Number of Nodes\n";
    }
    // Check to see if file opening succeeded
    ifstream the_file1(argv[1]);
    if (!the_file1.is_open())
    {
        cout << "INPUT ERROR:: Could not open  graph file\n";
    }
    ifstream the_file2(argv[2]);
    if (!the_file2.is_open())
    {
        cout << "INPUT ERROR:: Could not open SCC file\n";
    }
    int nodes = atoi(argv[3]);

    q = clock();
    D_Network X;

    D_ADJ Row;
    Row.In_Neigh.clear();
    Row.Out_Neigh.clear();
    X.resize(nodes, Row);
    readin_network(&X, argv[1]);
    // print_network(&X);
    vector<int> SCCx;
    SCCx.resize(nodes, -1); // this is to look up the ids of the nodes as per the index
    readin_SCC(&SCCx, argv[2]);
    // print_vector(SCCx);

    cout << "*****  DONE IMPLEMENTATION 1 *********" << endl;
    //  initilize metaNetwork

    vector<int> scc = SCCx;

    sort(scc.begin(), scc.end());
    scc.erase(unique(scc.begin(), scc.end()), scc.end());

    int len_scc = scc.size();
    int num_nodes = X.size();
    // cout << len_scc << "  " << num_nodes;

    metaNode *metaNetwork[nodes];

    for (int i = 0; i < nodes; i++)
    {
        if (SCCx.at(i) == i)
        {
            metaNode *m = new metaNode;
            m->id = SCCx.at(i);
            metaNetwork[SCCx.at(i)] = m;
        }
        else
        {
            metaNetwork[i] = nullptr;
        }
    }

    // populate metanetwork

    for (int m = 0; m < nodes; m++)
    {

        vector<int_int> outgoing;
        vector<int_int> incoming;
        int current_scc = SCCx[m];
        int current_node = m;

        for (int j = 0; j < X[m].Out_Neigh.size(); j++)
        {
            int current_neighbor = X[current_node].Out_Neigh[j];
            int current_neighbor_scc = SCCx[current_neighbor];
            if (current_scc != current_neighbor_scc)
            {
                int k = current_neighbor_scc;
                int v = 1;
                int_int t = make_pair(k, v);
                update_neighbors(t, &outgoing, outgoing.size());
            }
        }

        for (int k = 0; k < X[m].In_Neigh.size(); k++)
        {
            int current_neighbor = X[current_node].In_Neigh[k];
            int current_neighbor_scc = SCCx[current_neighbor];
            if (current_scc != current_neighbor_scc)
            {
                int key = current_neighbor_scc;
                int v = 1;
                int_int t = make_pair(key, v);
                update_neighbors(t, &incoming, incoming.size());
            }
        }
        // after each node calculate their vector, the private vector value must be updated with the neighbors
        // children information
        // update child of metaNode
        // cout << "fff" << metaNetwork[current_scc]->children.size() << endl;

        update_metaNode_children(outgoing, metaNetwork[current_scc]->children, outgoing.size(), metaNetwork[current_scc]->children.size());
        update_metaNode_children(incoming, metaNetwork[current_scc]->parents, incoming.size(), metaNetwork[current_scc]->parents.size());
        incoming.clear();
        outgoing.clear();
    }

    // print_metaNetwork(metaNetwork, nodes);
    vector<int> sources(scc.size(), 0);
    vector<int> sinks(scc.size(), 0);

    int count_source = 0;
    int count_sink = 0;
    int count_s = 0;
    int count_k = 0;
    for (int i = 0; i < scc.size(); i++)
    {
        int a = isSink_Source(metaNetwork, scc.at(i), 1); // check soruce
        int b = isSink_Source(metaNetwork, scc.at(i), 0); // check sink

        sources.at(i) = a;
        sinks.at(i) = b;
    }

    for (int i = 0; i < scc.size(); i++)
    {
        int a = isSink_Source(metaNetwork, scc.at(i), 1); // check source
        int b = isSink_Source(metaNetwork, scc.at(i), 0); // check sink
        sources.at(i) = a;
        sinks.at(i) = b;
        count_s = count_s + a;
        count_k = count_k + b;
    }

    int count_metanodes = 0;
    for (int i = 0; i < nodes; i++)
    {
        if (metaNetwork[i] != nullptr)
            count_metanodes++;
    }
    cout << "Total no of MetaNodes: " << count_metanodes << endl;
    cout << "Total no of Sources: " << count_s << " Sinks: " << count_k << endl;

    vector<int> S(count_s, -1);
    vector<int> K(count_k, -1);

    int pos_s = 0;
    int pos_k = 0;
    for (int i = 0; i < scc.size(); i++)
    {
        if (sources.at(i) == 1)
        {
            S.at(pos_s) = scc.at(i);
            pos_s++;
        }
        if (sinks.at(i) == 1)
        {
            K.at(pos_k) = scc.at(i);
            pos_k++;
        }
    }

    // Populate the metanode with Sink and Source Information
    for (int i = 0; i < scc.size(); i++)
    {
        vector<int_int> temp_s(count_s);
        vector<int_int> temp_k(count_k);
        for (int j = 0; j < S.size(); j++)
        {
            temp_s.at(j) = make_pair(S.at(j), -1);
        }
        for (int k = 0; k < K.size(); k++)
        {
            temp_k.at(k) = make_pair(K.at(k), -1);
        }
        metaNetwork[scc.at(i)]->source = temp_s;

        metaNetwork[scc.at(i)]->sink = temp_k;
    }

    // cout << "Sources: " << endl;
    // for (int i = 0; i < S.size(); i++)
    // {
    //     cout << S.at(i) << endl;
    // }

    // cout << "Skinks: " << endl;
    // for (int i = 0; i < K.size(); i++)
    // {
    //     cout << K.at(i) << endl;
    // }

    vector<int> junction_source[count_s];
    vector<int> junction_sink[count_k];

    for (int i = 0; i < count_s; i++)
    {
        junction_source[i] = writeLevels_source(metaNetwork, scc, scc.size(), S.at(i));
    }

    for (int i = 0; i < count_k; i++)
    {
        junction_sink[i] = writeLevels_sink(metaNetwork, scc, scc.size(), K.at(i));
    }

    print_metaNetwork(metaNetwork, nodes); // printing metannodes with sink and source information

    // Printing Junctions
    cout << "****** Printing Junctions: *****" << endl;

    for (int i = 0; i < count_s; i++)
    {
        cout << endl
             << "For source: " << S[i] << " -> ";
        for (int j = 0; j < junction_source[i].size(); j++)
        {
            cout << junction_source[i].at(j) << " ";
        }
    }

    for (int i = 0; i < count_k; i++)
    {
        cout << endl
             << "For Sink: " << K[i] << " -> ";
        for (int j = 0; j < junction_sink[i].size(); j++)
        {
            cout << junction_sink[i].at(j) << " ";
        }
        cout << endl;
    }

    // Preprocessing

    // STEP 1 Read changed EDGES
    // STEP 2 Convert changed EDGES

    // vector<int_int> inserts;
    // vector<int_int> deletes;
    // readin_changes(&inserts, &deletes, argv[3]);

    // vector<int_int> inserts_meta(inserts.size());
    // vector<int_int> deletes_meta(deletes.size());

    // inserts_meta = convert_to_meta_edge(inserts, SCCx);
    // deletes_meta = convert_to_meta_edge(deletes, SCCx);

    // cout << "Insert edges: " << endl;
    // printarr_2(&inserts, inserts.size());
    // cout << endl
    //      << "Insert metaEdges: " << endl;
    // printarr_2(&inserts_meta, inserts_meta.size());
    // cout << endl
    //      << "Delete Edges: " << endl;
    // printarr_2(&deletes, deletes.size());
    // cout << endl
    //      << "Delete metaEdges: " << endl;
    // printarr_2(&deletes_meta, deletes_meta.size());

    // handle_inserts(metaNetwork, inserts_meta, junction_source, junction_sink);

    return 0;
}
