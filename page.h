#ifndef PAGE_H
#define PAGE_H

#include <vector>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <iomanip>

using namespace std;

struct Page{
    int pagenum;
    bool usebit;
    int lastUsedTick;
};

class PageReplacementScheme
{
protected:
    int virtualTime=0;
    int hitcount=0;
    int misscount=0;

    virtual int isPageLoaded(int pagenum)=0;
    virtual void printOutput(int inpage,int outpage, int missed)=0;
    //virtual void refreshWorkingSet(){};
    virtual void insertPage(int pagenum)=0;
    virtual void runclock()=0;
    virtual int replacePage(int pagenum)=0;
public:
    virtual double getHitRate(){return double(hitcount)/(hitcount+misscount);};
    virtual double getMissRate(){return double(misscount)/(hitcount+misscount);};
    virtual void simulatePageReplacement(const vector<int> refVec , int framesize)=0;
};

#endif