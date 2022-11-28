#include "page.h"

class WS:public PageReplacementScheme
{
private:
    vector<Page> workingset;
    int evictionthreshold;
    int clockTickFreq=2;
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