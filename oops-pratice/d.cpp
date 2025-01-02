#include <bits/stdc++.h>
using namespace std;



int main()
{
   int a,b; cin>>a>>b;
   try{
    if(b==0){
        throw "Division by Zero";
    }
    int res=a/b;
    cout<<res<<endl;
   }catch(const char* e){
      cout<<"Exception Occured "<<e<<endl;
   }

try{
   int *p=new int[10000000000000000];
   cout<<"Memory Allocated"<<endl;
   delete []p;
//    khod throw kar rha hai
// exception class
}catch(const exception &e){
    cout<<"Exception Occured: "<<e.what()<<endl;
}

}
