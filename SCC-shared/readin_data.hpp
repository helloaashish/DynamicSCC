#ifndef READIN_DATA_HPP
#define READIN_DATA_HPP

using namespace std;

void readin_network(D_Network *X, char *file)
{

    // File reading parameters
    FILE *graph_file;
    char line[128];

    graph_file = fopen(file, "r");

    while (fgets(line, 128, graph_file) != NULL)
    {
        int n1, n2, wt;
        // Read line
        sscanf(line, "%d %d %d", &n1, &n2, &wt);
        X->at(n1).Out_Neigh.push_back(n2);
        X->at(n2).In_Neigh.push_back(n1);

    } // end of while
    fclose(graph_file);

    return;
}

// Read in the SCC Ids
void readin_SCC(vector<int> *SCCx, char *file)
{
    // File reading parameters
    FILE *graph_file;
    char line[128];

    graph_file = fopen(file, "r");
    int ID;
    int l = 0;

    while (fgets(line, 128, graph_file) != NULL)
    {
        // Read line
        sscanf(line, "%d ", &ID);
        SCCx->at(l) = ID;

        l++;

    } // end of while

    return;
}

// Information about set of changed edges
void readin_changes(vector<int_int> *inserts, vector<int_int> *deletes, char *myfile)
{
    // File reading parameters
    FILE *graph_file;
    char line[128];
    int type;
    int_int ID_edge;

    graph_file = fopen(myfile, "r");
    while (fgets(line, 128, graph_file) != NULL)
    {
        sscanf(line, "%d  %d %d", &ID_edge.first, &ID_edge.second, &type);

        if (type == 0) // Delete from X
        {
            deletes->push_back(ID_edge);
        }

        if (type == 1) // Insert to X
        {
            inserts->push_back(ID_edge);
        }

    } // end of while

    fclose(graph_file);
}
/*** End of Function ***/

#endif
