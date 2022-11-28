#include <bits/stdc++.h>
using namespace std;

class Aging
{
public:
    int frames;
    int size = 0;
    vector<pair<int, int>> vec;
    double numberofreference = 0, pagefault = 0;
    Aging(int frames)
    {
        this->frames = frames;
    }

    double hitRate()
    {
        return 1 - (pagefault / numberofreference);
    }
    
    void printPages(){
        cout<<"|";
        for(auto page : vec){
            cout<<page.first<<"|";
        }
    }
    
    void printOutput(int insertedPage, int removedPage)
    {
        cout << insertedPage << "-->";
        printPages();
        if (removedPage != -1)
            cout << "-->" << removedPage;

        cout << "\n";
    }

    void put(int entry)
    {
        int n = frames;
        int flag = 0;
        numberofreference++;
        for (auto &it : vec)
        {
            if (it.first == entry)
            {
                flag = 1;
            }
        }
        if (flag == 1)
        {
            int val = 0;
            for (auto &it : vec)
            {
                if (it.first == entry)
                {
                    val = it.second;
                }
            }
            for (auto &it : vec)
            {
                if (it.second > val)
                {
                    it.second--;
                }
            }
            for (auto &it : vec)
            {
                if (it.first == entry)
                {
                    it.second = n;
                }
            }
        }
        if (flag == 0)
        {
            pagefault++;
            if (vec.size() < n)
            {
                for (auto &it : vec)
                {
                    it.second -= 1;
                }
                vec.push_back({entry, n});
                printOutput(entry,-1);
            }
            else
            {   
                int ex;
                for (auto &it : vec)
                {
                    it.second -= 1;
                }
                auto it = vec.begin();
                for (it = vec.begin(); it != vec.end(); it++)
                {
                    if (it->second == 0)
                    {
                        ex=it->first;
                        vec.erase(it);
                    }
                    break;
                }
                vec.push_back({entry, n});
                it = vec.begin();
                for (it = vec.begin(); it != vec.end(); it++)
                {
                    if (it->second <= 0)
                    {
                        ex=it->first;
                        vec.erase(it);
                    }
                }
                printOutput(entry,ex);
            }
        }
    }
};
// void Aging::aging(int entry)
// {
//     int n = numberofpages;
//     int flag = 0;
//     numberofreference++;
//     for (auto &it : vec)
//     {
//         if (it.first == entry)
//         {
//             flag = 1;
//         }
//     }
//     if (flag == 1)
//     {
//         int val = 0;
//         for (auto &it : vec)
//         {
//             if (it.first == entry)
//             {
//                 val = it.second;
//             }
//         }
//         for (auto &it : vec)
//         {
//             if (it.second > val)
//             {
//                 it.second--;
//             }
//         }
//         for (auto &it : vec)
//         {
//             if (it.first == entry)
//             {
//                 it.second = n;
//             }
//         }
//     }
//     if (flag == 0)
//     {
//         pagefault++;
//         if (vec.size() < n)
//         {
//             for (auto &it : vec)
//             {
//                 it.second -= 1;
//             }
//             vec.push_back({entry, n});
//         }
//         else
//         {
//             for (auto &it : vec)
//             {
//                 it.second -= 1;
//             }
//             for (auto it = vec.begin(); it != vec.end(); it++)
//             {
//                 if (it->second == 0)
//                 {
//                     vec.erase(it);
//                 }
//                 break;
//             }
//             vec.push_back({entry, n});
//             for (auto it = vec.begin(); it != vec.end(); it++)
//             {
//                 if (it->second <= 0)
//                 {
//                     vec.erase(it);
//                 }
//             }
//         }
//     }
// }

void tokenize(string &inputSequence, vector<int> &pageSequence)
{
    stringstream inputStream(inputSequence);
    string str;
    char delimiter = ' ';
    while (getline(inputStream, str, delimiter))
    {
        pageSequence.push_back(stoi(str));
    }
}
int main(){
    int frames;
    string input;
    vector<int> pageSequence;


    cout << "Enter the number of frames : ";
    cin >> frames;
    cout << "Enter the page sequence : ";
    cin.ignore();
    getline(cin, input);
    cout<<"\n";
    tokenize(input, pageSequence);

    Aging aging(frames);

    for (auto page : pageSequence)
    {
        aging.put(page);
    }
    cout << aging.hitRate();

    return 0;
}

// int main()
// {
//     int t;
//     cin >> t;
//     int n;
//     // cin>>n;
//     n = 4;
//     Aging ob(n);
//     while (t--)
//     {
//         int entry;
//         cin >> entry;
//         ob.aging(entry);
//         // cout<<"---------------"<<endl;
//     }
//     cout << endl
//          << "pagefault: " << ob.pagefault << endl;
//     cout << "hitrate: " << (float)(ob.numberofreference - ob.pagefault) / ob.numberofreference << endl;
//     cout << "refereces: " << ob.numberofreference << endl;
//     return 0;
// }

// // #include <bits/stdc++.h>
// // using namespace std;

// // class AgingPolicy{
// //     public:
// //         int frames=0;
// //         double hits = 0;
// //         double totalReference = 0;
// //         // <page,age>
// //         vector<pair<int,int>> mainMemVec;
// //         unordered_set<int> mainMemSet;
// //         AgingPolicy(int frames){
// //             this->frames = frames;
// //         }

// //         static bool cmprtr(pair<int,int> &a, pair<int,int> &b){
// //             return a.second>b.second;
// //         }
// //         double hitRate(){
// //             return hits/totalReference;
// //         }
// //         void setAge(){
// //             for(int i=0;i<mainMemVec.size();i++){
// //                 mainMemVec[i].second--;
// //             }
// //         }
// //         void printPages(){
// //             for(auto page : mainMemVec){
// //                 cout<<"("<<page.first<<","<<page.second<<")"<<"| ";
// //             }
// //         }
// //         void printOutput(int insertedPage, int removedPage){
// //             cout << insertedPage << "-->";
// //             printPages();
// //             if (removedPage != -1)
// //                 cout << "-->" << removedPage;

// //             cout << "\n";
// //         }

// //         void put(int page){
// //             totalReference++;
// //             if(mainMemSet.find(page) != mainMemSet.end()){
// //                 setAge();

// //                 mainMemVec[0] = {page,frames};
// //                 printOutput(page,-1);
// //             }else{
// //                 // page not found
// //                 if(frames==0)
// //                     return;
// //                 if(frames<=mainMemSet.size()){
// //                     // remove the page
// //                     setAge();
// //                     int removedPage = mainMemVec[mainMemVec.size()-1].first;
// //                     mainMemVec.pop_back();

// //                     mainMemSet.erase(page);
// //                     printOutput(page,removedPage);

// //                 }else{
// //                     // first time insert the page
// //                     mainMemSet.insert(page);
// //                     mainMemVec.push_back({page,frames});
// //                     printOutput(page,-1);
// //                 }
// //             }
// //             sort(mainMemVec.begin(), mainMemVec.end(),cmprtr);
// //         }

// // };
// // void tokenize(string &inputSequence, vector<int> &pageSequence)
// // {
// //     stringstream inputStream(inputSequence);
// //     string str;
// //     char delimiter = ' ';
// //     while (getline(inputStream, str, delimiter))
// //     {
// //         pageSequence.push_back(stoi(str));
// //     }
// // }
// // int main(){
// //     int frames;
// //     string input;
// //     vector<int> pageSequence;

// //     cout << "Enter the number of frames : ";
// //     cin >> frames;
// //     cout << "Enter the page sequence : ";
// //     cin.ignore();
// //     getline(cin, input);
// //     tokenize(input, pageSequence);
// //     cout<<"\n";

// //     AgingPolicy aging(frames);

// //     for (auto page : pageSequence)
// //     {
// //         aging.put(page);
// //     }
// //     cout << aging.hits << "\n";
// //     cout << aging.totalReference << "\n";
// //     cout << aging.hitRate();

// //     return 0;
// // }
// // // int size = 0;
// // // int numberofpages = 4;
// // // vector<pair<int, int>> vec;
// // // int numberofreference = 0, pagefault = 0;
// // // int entry;
// // // int n = numberofpages;
// // // void aging(int entry)
// // // {
// // //     int flag = 0;

// // //     numberofreference++;
// // //     // found

// // //     for (auto it = vec.begin(); it != vec.end(); it++)
// // //     {
// // //         if (it->first == entry)
// // //         {
// // //             flag = 1;
// // //         }
// // //     }
// // //     // if exist

// // //     if (flag == 1)
// // //     {
// // //         int val = 0;
// // //         for (auto it = vec.begin(); it != vec.end(); it++)
// // //         {
// // //             if (it->first == entry)
// // //             {
// // //                 val = it->second;
// // //                 it->second = n;
// // //             }
// // //         }
// // //         for (auto it = vec.begin(); it != vec.end(); it++)
// // //         {
// // //             if (it->first == entry)
// // //             {
// // //                 it->second = n;
// // //             }
// // //             else
// // //             {
// // //                 if (it->second >= val)
// // //                 {
// // //                     it->second -= 1;
// // //                 }
// // //             }
// // //         }
// // //     }

// // //     else
// // //     {
// // //         pagefault++;

// // //         if (vec.size() == numberofpages)
// // //         {
// // //             for (auto it = vec.begin(); it != vec.end(); it++)
// // //             {
// // //                 it->second -= 1;
// // //             }
// // //             for (auto it = vec.begin(); it != vec.end(); it++)
// // //             {
// // //                 if (it->second == 0)
// // //                 {
// // //                     vec.erase(it);
// // //                 }
// // //             }
// // //             vec.push_back({entry, n});
// // //         }

// // //         if (vec.size() < numberofpages)
// // //         {
// // //             for (auto &it : vec)
// // //             {
// // //                 it.second -= 1;
// // //             }
// // //             vec.push_back({entry, n});
// // //         }
// // //     }

// // //     cout << entry << " ";
// // //     for (auto it : vec)
// // //     {
// // //         cout << "" << it.first << "(" << it.second << ")"
// // //              << "||";
// // //     }
// // //     cout << vec.size() << endl;
// // // }

// // // int main()
// // // {
// // //     int t;
// // //     cin >> t;
// // //     while (t--)
// // //     {
// // //         int entry;
// // //         cin >> entry;
// // //         aging(entry);
// // //         // cout<<"---------------"<<endl;
// // //     }
// // //     cout << "hi";
// // //     cout << endl
// // //          << "pagefault: " << pagefault << endl;
// // //     cout << "hitrate: " << (float)(numberofreference - pagefault) / numberofreference << endl;
// // //     cout << "refereces: " << numberofreference << endl;
// // //     return 0;
// // // }