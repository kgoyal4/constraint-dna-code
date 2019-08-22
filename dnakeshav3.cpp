/*
 Algorithm : Altruistic Algorithm to Generate DNA Codewords with runlength constraint atmost three. 
Input: DNA code length n, GC-weight w, minimum distance d
Output: Altruistic DNA codebook
1 Generate all valid codewords over A/G/C/T such that no base repeat more than
three times consecutively and having GC-weight w.
2 For each codeword in the list, count all the codewords at distance d − 1 in a sphere.
3 Remove the codeword with a maximum number of codewords in radius d − 1. Reduce the number of codewords
at distance d − 1 by 1 for all codewords that were within distance d − 1 of this removed codeword.
4 Repeat the process in Step3 till the maximum number of codewords within distance d − 1
is at most 1 for each element of the list.
5 Add those removed codewords in codebook which are at distance greater than d-1
with all codewords in codebook.    
*/
#include <bits/stdc++.h>
using namespace std;
 struct Node{
    long data;
    Node* next;
};
int weight(string prefix,int w){
    int sum=0;
    for(int i=0;i<prefix.size();i++){
        if(prefix[i]=='G' || prefix[i] =='C')
        {
            sum++;
        }
        if(sum>w)
          return -1;
    }
    return sum;
}
void generateword(vector< pair <string, pair <long,Node*> > > &vec,char arr[], string prefix,int m, int n, int w) 
{    
    if (n == 0)  
    {
        if(weight(prefix,w)==w){
            Node* head = NULL;
            vec.push_back(make_pair(prefix,make_pair(0,head)));  
        }
        return;
    } 
    for (int i = 0; i < m; ++i) 
    { 
  
      if((weight(prefix,w)>w)||(prefix.size()>2&&arr[i]==prefix[prefix.size()-1]&&arr[i]==prefix[prefix.size()-2]&&arr[i]==prefix[prefix.size()-3]))
		        continue;
            string newPrefix = prefix + arr[i];
            generateword(vec,arr, newPrefix, m, n - 1, w);
    }   
}
int distance(string a, string b, int n){
    int dis = 0;
    for(int i = 0; i < n; i++)
    if(a[i]!=b[i])
       dis++;
    return dis;
}
Node* newNode(long data)
{
    Node* temp = new Node;
    temp->data = data;
    temp->next = NULL;
    return temp;
}
void atdistance(vector< pair <string, pair <long,Node*> > > &vec,int n,int d){
    for(long i=0;i<vec.size();i++){
        for(long j=i+1;j<vec.size();j++){
            int dist = distance(vec[i].first,vec[j].first,n);
            if(dist<d&&dist>0){
                vec[i].second.first++;
                vec[j].second.first++;
                Node* ptr_it = newNode(i);
                Node* ptr_it1 = newNode(j);
                if(vec[i].second.second == NULL)
                    vec[i].second.second = ptr_it1;
                else{
                    Node* temp = vec[i].second.second;
                    while(temp->next!=NULL) temp = temp->next;
                    temp->next = ptr_it1;
                }
                if(vec[j].second.second == NULL)
                    vec[j].second.second = ptr_it;
                else{
                    Node* temp = vec[j].second.second;
                    while(temp->next!=NULL) temp = temp->next;
                    temp->next = ptr_it;
                }
            }
        }
    }
}
void deletemax(vector< pair <string, pair <long,Node*> > > &vec,vector< pair <string, pair <long,Node*> > > &vecm,int n,int d,int w,ofstream &output){
    while(true){
         long max=vec[0].second.first,x=0;
              for(int i=1;i<vec.size();i++)
              if(max<vec[i].second.first)
                 {
                 	x=i;
				    max=vec[i].second.first;
			     }
                 if(max==0)
                    break;
        Node* ptr1 = vec[x].second.second;
        while(ptr1!= NULL){
            if( vec[ptr1->data].second.first!= -1){
                vec[ptr1->data].second.first--;
            }
            ptr1 = ptr1->next;
        }
        vec[x].second.first= -1;
    }
    output<<"Number of Vaild Codewords("<<n<<","<<w<<") : "<<vec.size()<<endl;
  for(int i=0;i<vec.size();i++)
        if(vec[i].second.first!=-1)
			vecm.push_back(vec[i]);
	for(int i=0;i<vec.size();i++)
		{
			int f=0;
			for(int j=0;j<vecm.size();j++)
			{
				 if((vec[i].second.first==0)||(vec[i].second.first==-1)&&(distance(vec[i].first,vecm[j].first,n)<d))
				  {
				      f=1;
					  break;	
				  }	
			}
			if(f==0)
			{
			    vec[i].second.first=0;
			   vecm.push_back(vec[i]);
		    }
		}
		  output<<"Number of Vaild Codewords("<<n<<","<<w<<","<<d<<") : "<<vecm.size()<<endl;
 }
int main()
 {
   ifstream input;
   ofstream output;
   input.open("input.txt"); //add input file location
   output.open("output.txt"); //add output file location
    char arr[] = {'G', 'A', 'C', 'T'};
    int d, n, w;
    int t;
    input>>t;
    for(int i=0;i<t;i++){
        input>>n>>d;
        if(d>n)
        {
        	cout<<"ERROR! HAMMING DISTANCE CANNOT BE GREATER THEN n"<<endl;
        	       continue;
		}
          w = n/2;
        vector< pair <string, pair <long,Node*> > > vec;
        vector< pair <string, pair <long,Node*> > > vecm;
          generateword(vec,arr, "", 4, n, w);
          atdistance(vec,n, d);
          deletemax(vec,vecm,n,d,w,output);
    }
     input.close();
    output.close();
    return 0;
}
