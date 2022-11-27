#include "ws.h"


int WS::isPageLoaded(int pagenum){
    for(auto itr=workingset.begin();itr!=workingset.end();++itr){
        if (itr->pagenum == pagenum) return itr-workingset.begin();
    }
    return -1;
}

void WS::printOutput(int inpage,int outpage, int missed){
        cout<<"\033[0;32;1m"<<inpage<<"--->"<<"\033[0m";
        for(auto item=workingset.begin();item!=workingset.end();++item){
            cout<<"|"<<item->pagenum;
        }
         cout<<"|";
        if(outpage!=-1)
            cout<<"\033[0;31;1m"<<"--->"<<outpage<<"\033[0m";
        cout<<endl;
        
}

void WS::refreshWorkingSet(){
    for(auto itr=workingset.begin();itr!=workingset.end();++itr){
        if(virtualTime-itr->lastUsedTick>=evictionthreshold){
            workingset.erase(itr);
            break;
        }
    }
}

int WS::replacePage(int pagenum){
    bool pageadded=false;
    int replacedpage=-1;
    for(auto itr=workingset.begin();itr!=workingset.end();){
        if(itr->usebit==1){
            itr->lastUsedTick=virtualTime;
        }
        else if(itr->usebit==0 && virtualTime-itr->lastUsedTick>=evictionthreshold){
            if (pageadded==false){
                replacedpage=itr->pagenum;
                itr->pagenum=pagenum;
                itr->usebit=1;
                itr->lastUsedTick=virtualTime;
                pageadded=true;
            }
            else{
                workingset.erase(itr);
                continue;
            }
        }
        ++itr;
    }

    if(pageadded==false)
        workingset.push_back(Page({pagenum,1,virtualTime}));

    return replacedpage;
}

void WS::insertPage(int pagenum){
    int pageidx=isPageLoaded(pagenum);
    int missed=0;
    int replacedpage=-1;
    if (pageidx==-1){
        ++misscount;
        replacedpage=replacePage(pagenum);
        missed=1;
    }
    else{
        ++hitcount;
        auto page=workingset.begin()+pageidx;
        page->lastUsedTick=virtualTime;   
        missed=0;
    }
    refreshWorkingSet();
    printOutput(pagenum,replacedpage,missed);
}

void WS::runclock(){
    ++virtualTime;
    for(auto itr=workingset.begin();itr!=workingset.end();++itr)
        itr->usebit=0;
}

void WS::simulatePageReplacement(const vector<int> refVec , int framesize ){
    cout<<"Simulating WS Algo.\n";
    evictionthreshold=int(framesize/clockTickFreq);
    for(int i=0; i<refVec.size();i++)
    {
        if((i)%clockTickFreq==0)
            runclock();

        auto itr=refVec.at(i);  
        insertPage(itr);
    }
    cout<<"Hit Rate --> "<<setprecision(2)<<(float)hitcount/(hitcount+misscount)<<endl;
    cout<<"Hit to Fault Ratio --> "<<setprecision(2)<<(float)hitcount/(misscount)<<endl;
}

