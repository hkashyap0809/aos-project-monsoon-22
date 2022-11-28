#include <bits/stdc++.h>
using namespace std;

class RandomPolicy{
public:
    int frames;
    RandomPolicy(int frames){
        this->frames = frames;
    }
    unordered_map<int, int> mapa;
    int size = 0;
    double totalReference = 0, pagefault = 0;
    
    double hitRate(){
        return 1-(pagefault/totalReference);
    }
    void printPages(){
        unordered_map<int,int> :: iterator iter;
        cout<<"|";
        for(iter = mapa.begin(); iter!=mapa.end() ; iter++){
            cout<<iter->first<<"|";
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
    void put(int entry){
        totalReference++;
        if (mapa.find(entry) == mapa.end()){
            pagefault++;
            if (size == frames)
            {
                int ad = rand() % mapa.size();
                auto it = mapa.begin();
                int i = 0;
                for (; i < ad; i++){
                    it++;
                }
                printOutput(entry,it->first);
                mapa.erase(it);
                mapa[entry]++;
            }
            else
            {
                mapa[entry]++;
                size++;
                printOutput(entry,-1);
            }
        }
    }
};

// void RandomPolicy::put(int entry)
// {
//     numberofreference++;
//     if (mapa.find(entry) == mapa.end())
//     {
//         pagefault++;
//         if (size == frames)
//         {

//             cout << endl;
//             int ad = rand() % mapa.size();
//             auto it = mapa.begin();
//             int i = 0;
//             for (; i < ad; i++)
//             {
//                 it++;
//             }
//             mapa.erase(it);
//             mapa[entry]++;
//         }
//         else
//         {
//             mapa[entry]++;
//             size++;
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

    RandomPolicy randomPolicy(frames);

    for (auto page : pageSequence)
    {
        randomPolicy.put(page);
    }
    cout << randomPolicy.totalReference << "\n";
    cout << randomPolicy.hitRate();

    return 0;
}

// int main(){
    
//     int n;
//     n = 100;
//     Random ob(n);
//     int t;
//     cin >> t;
//     srand(time(0));

//     while (t--)
//     {
//         int entry;
//         cin >> entry;
//         ob.randompr(entry);
//         cout << endl;
//     }
//     cout << endl
//          << "pagefault: " << ob.pagefault << endl
//          << "refereces: " << ob.numberofreference;
//     return 0;
// }