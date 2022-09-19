


#ifndef insert_hpp
#define insert_hpp



void find_valid_inserts( D_Network *M, vector<int> *SCCx, vector<int_int> *inserts, vector<bool> *valid_inserts, vector<bool> *insert_nodes, int p)
{
    
#pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<inserts->size();i++)
    {
        
        int src=inserts->at(i).first;
        int dest=inserts->at(i).second;
        
        //Find the SCC_IDs for the meta graph
        int src_id=SCCx->at(src);
        int dest_id=SCCx->at(dest);

      //  printf("%d %d:: %d %d \n", src, dest, src_id, dest_id);
    
        //If they are in the same SCC then continue
        if(src_id!=dest_id)
        {
	      valid_inserts->at(i)=true;
            
            //Not valid if src-->dest exists
	for(int x=0;x<M->at(src_id).In_Neigh.size();x++)
	{
        if(M->at(src_id).In_Neigh[x]==dest_id)
		{valid_inserts->at(i)=false; break;}
	}
            
       //Not valid if src is source node (without any Out degree)
       if(M->at(src_id).Out_Neigh.size()==0)
       {valid_inserts->at(i)=false;}
            
    //Not valid if dest is sink node (without anyIndegree degree)
    if(M->at(dest_id).In_Neigh.size()==0)
            {valid_inserts->at(i)=false;}
            
            if(valid_inserts->at(i)==true)
            {
                insert_nodes->at(src_id)=true;
                insert_nodes->at(dest_id)=true;
            }
	}//end of for
        
}//eend of for

}
/** End of Function ***/


void fork_numbering(D_Network *M, vector<bool> *insert_nodes, vector< vector<int> > *fork, vector< vector<int> > *branch)
{
    bool change=true;
    while(change)
    {
        change=false;
#pragma omp parallel for schedule(dynamic) num_threads(p)
        for(int i=0;i<insert_nodes->size();i++)
        {
            if(insert_nodes->at(i)==false){continue;}
             printf("%d--  %d\n",i, fork->at(i).size());
            //This node has been processed
            if(fork->at(i).size()>0) {insert_nodes->at(i)=false; continue;}
            
            change=true;
            int node=i;
             insert_nodes->at(node)=false; //set to false to indicate it is processed
            
            //If node is source;
            if(M->at(node).Out_Neigh.size()==0)
            {
                fork->at(node).push_back(node);
                branch->at(node).push_back(node);
            }
           else
           {
            //Else process the node
            for(int j=0;j<M->at(node).Out_Neigh.size();j++)
            {
                int mybranch=node;
                int myfork=M->at(node).Out_Neigh[j];
                
               
                while(M->at(myfork).Out_Neigh.size()==1)
                {
                   M->at(myfork).Out_Neigh.size();
                     mybranch=myfork;
                    myfork=M->at(myfork).Out_Neigh[0];
                    
                }
                fork->at(node).push_back(myfork);
                branch->at(node).push_back(mybranch);
                
                //set this to be processed
                if(fork->at(myfork).size()==0)
                {insert_nodes->at(myfork)=true;}
                
            }//end of for j
               
           }//end of else
            
        }//end of for i
        
    }//end of while
}
/** End of Function ***/



    /*
void fork_numbering(D_Network *M, vector<bool> *insert_nodes, vector< vector<int> > *fork, vector< vector<int> > *branch)
{
    bool change=true;
    while(change)
    {
        change=false;
#pragma omp parallel for schedule(dynamic) num_threads(p)
        for(int i=0;i<insert_nodes->size();i++)
        {
            if(insert_nodes->at(i)==false){continue;}
            printf("%d--  %d\n",i, fork->at(i).size());
            //This node has been processed
            if(fork->at(i).size()>0) {insert_nodes->at(i)=false; continue;}
            
            change=true;
            int node=i;
            insert_nodes->at(node)=false; //set to false to indicate it is processed
            
            //If node is source;
            if(M->at(node).Out_Neigh.size()==0)
            {
                fork->at(node).push_back(node);
                branch->at(node).push_back(node);
            }
            else
            {
                //Else process the node
                for(int j=0;j<M->at(node).Out_Neigh.size();j++)
                {
                    int mybranch=node;
                    int myfork=M->at(node).Out_Neigh[j];
                    
                    
                    while(M->at(myfork).Out_Neigh.size()==1)
                    {
                        M->at(myfork).Out_Neigh.size();
                        mybranch=myfork;
                        myfork=M->at(myfork).Out_Neigh[0];
                        
                    }
                    fork->at(node).push_back(myfork);
                    branch->at(node).push_back(mybranch);
                    
                    //set this to be processed
                    if(fork->at(myfork).size()==0)
                    {insert_nodes->at(myfork)=true;}
                    
                }//end of for j
                
            }//end of else
            
        }//end of for i
        
    }//end of while
}
/** End of Function ***/


/*
void connect_SCCs(A_Network *M, vector<int> *SCCx,vector<int> *newSCC, vector<int_int> *inserts, vector<bool> *valid_inserts, vector< vector<int> > *fork, vector< vector<int> >*branch,int p)
{
  
#pragma omp parallel for schedule(dynamic) num_threads(p)
    for(int i=0;i<inserts->size();i++)
    {
        //Continue if inserts are not valid
        if(valid_inserts->at(i)==false){continue;}
        
        
        int src=inserts->at(i).first;
        int dest=inserts->at(i).second;
        
        //Find the SCC_IDs for the meta graph
        int src_id=SCCx->at(src);
        int dest_id=SCCx->at(dest);
        
        printf("%d  %d \n",src_id, dest_id);
        
        //If the edge is valid then one of the upstream forks from destination should be the same as the frks of src
        
        //List all the branches until common frk reached
        vector<int> listfrks;
        listfrks.clear();
        
         listfrks=
        int st_index=0;
        int end_index=listfrks.size();
       
        
        int src_frk=fork->at(src_id)[index];
        int dest_frk=fork->at(dest_id)[index];
        int src_branch=branch->at(src_id)[index];
        int dest_branch=branch->at(dest_id)[index];
        int next_src_frk=-1;
        int next_src_branch=-1;
        
        {listfrks.push_back(src_branch);}
        
        while(src_frk!=dest_frk) //has to be equal at some point since earliest fork is source (root)
        {
            next_src_frk=fork->at(src_frk)[index];
            next_src_branch=branch->at(src_frk)[index];
            src_frk=next_src_frk;
            src_branch=next_src_branch;
            listfrks.push_back(src_branch);
        }//end of while
       // listfrks.push_back(dest_id);
        //print_vector(listfrks);

        int b; //from where to read listfrks
        if((src_frk==dest_id) &&(dest_id!=dest_branch)) //if destination is a fork, but not the root include final branch
        { b=listfrks.size()-1;}
        else //dont include it
         { b=listfrks.size()-2;}
        
        
        //traverse move to the correct branch
        vector<int> visited_nodes;
        visited_nodes.clear();
        int cur_node=dest_id;
        int next_node;
        visited_nodes.push_back(dest_id);
       
        
        while(cur_node!=src_id)
        {
            //If single path continue;
            while(M->at(cur_node).ListW.size()==1)
            {
                next_node=M->at(cur_node).ListW[0].first;
                visited_nodes.push_back(next_node);
                cur_node=next_node;
            }
            
            //Now arrive at fork
            for(int j=0;j<M->at(cur_node).ListW.size();j++)
            {
                if(M->at(cur_node).ListW[j].first==listfrks[b])
                {
                    next_node=M->at(cur_node).ListW[j].first;
                    visited_nodes.push_back(next_node);
                    cur_node=next_node;
                    b=b-1;
                }
            }
        }//end of while
        
       // print_vector(visited_nodes);
        
        //get minimum node id from visited
        int min_id=*min_element(visited_nodes.begin(), visited_nodes.end());
        
        int myvisit=1;
        for(int k=0;k<visited_nodes.size();k++)
        {
            if(newSCC->at(visited_nodes[k])>min_id)
            {
                {newSCC->at(visited_nodes[k])=min_id;}
                myvisit++;
                           // change=true;
            }
        }//end of for
        
        //If all the nodes updated, then this path no longer needed
        if(myvisit==visited_nodes.size())
          {valid_inserts->at(i)=-1;}
       // printf("%d ...==\n",myvisit);
    
    }//end of for
                            
    
    
    //Update the SCC to the lowest value iteratively
    bool change=true;
    while(change)
    {
    change=false;
        
#pragma omp parallel for schedule(dynamic) num_threads(p)
        for(int i=0;i<newSCC->size();i++)
        {
            int id=newSCC->at(i);
            
            if(newSCC->at(id)<id)
            {newSCC->at(i)=newSCC->at(id);
                change=true;}
            
        }//end of for
        
        
    }//end of while
    
    return;
}
 */
/*** End of Function ***/

#endif /* insert_across_h */
