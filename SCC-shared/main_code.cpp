
//INPUT HEADERS
#include "translate_from_input.hpp"
#include "input_to_network.hpp"
#include"structure_defs.hpp"
#include <omp.h>

//OUTPUT HEADERS
#include "printout_network.hpp"
#include "printout_others.hpp"


#include "SP_structure.hpp"
#include "readin_data.hpp"
#include "deletes.hpp"
#include "Tarjan.hpp"
#include "TarjanIns.hpp"
#include "metagraph.hpp"
#include "insert.hpp"


using namespace std;

int main(int argc, char *argv[])
{       
	clock_t q, t;
    
    
    //Check if valid input is given
    if ( argc < 3) { cout << "INPUT ERROR:: 5 inputs required. First:  filename \n Second: file with component id \n  Third: Set of changed edges \n Fourth: Number of Nodes \n Fifth: Number of Threads \n";}
    //Check to see if file opening succeeded
     ifstream the_file1 ( argv[1] ); if (!the_file1.is_open() ) { cout<<"INPUT ERROR:: Could not open  graph file\n";}
      ifstream the_file2 ( argv[2] ); if (!the_file2.is_open() ) { cout<<"INPUT ERROR:: Could not open SCC file\n";}
     ifstream the_file3 ( argv[3] ); if (!the_file3.is_open() ) { cout<<"INPUT ERROR:: Could not open changed file\n";}
    // ifstream the_file4 ( argv[4] ); if (!the_file3.is_open() ) { cout<<"INPUT ERROR:: Could not openchanged  file\n";}
    int nodes=atoi(argv[4]);
    int p = atoi(argv[5]);  //total number of threads per core
  
    //omp_set_num_threads(p);
   //===Read in Network to the Processors
    //Back edges are not explicitly stored--we will convert as needed
    q=clock();
    D_Network X;
    
    D_ADJ Row;
    Row.In_Neigh.clear();
    Row.Out_Neigh.clear();
    X.resize(nodes, Row);
    readin_network(&X,argv[1]);
    print_network(&X); 
    vector<int> SCCx;
    SCCx.resize(nodes,-1); //this is to look up the ids of the nodes as per the index
    readin_SCC(&SCCx, argv[2]);
    print_vector(SCCx);
    
   
   //===Read in changed edges
    //Reads the set of changed edges and then keeps the ones with at least one neighbor in its processor
    vector <int_int> inserts;
    inserts.clear();
    vector<int_int> deletes;
    deletes.clear();
    
    //We assume that the inserts and deletes are sorted
    readin_changes(&inserts, &deletes, argv[3]);
    q=clock()-q;
    printf("Total Time for Reading  %f \n", ((float)q)/CLOCKS_PER_SEC);
    //========= Reading Done ===//
   print_vector(inserts);
   print_vector(deletes);   
 
    //==PreProcessing===//
    /*
    q=clock();
    vector<bool> is_connect;
    is_connect.resize(X.size(),false);
    #pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<X.size();i++)
    {
            int node_id=SCCx[i];
        for(int k=0;k<X[i].In_Neigh.size();k++)
        {
        int neigh=X[i].In_Neigh[k];
        int neigh_id=SCCx[neigh];
            
            if(node_id!=neigh_id)
             {is_connect[i]=true; break;}
        }
        
        if(is_connect[i]){continue;} //already marked as connector
        
        for(int k=0;k<X[i].Out_Neigh.size();k++)
        {
            int neigh=X[i].Out_Neigh[k];
            int neigh_id=SCCx[neigh];
            
            if(node_id!=neigh_id)
            {is_connect[i]=true; break;}
        }
        
    }//end of for
    
    q=clock()-q;
    printf("Total Time for Preprocessing  %f \n", ((float)q)/CLOCKS_PER_SEC);
    //========= Create Meta network Done ===//
   // print_network(&X);
    // print_vector(SCCx);
    
    t=clock();
    //==Apply Deletes ===//
    clock_t q3;
    q3=clock();
    
    vector<bool> mynodes; //whether these vertices are associated with valid deletes/inserts
    mynodes.resize(X.size(),false);
    //Find valid deletes
   find_valid_deletes(&X, &SCCx, &deletes,&mynodes,&is_connect,p);
    
   // print_vector(mynodes);
    
    //Obtain new SCCs by running Tarjan from multiple sources
   // if(delete_nodes.size()>1)
    multiple_tarjan(&X, &SCCx, &mynodes, &is_connect,p);
    mynodes.clear();
    q3=clock()-q3;
    printf("Total Time for Deletion %f \n", ((float)q3)/CLOCKS_PER_SEC);
 //   print_vector(SCC_ID);
  //print_vector(SCCx);
    
    //==Apply Deletes Done ===//
    
    //printf("Apply deletes doen \n");
    //==Create Metagraph and Fork Based Numbering ===//
    //Create meta graph of unique SCC_IDs
    
    //print_vector(is_connect);
    //print_vector(SCCx);
   clock_t q2;
      q2=clock();
    D_Network M;
    create_metagraph(&X,&M,&SCCx,&is_connect,p);
  X.clear();
    q2=clock()-q2;
    printf("Total Time for Metagraph Creation %f \n", ((float)q2)/CLOCKS_PER_SEC);
     //==Create Metagraph and Fork Based Numbering Done ===//

    //==Apply Inserts ===//
    clock_t q1;
    q1=clock();
    vector<bool> valid_inserts;
    valid_inserts.resize(inserts.size(),false);
      mynodes.resize(M.size(),false);
    find_valid_inserts(&M,&SCCx, &inserts, &valid_inserts, &mynodes,p);
  // print_vector(mynodes);
    
    add_edges_to_metagraph(&M,&SCCx, &inserts, &valid_inserts, &mynodes,p);
   // print_network(&M);
    
    //Now keep on trimming and checking for valid
    vector<bool> trimmed;
    trimmed.resize(M.size(),false);
    trim(&M,&trimmed,p); //trim sources and sinks, etc.
  //  print_vector(trimmed);
    
    multiple_tarjan_ins(&M,&SCCx,&mynodes, &trimmed, p );
   // print_vector(SCCx);
 
    
    q1=clock()-q1;
    printf("Total Time for Insert  %f \n", ((float)q1)/CLOCKS_PER_SEC);
    //==Apply Inserts Done===//
    
    t=clock()-t;
    printf("Total Time for Update  %f \n", ((float)t)/CLOCKS_PER_SEC);
    */
    //print_vector(newSCC);

	return 0;
}//end of main
	
