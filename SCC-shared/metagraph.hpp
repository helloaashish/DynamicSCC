


#ifndef metagraph_hpp
#define metagraph_hpp

#include "printout_others.hpp"
#include "translate_from_input.hpp"
#include "input_to_network.hpp"
#include"ADJ/create_network.hpp"


void create_metagraph(D_Network *X,D_Network *M, vector<int> *SCCx, vector<bool> *is_connector, int p)
{
  
    //Get maxSCC
    int maxID=*max_element(SCCx->begin(),SCCx->end());
    maxID=maxID+1; //One more to account for 0
    D_ADJ Row;
    Row.In_Neigh.clear();
    Row.Out_Neigh.clear();
    M->resize(maxID, Row);
    
    
#pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<maxID;i++)
    {
        for(int j=0;j<X->size();j++)
        {
            //Only process for connectors
            if(is_connector->at(j)==false) {continue;}
            
            //Only process for nodes with same SCC Id as  i
            if(SCCx->at(j)!=i) {continue;}
            
            int node=j;
            int node_id=SCCx->at(node);
          //go through neighbors of node
            
            //In_degree
        for(int k=0;k<X->at(node).In_Neigh.size();k++)
           {
               int neigh=X->at(node).In_Neigh[k];
               //If deleted then continue;
               if(neigh==-1){continue;}
               
            //get SCC ID of neighbors
            int neigh_id=SCCx->at(neigh);
            if(node_id!=neigh_id) //Avoid self loops
            { M->at(node_id).In_Neigh.push_back(neigh_id);}
        }//end of k
            
            //Out_degree
            for(int k=0;k<X->at(node).Out_Neigh.size();k++)
            {
                int neigh=X->at(node).Out_Neigh[k];
                //If deleted then continue;
                if(neigh==-1){continue;}
                //get SCC ID of neighbors
                int neigh_id=SCCx->at(neigh);
                if(node_id!=neigh_id) //Avoid self loops
                { M->at(node_id).Out_Neigh.push_back(neigh_id);}
            }//end of k
            
        }//end of for j
        
    }//end of for i
    
    //Sort the neighbors of M
#pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<maxID;i++)
    {
        vector<int> temp;
        
        temp.clear();
        temp=M->at(i).In_Neigh;
        sort(&temp);
        M->at(i).In_Neigh=unique(temp);
        
        temp.clear();
        temp=M->at(i).Out_Neigh;
        sort(&temp);
        M->at(i).Out_Neigh=unique(temp);
    }
    
    return;
}
/** End of Function ****/

void add_edges_to_metagraph(D_Network *M, vector<int>* SCCx, vector<int_int> *inserts, vector<bool> *valid_inserts, vector<bool> *insert_nodes, int p)
{
    
#pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<M->size();i++)
    {
        //Not part of insert_nodes so new edges will be added
        if(insert_nodes->at(i)==false) {continue;}
        
        for(int j=0;j<inserts->size();j++)
        {
            
            if(valid_inserts->at(j)==false){continue;}
            
            int src=inserts->at(j).first;
            int dest=inserts->at(j).second;
            
            //Find the SCC_IDs for the meta graph
            int src_id=SCCx->at(src);
            int dest_id=SCCx->at(dest);
            
            
            if(src_id==i)
            {M->at(src_id).In_Neigh.push_back(dest_id);}
            
            if(dest_id==i)
            {M->at(dest_id).Out_Neigh.push_back(src_id);}
            
            } //end of for j
                
            
        }//end of for i
    
    return;
}


//Triming the sources and sinks in M
bool trim(D_Network *M, vector<bool> *trimmed, int p)
{
    bool change=false;
    #pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<M->size();i++)
    {
        
        
        //If already trimmed ---continue;
        if(trimmed->at(i)==true){continue;}
        
        //Process for sink
        queue<int> myQ;
        myQ.push(i);
        while(!myQ.empty())
        {
             bool is_trim=true;
            int node=myQ.front();
            myQ.pop();
            
        for(int j=0;j<M->at(node).Out_Neigh.size();j++)
        {
            int neigh=M->at(node).Out_Neigh[j];
            //If even one neighbor is valid
            if((neigh>-1) &&(trimmed->at(neigh)==false))
            {is_trim=false; break;}
        }
            if(is_trim==true)
            { trimmed->at(node)=true;
                
                change=true;
                
                
                for(int j=0;j<M->at(node).In_Neigh.size();j++)
                {
                    int neigh=M->at(node).In_Neigh[j];
                    if((neigh>-1) &&(trimmed->at(neigh)==false))
                        myQ.push(M->at(node).In_Neigh[j]);
                }
            }//end of if
        }//end of while
        
        if(trimmed->at(i)==true){continue;}
        
        
        //Process for source
        queue<int> myQx;
        myQx.push(i);
        while(!myQx.empty())
        {
            bool is_trim=true;
            int node=myQx.front();
            myQx.pop();
            
            for(int j=0;j<M->at(node).In_Neigh.size();j++)
            {
                int neigh=M->at(node).In_Neigh[j];
                
                //If even one neighbor is valid
                if((neigh>-1) &&(trimmed->at(neigh)==false))
                {is_trim=false; break;}
            }
            if(is_trim==true)
            { trimmed->at(node)=true;
                change=true;
                
                for(int j=0;j<M->at(node).Out_Neigh.size();j++)
                {
                    int neigh=M->at(node).Out_Neigh[j];
                     if((neigh>-1) && (trimmed->at(neigh)==false))
                        myQx.push(M->at(node).Out_Neigh[j]);
                        }
            }//end of if
        }//end of while
        
    }//end of for
    
    return change;
}
/** End of Function ****/





#endif
