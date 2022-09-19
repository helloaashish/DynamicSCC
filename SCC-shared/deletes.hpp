//
//  delete_within.hpp
//  
//
//  Created by Bhowmick, Sanjukta on 8/18/20.
//

#ifndef deletes_hpp
#define deletes_hpp

//Check if there is a path from n1 to its SCC

 void find_valid_deletes(D_Network *X, vector<int> *SCCx, vector<int_int> *deletes, vector<bool> *delete_nodes, vector<bool> *is_connect, int p)
{
    
#pragma omp parallel for num_threads(p) schedule(static)
    for(int i=0;i<deletes->size();i++)
    {
        //mark edges as -1
        int n1=deletes->at(i).first;
        int n2=deletes->at(i).second;
        
        //First delete from network
        //n1-->n2
        for(int j=0;j<X->at(n1).In_Neigh.size();j++)
        {
            int n=X->at(n1).In_Neigh[j];
            if(n==n2)
            {X->at(n1).In_Neigh[j]=-1;}
        }//end of for
        
        //n2 to n1
        for(int j=0;j<X->at(n2).Out_Neigh.size();j++)
        {
            int n=X->at(n2).Out_Neigh[j];
            if(n==n1)
            {X->at(n2).Out_Neigh[j]=-1;}
        }//end of for
        
        //Check if delete affects the SCC
        //If the end points have same SCC then they are potential candidates for deletion
        //We only need
        if(SCCx->at(n1)== SCCx->at(n2))
        {delete_nodes->at(n1)=true;
         delete_nodes->at(n2)=true;}
    
    }//end of for

    //Find whether  deletes are valid based on paths
    //Do in two stages to ensure all -1 are marked.
    //Check whether src(n1) can reach its SCC through any other neighbors
#pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int i=0;i<delete_nodes->size();i++)
    {
        //If not marked as candidate for deletion move
        if(delete_nodes->at(i)==false) {continue;}
        
        int n=i;
        bool found=false;
        
        //Check if n can still reach its own SCC and can be reached from its SCC
        
        //n->SCC
        bool found1=false;
        for(int k=0;k<X->at(n).In_Neigh.size();k++)
        {
            //ignore if edge has been deleted
            if(X->at(n).In_Neigh[k]==-1) {continue;}
            int neigh=X->at(n).In_Neigh[k];
            
             if(SCCx->at(n)== SCCx->at(neigh))
             {found1=true; break;}
        }//end of for
        
        //SCC->n
        bool found2=false;
        for(int k=0;k<X->at(n).Out_Neigh.size();k++)
        {
            //ignore if edge has been deleted
            if(X->at(n).Out_Neigh[k]==-1) {continue;}
            int neigh=X->at(n).Out_Neigh[k];
            
            if(SCCx->at(n)== SCCx->at(neigh))
            {found2=true; break;}
        }//end of for

        //printf("%d  %d %d \n",n,found1, found2);
        //If bothways found then this node is still in its SCC
        if(found1==true && found2==true)
        {delete_nodes->at(n)=false;}
        
     }//end of for
  
    //If any of the delete_nodes is also the same as SCC_ID then its connectors must be kept
    //Otherwise break will not be seen
#pragma omp parallel for num_threads(p) schedule(static)
    for(int i=0;i<deletes->size();i++)
    {
        //mark edges as -1
        int n1=deletes->at(i).first;
        int n2=deletes->at(i).second;
        
        //SCC_ID of n1 is same as node and it is marked as valid
        if(n1==SCCx->at(n1) && delete_nodes->at(n1)==true)
        {delete_nodes->at(n2)=true;}
        
        //SCC_ID of n2 is same as node and it is marked as valid
        if(n1==SCCx->at(n2) && delete_nodes->at(n2)==true)
        {delete_nodes->at(n1)=true;}
        
    }
    
    //Mark all valid nodes as potential connectors
#pragma omp parallel for num_threads(p) schedule(static)
    for(int i=0;i<delete_nodes->size();i++)
    {
        if(delete_nodes->at(i)==true)
        {is_connect->at(i)=true;}
    }
    
    
    
    return;
}
/*** End of Function ***/

#endif /* delete_within_h */

