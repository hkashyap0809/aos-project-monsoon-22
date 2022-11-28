#include<bits/stdc++.h>
using namespace std;

class Aging{
    public:
    int numberofpages;
    Aging(int n){
        numberofpages=n;
    }
    int size=0;
    vector<pair<int,int>>vec;
    int numberofreference=0,pagefault=0;
    int entry;
    void aging(int entry);
};
void Aging::aging(int entry){
    int n=numberofpages;
    int flag=0;
    numberofreference++;
    for(auto &it:vec){
        if(it.first==entry){
            flag=1;
        }
    }
    if(flag==1){
        int val=0;
        for(auto &it:vec){
            if(it.first==entry){
                val=it.second;
            }
        }
        for(auto &it:vec){
            if(it.second>val){
                it.second--;
            }
        }
        for(auto &it:vec){
            if(it.first==entry){
                it.second=n;
            }
        }
    }
    if(flag==0){
        pagefault++;
        if(vec.size()<n){
            for(auto &it:vec){
                it.second-=1;
            }
            vec.push_back({entry,n});
        }
        else{
            for(auto &it:vec){
                it.second-=1;
            }
            for(auto it=vec.begin();it!=vec.end();it++){
                if(it->second==0){
                    vec.erase(it);
                }break;
            }
            vec.push_back({entry,n});
            for(auto it=vec.begin();it!=vec.end();it++){
                if(it->second<=0){
                    vec.erase(it);
                }
            }
        }

    }

    // for(auto it:vec){
    //     cout<<entry<<" "<<it.first<<"("<<it.second<<")"<<" || ";
    // }
    // cout<<endl;



}

int main()
{
    int t;
    cin>>t;
    int n;
    // cin>>n;
    n=4;
    Aging ob(n);
    while (t--)
    {   int entry;
        cin>>entry;
        ob.aging(entry);
        // cout<<"---------------"<<endl;
    }
    cout<<endl<<"pagefault: "<<ob.pagefault<<endl;
    cout<<"hitrate: "<<(float)(ob.numberofreference-ob.pagefault)/ob.numberofreference<<endl;
    cout<<"refereces: "<<ob.numberofreference<<endl;
    return 0;
}