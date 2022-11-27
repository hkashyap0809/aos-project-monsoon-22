#include "page.h"

class WSClock:public PageReplacementScheme
{
private:
    vector<Page> workingset;
    int clockAt=0;
    int evictionthreshold;
    int clockTickFreq=1;
protected:
    int isPageLoaded(int pagenum);
    void printOutput(int inpage,int outpage, int missed);
    void refreshWorkingSet();
    int replacePage(int pagenum);
    void insertPage(int pagenum);
    void runclock();
public:
    void simulatePageReplacement(const vector<int> refVec , int framesize);
};

