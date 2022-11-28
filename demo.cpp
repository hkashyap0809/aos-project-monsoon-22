#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    ofstream writer("hitRate.txt", ios::app);

    if (!writer)
    {
        cout << "Error Opening File" << endl;
        return -1;
    }

    string info = "fr"; // insert your text to be appended here
    writer.append(info);

    writer << info << endl;
    writer.close();
    return 0;
}