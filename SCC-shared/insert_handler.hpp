#ifndef INSERT_HANDLER_HPP
#define INSERT_HANDLER_HPP
#include "structure_defs.hpp"
#include <vector>

using namespace std;

bool find_path(int a, int b, metaNode **N, vector<int> junction_source[], vector<int> junction_sink[])
{
    // TODO
    cout << "finding path from a to b" << endl;
}

void handle_inserts(metaNode **N, vector<int_int> inserts_meta, vector<int> junction_source[], vector<int> junction_sink[])
{
    for (int i = 0; i < inserts_meta.size(); i++)
    {
        int a = inserts_meta[i].first;
        int b = inserts_meta[i].second;

        // case 1 if both inserts in same scc
        if (a == b)
        {
            cout << "same scc: Do nothing" << endl;
        }
        // case 2 differet SCC
        else
        {
            bool path = find_path(b, a, N, junction_source, junction_sink);

            // case A path exists
            if (path)
            {
                // TODO
                // COMBINE SCC
            }
            else
            {
                // TODO
                // UPDATE SCC CHILDREN AND PARENT INFORMATIONN
                // ALSO UPDATE THE
            }
        }
    }
}

#endif