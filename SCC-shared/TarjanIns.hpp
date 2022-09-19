//
//  Tarjan.hpp
//  
//
//  Created by Sanjukta Bhowmick on 4/4/19.
//Modified from https://www.geeksforgeeks.org/tarjan-algorithm-find-strongly-connected-components/
//

#ifndef TarjanIns_h
#define TarjanIns_h

void tarjan_in(int node, D_Network *X, vector<bool> *trimmed, vector<int> *SCCx,  vector< vector< int > > *mySCC,  vector< vector< int > > *st_index, vector< vector< int > > *min_id, vector<int> *disc, vector<int> *low, vector<bool> *stackmember, stack<int>* st, int *iter )
{
 
    {*iter=*iter+1;}
    
    disc->at(node)=*iter;
    low->at(node)=*iter;
    st->push(node);
    stackmember->at(node)=true;
    
    //Get neighbors of node
    for(int i=0;i<X->at(node).In_Neigh.size();i++)
    {
       
        int neigh=X->at(node).In_Neigh[i];
        if(trimmed->at(neigh)==true){continue;}
    
        
        if(disc->at(neigh)==-1)
        {
            tarjan_in(neigh,X,trimmed,SCCx, mySCC, st_index, min_id,disc,low, stackmember, st, iter);
            if( low->at(node) > low->at(neigh))
            {low->at(node) = low->at(neigh);}
        }
        else if (stackmember->at(neigh)==true)
        {
            if( low->at(node) > disc->at(neigh))
            {low->at(node) = disc->at(neigh);}
        }
    } //end of for
    
    //Popping the stack
    int w=0;
   
    if(low->at(node)==disc->at(node))
    {
        st_index->at(node).push_back(mySCC->at(node).size());
        int mymin=X->size()+2;
        
        while(st->top()!=node)
        {
            w=st->top();
            stackmember->at(w)=false;
            mySCC->at(node).push_back(w);
            if(mymin>w){mymin=w;}
            st->pop();
        }//end of while
        
        w=st->top();
        stackmember->at(w)=false;
        mySCC->at(node).push_back(w);
          if(mymin>w){mymin=w;}
        st->pop();
        
        min_id->at(node).push_back(mymin);
        
    }//end of if
}

/*** End of Function **/

void multiple_tarjan_ins(D_Network *M,vector<int> *SCCx, vector<bool> *insert_nodes, vector<bool> *trimmed, int p )
{
    vector<int> dummy;
    dummy.clear();
    vector< vector<int> > mySCC;
    mySCC.resize(M->size(),dummy);
    
  
    vector< vector<int> > st_index;
    st_index.resize(M->size(),dummy);
    
    vector< vector<int> > min_id;
    min_id.resize(M->size(),dummy);
    
    vector<bool> did_tarjan;
    did_tarjan.resize(M->size(),false);
    
#pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int i=0;i<M->size();i++)
    {
      //  printf("Tarjan:::: %d \n",i);
        if(insert_nodes->at(i)==false){continue;}
        
        if( (trimmed->at(i)==true)) {continue;}
        
        
       // printf("Tarjan....==== %d \n",i);
    //Variables for Tarjan's algorithm
        
        did_tarjan[i]=true;
        
    vector<int> disc;
    disc.resize(M->size(),-1);
    
    vector<int> low;
    low.resize(M->size(), -1);
    
    vector<bool> stackmember;
    stackmember.resize(M->size(),false);
    
       // printf("Tarjan %d \n",i);
        int iter=0;
        stack<int> mystack;
        tarjan_in(i,M,trimmed,SCCx, &mySCC, &st_index, &min_id,&disc,&low, &stackmember, &mystack, &iter);
        
        disc.clear();
        low.clear();
        stackmember.clear();
    }
    
    
    //Get the lowest SCC_ID
    bool changed=true;
    while(changed)
    {
        changed=false;
#pragma omp parallel for num_threads(p) schedule(dynamic)
    for(int i=0;i<M->size();i++)
    {
        //Ignore if not seed node for tarjan
        if(did_tarjan[i]==false){continue;}
        
        for(int j=0;j<st_index[i].size();j++)
        {
            int mymin=min_id[i][j];
            
            for(int k=st_index[i][j]; k<st_index[i][j+1]; k++)
            {
                if(k==mySCC[i].size()){break;}
                
                int node=mySCC[i][k];
                
                
                if(SCCx->at(node)>mymin)
                {SCCx->at(node)=mymin;
                    changed=true;}
            }//end of for k
        }//end of for j

    }//end of for i
        
    }//end of while

}
/*** End of Function **/


#endif /* Tarjan_h */
