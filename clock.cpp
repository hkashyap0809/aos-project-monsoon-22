#include "clock.h"

int Clock::isPageLoaded(int pagenum)
{
    for (auto itr = pageframes.begin(); itr != pageframes.end(); ++itr)
    {
        if (itr->pagenum == pagenum)
            return itr - pageframes.begin();
    }
    return -1;
}

void Clock::printOutput(int inpage, int outpage, int missed)
{
    cout << "\033[0;32;1m" << inpage << "--->"
         << "\033[0m";
    for (auto item = pageframes.begin(); item != pageframes.end(); ++item)
    {
        cout << "|" << item->pagenum;
    }
    cout << "|";
    if (outpage != -1)
        cout << "\033[0;31;1m"
             << "--->" << outpage << "\033[0m";
    cout << endl;
}

int Clock::replacePage(int pagenum)
{
    bool pageadded = false;
    int replacedpage = -1;
    if (pageframes.size() < framesize)
    {
        ++clockAt;
        clockAt = clockAt % (framesize);
        pageframes.push_back(Page({pagenum, 1, virtualTime}));
        return replacedpage;
    }

    while (true)
    {
        ++clockAt;
        clockAt = clockAt % (framesize);
        auto itr = clockAt + pageframes.begin();
        if (itr == pageframes.end())
            itr = pageframes.begin();

        if (itr->usebit == 1)
        {
            itr->lastUsedTick = virtualTime;
        }
        else if (itr->usebit == 0)
        {
            if (pageadded == false)
            {
                replacedpage = itr->pagenum;
                itr->pagenum = pagenum;
                itr->usebit = 1;
                itr->lastUsedTick = virtualTime;
                pageadded = true;
                break;
            }
        }
        ++itr;
    }

    if (pageadded == false)
        pageframes.push_back(Page({pagenum, 1, virtualTime}));
    return replacedpage;
}

void Clock::insertPage(int pagenum)
{
    int pageidx = isPageLoaded(pagenum);
    int missed = 0;
    int replacedpage = -1;
    if (pageidx == -1)
    {
        ++misscount;
        replacedpage = replacePage(pagenum);
        missed = 1;
    }
    else
    {
        ++hitcount;
        auto page = pageframes.begin() + pageidx;
        page->lastUsedTick = virtualTime;
        missed = 0;
    }
    printOutput(pagenum, replacedpage, missed);
}

void Clock::runclock()
{
    ++virtualTime;
    for (auto itr = pageframes.begin(); itr != pageframes.end(); ++itr)
        itr->usebit = 0;
}

void Clock::simulatePageReplacement(const vector<int> refVec, int framesize)
{
    cout << "Simulating Clock Algo.\n";
    this->framesize = framesize;
    for (int i = 0; i < refVec.size(); i++)
    {
        if ((i) % clockTickFreq == 0)
            runclock();
        auto itr = refVec.at(i);
        insertPage(itr);
    }

    // cout<<"Hit Rate --> "<<setprecision(2)<<(float)hitcount/(hitcount+misscount)<<endl;
    // cout<<"Hit to Fault Ratio --> "<<setprecision(2)<<(float)hitcount/(misscount)<<endl;
}