#ifndef SUPPORTING_FUNCTIONS_HPP
#define SUPPORTING_FUNCTIONS_HPP
#include "structure_defs.hpp"
using namespace std;

void printarr(int *ptr, int l)
{

    for (int k = 0; k < l; ++k)
    {
        printf("%d", ptr[k]);
    }
}

// print the array of int_int
void printarr_2(vector<int_int> *ptr, int l)
{

    for (int i = 0; i < l; i++)
    {
        cout << ptr->at(i).first << " -> " << ptr->at(i).second << endl;
    }
}

// implementation 2
void print_metaNode(metaNode *m)
{
    if (m == nullptr)
    {
        // cout << "NONE" << endl;
        return;
    }
    else
    {
        cout << "***** MetaNode ID  " << m->id << "*****" << endl;
        cout << "Children: " << endl;
        printarr_2(&m->children, m->children.size());
        cout << "Parents: " << endl;
        printarr_2(&m->parents, m->parents.size());
        cout << "Sources: " << endl;
        printarr_2(&m->source, m->source.size());
        cout << "Sinks: " << endl;
        printarr_2(&m->sink, m->sink.size());
    }
}

// implementation 2
void print_metaNetwork(metaNode **N, int k)
{
    for (int i = 0; i < k; i++)
    {

        print_metaNode(N[i]);
    }
}

// node operation
/// update the children information as a child is encountered
void update_neighbors(int_int x, vector<int_int> *arr, int n)
{
    int key = get<0>(x);
    for (int i = 0; i < n; i++)
    {
        if (arr->at(i).first == key)
        {
            arr->at(i).second = arr->at(i).second + get<1>(x);
            return;
        }
    }
    arr->push_back(x);
}
// metaNode operation
// updates is the children information calculated by each nodes
// result is the children in metaNode which need to be updated
// x-> size of updates, y -> size of result
void update_metaNode_children(vector<int_int> &updates, vector<int_int> &result, int x, int y)
{
    for (int i = 0; i < x; i++) // for each update
    {
        int_int change = updates.at(i);
        update_neighbors(change, &result, y);
    }
}

int isSink_Source(metaNode **N, int m, int k)
{
    if (k == 1)
    {
        if (N[m]->parents.size() == 0)
        {
            return 1;
        }
    }
    else if (k == 0)
    {
        if (N[m]->children.size() == 0)
        {
            return 1;
        }
    }
    return 0;
}

// C++ program to find the index
// of an element in a vector
#include <bits/stdc++.h>

// Function to print the index of an element
int getIndex(vector<int> v, int K)
{
    auto it = find(v.begin(), v.end(), K);

    if (it != v.end())
    {
        int index = it - v.begin();
        return index;
    }
    else
    {
        return -1;
    }
}

// returns the level of metanode from source
int find_level_source(metaNode **N, int metanode, int Source)
{
    for (int i = 0; i < N[metanode]->source.size(); i++)
    {
        if (N[metanode]->source.at(i).first == Source)
        {
            return N[metanode]->source.at(i).second;
        }
    }
    return -1;
}

// returns the level of metanode from source
int find_level_sink(metaNode **N, int metanode, int Sink)
{
    for (int i = 0; i < N[metanode]->sink.size(); i++)
    {
        if (N[metanode]->sink.at(i).first == Sink)
        {
            return N[metanode]->sink.at(i).second;
        }
    }
    return -1;
}

// updates the level of metanode from the source
void update_level_source(metaNode **N, int metanode, int Source, int val)
{
    for (int i = 0; i < N[metanode]->source.size(); i++)
    {
        if (N[metanode]->source.at(i).first == Source)
        {

            N[metanode]->source.at(i).second = val;
            return;
        }
    }
    cout << "not updated";
}

// updates the level of metanode from the source
void update_level_sink(metaNode **N, int metanode, int Sink, int val)
{

    for (int i = 0; i < N[metanode]->sink.size(); i++)
    {
        if (N[metanode]->sink.at(i).first == Sink)
        {

            N[metanode]->sink.at(i).second = val;
            return;
        }
    }
    cout << "not updated";
}

// N-> metaNetwork
//  scc-> list of SCC Ids;
//  int V -> number of SCCs => scc.size()
// int x -> source/sink ID
// bool top -> true if levels from source, false if levels from sink
vector<int> writeLevels_source(metaNode **N, vector<int> scc, int V, int x)
{
    vector<int> junctions;
    int start = x;
    bool marked[V]; // mark all false for visited
    cout << "For Source " << x << endl;
    for (int i = 0; i < V; i++)
    {
        marked[i] = false;
    }
    queue<int> que; // queue for BFS
    que.push(x);    // push source ID to start BFS

    // set level = 0 for the source Node

    update_level_source(N, x, start, 0);
    int index = getIndex(scc, x);

    marked[index] = true;

    while (!que.empty())
    {

        x = que.front();
        que.pop();
        // int level_parent = x -> from source;
        for (int j = 0; j < N[x]->children.size(); j++)
        {

            int b = N[x]->children.at(j).first;

            // check if b was already visited
            int b_index = getIndex(scc, b);

            if (marked[b_index] == true)
            {
                // already visited
                cout << "Junction Node" << b << endl;
                junctions.push_back(b);

                for (int i = 0; i < N[b]->source.size(); i++)
                {
                    if (N[b]->source.at(i).first == start)
                    {
                        N[b]->source.at(i).second = max(N[b]->source.at(i).second, find_level_source(N, x, start) + 1);
                    }
                }
            }
            else
            {
                que.push(b);
                update_level_source(N, b, start, find_level_source(N, x, start) + 1);
                marked[b_index] = true;
            }
        }
    }
    return junctions;
}

vector<int> writeLevels_sink(metaNode **N, vector<int> scc, int V, int x)
{
    vector<int> junctions;
    int start = x;
    bool marked[V]; // mark all false for visited
    for (int i = 0; i < V; i++)
    {
        marked[i] = false;
    }
    queue<int> que; // queue for BFS
    que.push(x);    // push source ID to start BFS

    // set level = 0 for the source Node

    update_level_sink(N, x, start, 0);

    int index = getIndex(scc, x);

    marked[index] = true;

    while (!que.empty())
    {

        x = que.front();
        que.pop();

        for (int j = 0; j < N[x]->parents.size(); j++)
        {

            int b = N[x]->parents.at(j).first;

            // check if b was already visited
            int b_index = getIndex(scc, b);

            if (marked[b_index] == true)
            {
                // already visited
                // cout << "Junction Node" << b << endl;
                junctions.push_back(b);
                for (int i = 0; i < N[b]->sink.size(); i++)
                {
                    if (N[b]->sink.at(i).first == start)
                    {
                        N[b]->sink.at(i).second = max(N[b]->sink.at(i).second, find_level_sink(N, x, start) + 1);
                    }
                }
            }
            else
            {
                que.push(b);
                update_level_sink(N, b, start, find_level_sink(N, x, start) + 1);
                marked[b_index] = true;
            }
        }
    }
    return junctions;
}

vector<int_int> convert_to_meta_edge(vector<int_int> change_edges, vector<int> SCCx)
{
    vector<int_int> change_meta_edges(change_edges.size());
    for (int i = 0; i < change_edges.size(); i++)
    {
        int a = change_edges.at(i).first;
        int b = change_edges.at(i).second;
        change_meta_edges.at(i) = make_pair(SCCx.at(a), SCCx.at(b));
    }
    return change_meta_edges;
}

#endif
