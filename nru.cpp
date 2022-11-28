#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct PageDetails
{
	int page;
	unsigned short referenced = 0;
	unsigned short modified = 0;
	unsigned short removalClass = 0;
};

class NRUPolicy
{
public:
	double hits = 0;
	double totalreference = 0;
	int frames = 0;
	int clock = 0;
	int clockTriggerConst = 1;
	unordered_map<int, PageDetails> mainMemMap;
	vector<PageDetails> mainMemVec;

	NRUPolicy(int frames)
	{
		this->frames = frames;
		if (frames > 3)
			clockTriggerConst = frames / 2;
	}

	static bool cmprtr(PageDetails &page1, PageDetails &page2)
	{
		if (page1.removalClass == page2.removalClass)
			return true;

		return page1.removalClass > page2.removalClass;
	}

	double hitRate()
	{
		return (hits / totalreference);
	}

	double missRate()
	{
		return (1 - hitRate());
	}

	void setReferenceToZero()
	{
		unordered_map<int, PageDetails>::iterator iter;
		for (iter = mainMemMap.begin(); iter != mainMemMap.end(); iter++)
		{
			iter->second.referenced = 0;
		}
	}

	void setClass()
	{
		unordered_map<int, PageDetails>::iterator iter;
		for (iter = mainMemMap.begin(); iter != mainMemMap.end(); iter++)
		{
			PageDetails pageDetails = iter->second;
			unsigned short ref = pageDetails.referenced;
			unsigned short mod = pageDetails.modified;
			if (ref == 0 && mod == 1)
				iter->second.removalClass = 1;
			else if (ref == 1 && mod == 0)
				iter->second.removalClass = 2;
			else if (ref == 1 && mod == 1)
				iter->second.removalClass = 3;
		}

		for (int i = 0; i < mainMemVec.size(); i++)
		{
			PageDetails pageDetails = mainMemMap[mainMemVec[i].page];
			unsigned short ref = pageDetails.referenced;
			unsigned short mod = pageDetails.modified;
			if (ref == 0 && mod == 1)
				mainMemVec[i].removalClass = 1;
			else if (ref == 1 && mod == 0)
				mainMemVec[i].removalClass = 2;
			else if (ref == 1 && mod == 1)
				mainMemVec[i].removalClass = 3;
		}
	}
	void printPageDetails(PageDetails pageDetails)
	{
		cout << "PAGE : ";
		cout << "[" << pageDetails.page << "," << pageDetails.referenced << "," << pageDetails.modified << "," << pageDetails.removalClass << "]\n";
	}

	void printPages()
	{
		cout << "|";
		for (auto page : mainMemVec)
		{
			cout << page.page << "|";
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

	void put(int page)
	{
		// cout << "PUT " << page << "\n";
		if (mainMemMap.find(page) != mainMemMap.end())
		{
			// page present in memory
			PageDetails pageDetails = mainMemMap[page];
			// printPageDetails(pageDetails);
			pageDetails.modified = 1;
			mainMemMap[page] = pageDetails;

			for (auto eachPage : mainMemVec)
			{
				if (eachPage.page == page)
				{
					eachPage.modified = 1;
					break;
				}
			}
			printOutput(page, -1);
			hits++;
		}
		else
		{
			// page not found
			if (frames == 0)
				return;

			if (frames <= mainMemMap.size())
			{
				// main memory is full
				PageDetails pageDetails = mainMemVec[mainMemVec.size() - 1];
				mainMemVec.pop_back();

				// cout << "erasing " << pageDetails.page << "\n";
				mainMemMap.erase(pageDetails.page);

				struct PageDetails newPage;
				newPage.page = page;
				newPage.referenced = 1;
				mainMemMap[page] = newPage;

				mainMemVec.push_back(newPage);
				printOutput(page, pageDetails.page);
			}
			else
			{
				// main memory available
				struct PageDetails pageDetails;
				pageDetails.page = page;
				pageDetails.referenced = 1;
				mainMemMap[page] = pageDetails;

				mainMemVec.push_back(pageDetails);
				printOutput(page, -1);
			}
		}
		clock++;
		totalreference++;
		if (clock == clockTriggerConst)
		{
			clock = 0;
			setReferenceToZero();
		}
		setClass();
		sort(mainMemVec.begin(), mainMemVec.end(), cmprtr);
	}

	// void printPages()
	// {
	// 	unordered_map<int, PageDetails>::iterator iter;
	// 	for (iter = mainMemMap.begin(); iter != mainMemMap.end(); iter++)
	// 	{
	// 		struct PageDetails page = iter->second;
	// 		cout << iter->first << " : [" << page.page << ","
	// 			 << page.referenced << "," << page.modified << "," << page.removalClass << "]  ";
	// 	}

	// 	cout << "\n";
	// }

	void printVec()
	{
		for (auto page : mainMemVec)
		{
			cout << "[" << page.page << "," << page.referenced << "," << page.modified << "," << page.removalClass << "] ";
		}
	}
};

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

void printVec(vector<int> &vec)
{
	for (auto page : vec)
	{
		cout << page << "\t";
	}
	cout << "\n";
}

int main()
{
	string input;
	vector<int> pageSequence;

	getline(cin, input);
	tokenize(input, pageSequence);

	int noOfFrames = pageSequence.back();
	pageSequence.pop_back();

	cout << "FRAMES : " << noOfFrames << "\n";
	cout << "PAGE SEQUENCE\n";
	printVec(pageSequence);

	NRUPolicy nru(noOfFrames);

	for (auto page : pageSequence)
	{
		nru.put(page);

		// cout << "MAP : ";
		// nru.printPages();
		// cout<<"VEC : ";
		// nru.printVec();
		// cout << "\n";
	}

	cout << "HITS : " << nru.hits << "\n";
	cout << "TOTAL REF : " << nru.totalreference << "\n";
	cout << "HIT RATIO : " << nru.hitRate() << "\n";
}
