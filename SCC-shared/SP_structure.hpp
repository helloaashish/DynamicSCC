#ifndef SP_STRUCTURE_HPP
#define SP_STRUCTURE_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <fstream>
#include <cmath>
#include <ctime>
#include <queue>
#include <stack>
#include <map>
#include <list>
#include <omp.h>

// ESSENS functions
#include "unary_set_operations.hpp"
//#include <concurrent_vector.h>
using namespace std;

// Properties of the Network having the SCC
// Determines links of a vertex
struct D_ADJ
{
    vector<int> In_Neigh;  // indegree neighbors
    vector<int> Out_Neigh; // outdegree neighbors

    // Constructor
    D_ADJ()
    {
        In_Neigh.clear();
        Out_Neigh.clear();
    }

    // Destructor
    void clear()
    {
        while (!In_Neigh.empty())
        {
            In_Neigh.pop_back();
        }
        while (!Out_Neigh.empty())
        {
            Out_Neigh.pop_back();
        }
    }
};
typedef vector<D_ADJ> D_Network;

struct metaNode
{
    int id;
    vector<int_int> children; // children of meta_node with no of edges connecting
    vector<int_int> parents;  // parents of meta_node with no of edge connecting
    vector<int_int> source;   // sources of metanode with level number from the source
    vector<int_int> sink;
    // destructor
    void clear()
    {
        while (!children.empty())
        {
            children.pop_back();
        }
        while (!parents.empty())
        {
            parents.pop_back();
        }
        while (!source.empty())
        {
            source.pop_back();
        }
        while (!sink.empty())
        {
            sink.pop_back();
        }
    }
};

/*** End of Function ***/

struct D_ADJ2
{
    vector<int_int> In_Neigh;  // indegree neighbors
    vector<int_int> Out_Neigh; // outdegree neighbors

    // Constructor
    D_ADJ2()
    {
        In_Neigh.clear();
        Out_Neigh.clear();
    }

    // Destructor
    void clear()
    {
        while (!In_Neigh.empty())
        {
            In_Neigh.pop_back();
        }
        while (!Out_Neigh.empty())
        {
            Out_Neigh.pop_back();
        }
    }
};
typedef vector<D_ADJ2> D2_Network;

// Printing Network
void print_network(D_Network *X)
{
    for (int i = 0; i < X->size(); i++)
    {
        printf("Node: %d \n", i);

        for (int j = 0; j < X->at(i).In_Neigh.size(); j++)
            printf("--> %d \n", X->at(i).In_Neigh[j]);

        printf(".....\n");

        for (int j = 0; j < X->at(i).Out_Neigh.size(); j++)
            printf("%d <-- \n", X->at(i).Out_Neigh[j]);
    }
}

/*** End of Function ***/

#endif
