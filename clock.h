#include "page.h"

class Clock:public PageReplacementScheme{
private:
    vector<Page> pageframes;
    int framesize;
    int clockAt=0;
    int clockTickFreq=1 ;
protected:
    int isPageLoaded(int pagenum);
    void printOutput(int inpage,int outpage, int missed);
    int replacePage(int pagenum);
    void insertPage(int pagenum);
    void runclock();
public:
    void simulatePageReplacement(const vector <int> refVec, int framesize );
};