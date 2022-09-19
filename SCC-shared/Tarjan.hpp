//
//  Tarjan.hpp
//  
//
//  Created by Sanjukta Bhowmick on 4/4/19.
//Modified from https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
//

#ifndef Tarjan_h
#define Tarjan_h

void tarjan(int node, D_Network *X, vector<int> *SCCx, vector<int> *newSCC_ID, vector<int> *disc, vector<int> *low, vector<bool> *stackmember, stack<int>* st, int *iter )
{
 
   #pragma omp atomic
    {*iter=*iter+1;}
    
    disc->at(node)=*iter;
    low->at(node)=*iter;
    st->push(node);
    stackmember->at(node)=true;
    
    //Get neighbors of node
    for(int i=0;i<X->at(node).In_Neigh.size();i++)
    {
        //ignore if deleted
        if(X->at(node).In_Neigh[i]==-1){continue;}
        
        int neigh=X->at(node).In_Neigh[i];
        
        //ignore if SCC is different
        if(SCCx->at(node)!=SCCx->at(neigh)){continue;}
        
        if(disc->at(neigh)==-1)
        {
            tarjan(neigh,X,SCCx,newSCC_ID, disc,low, stackmember,st,iter);
            if( low->at(node) > low->at(neigh))
            {low->at(node) = low->at(neigh);}
        }
        else if (stackmember->at(neigh)==true)
        {
            if( low->at(node) > disc->at(neigh))
            {low->at(node) = disc->at(neigh);}
        }
    } //end of for
    
    //Popping th stack
    int w=0;
    vector<int> mySCC;
    mySCC.clear();
   
    if(low->at(node)==disc->at(node))
    {
         mySCC.clear();
        while(st->top()!=node)
        {
            w=st->top();
            stackmember->at(w)=false;
            mySCC.push_back(w);
            st->pop();
        }//end of while
        
        w=st->top();
        stackmember->at(w)=false;
        mySCC.push_back(w);
        st->pop();
        
        //Get new ID
        int id=*min_element(mySCC.begin(),mySCC.end());
        for(int i=0; i<mySCC.size();i++)
        {newSCC_ID->at(mySCC[i])=id;}

    }//end of if
}

/*** End of Function **/

void multiple_tarjan(D_Network *X,vector<int> *SCCx, vector<bool> *delete_nodes, vector<bool> *is_connect, int p )
{
    vector<int> newSCC_ID; //stores the SCC ids of the vertices
    newSCC_ID.resize(X->size(),-1);
    

    //Variables for Tarjan's algorithm
    vector<int> disc;
    disc.resize(X->size(),-1);
    
    vector<int> low;
    low.resize(X->size(), -1);
    
    vector<bool> stackmember;
    stackmember.resize(X->size(),false);
    
    #pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int i=0;i<delete_nodes->size();i++)
    {
        if(delete_nodes->at(i)==false) {continue;}
        
         int iter=0;
         stack<int> mystack;
         if(disc[i]==-1)
         {tarjan(i,X,SCCx,&newSCC_ID, &disc,&low, &stackmember, &mystack, &iter);}
    }
    
    disc.clear();
    low.clear();
    stackmember.clear();
    
    //print_vector(newSCC_ID);
    //print_vector(*SCCx);
    
 #pragma omp parallel for num_threads(p)
    for(int k=0;k<SCCx->size();k++)
    {
        if(newSCC_ID[k]!=-1)
        {SCCx->at(k)=newSCC_ID[k];
            is_connect->at(k)=true;
        }//end of if
    }//end of for
    
    
}
/*** End of Function **/


#endif /* Tarjan_h */
