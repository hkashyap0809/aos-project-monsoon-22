#include "clock.h"
#include "ws.h"
#include "wsclock.h"
#include <cstring>
#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <fstream>
#include <stdlib.h>
using namespace std;

double fifo_ratio;
double fifo_2nd_ratio;
double optimal_ratio;

queue<int> q;
queue<int> q2;

#define COL_RED "\x1b[31m"
#define COL_GREEN "\x1b[32m"
#define COL_RESET "\x1b[0m"

#define printRED(message) printf("%s %s %s", COL_RED, message, COL_RESET);
#define printGREEN(message) printf("%s %s %s", COL_GREEN, message, COL_RESET);

// *********************************************
// UTILITY FUNCTIONS
// *********************************************
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

void printAlgoMenu()
{
	cout << "1. Random\n"
		 << "2. Optimal\n"
		 << "3. NRU(Not Recently Used)\n"
		 << "4. FIFO.\n"
		 << "5. FIFO with 2nd chance.\n"
		 << "6. Clock\n"
		 << "7. LRU (Least Recently Used)\n"
		 << "8. NFU (Not Frequently Used)\n"
		 << "9. Working Set\n"
		 << "10 Aging (approximate LRU)\n"
		 << "11. WorkingSet Clock.\n";
}

void printMenu()
{
	cout << "1. Simulate single page replacement algorithm.\n";
	cout << "2. Simulate all page replacement algorithm.\n";
	cout << "0. Exit\n";
}

void takeInput(int &frames, string &input)
{
	cout << "Enter the number of frames : ";
	cin >> frames;
	cin.ignore();
	//cout << "Enter the page sequence : ";
	//getline(cin, input);
	input = "42 65 30 23 71 76 82 58 10 93 17 80 39 62 100 13 92 16 34 73 79 37 61 41 25 17 51 38 72 95 21 9 32 57 62 53 75 84 85 66 91 33 62 20 97 24 12 50 77 3 49 1 45 82 56 90 62 11 99 99 99 99 99 99 87 29 14 82 36 40 5 17 80 39 62 100 13 92 16 34 73 79 37 61 41 25 52 51 38 94 59 83 19 43 68 63 44 15 81 18";
	cout <<input.length() << "\n";
}

// *********************************************
// RANDOM
// *********************************************

class RandomPolicy
{
public:
	int frames;
	RandomPolicy(int frames)
	{
		this->frames = frames;
	}
	unordered_map<int, int> mapa;
	int size = 0;
	double totalReference = 0, pagefault = 0;

	double hitRate()
	{
		return 1 - (pagefault / totalReference);
	}
	void printPages()
	{
		unordered_map<int, int>::iterator iter;
		cout << "|";
		for (iter = mapa.begin(); iter != mapa.end(); iter++)
		{
			cout << iter->first << "|";
		}
	}

	void printOutput(int insertedPage, int removedPage)
	{
		string msg = to_string(insertedPage) + "-->";
		printGREEN(msg.c_str());
		printPages();
		if (removedPage != -1)
		{
			printRED("-->");
			msg = to_string(removedPage);
			printRED(msg.c_str());
		}

		cout << "\n";
	}
	void put(int entry)
	{
		totalReference++;
		if (mapa.find(entry) == mapa.end())
		{
			pagefault++;
			if (size == frames)
			{
				int ad = rand() % mapa.size();
				auto it = mapa.begin();
				int i = 0;
				for (; i < ad; i++)
				{
					it++;
				}
				printOutput(entry, it->first);
				mapa.erase(it);
				mapa[entry]++;
			}
			else
			{
				mapa[entry]++;
				size++;
				printOutput(entry, -1);
			}
		}
	}
};

// *********************************************
// *********************************************

// *********************************************
// LRU (Least Recently Used)
// *********************************************
class LRUPageNode
{
public:
	int page;
	LRUPageNode *prev;
	LRUPageNode *next;

	LRUPageNode()
	{
		this->page = -9;
		this->prev = NULL;
		this->next = NULL;
	}

	LRUPageNode(int page)
	{
		this->page = page;
		this->prev = NULL;
		this->next = NULL;
	}
};

class LRUPageLinkedList
{
public:
	LRUPageNode *head = new LRUPageNode();
	LRUPageNode *tail = new LRUPageNode();
	LRUPageLinkedList()
	{
		head->next = tail;
		head->prev = NULL;
		tail->prev = head;
		tail->next = NULL;
	}
	LRUPageNode *insertAfterHead(int key)
	{
		LRUPageNode *newNode = new LRUPageNode(key);
		LRUPageNode *tempNode = head->next;
		head->next = newNode;
		newNode->prev = head;
		newNode->next = tempNode;
		tempNode->prev = newNode;
		return newNode;
	}

	void deleteNode(LRUPageNode *node)
	{
		LRUPageNode *previousNode = node->prev;
		LRUPageNode *nextNode = node->next;
		previousNode->next = nextNode;
		nextNode->prev = previousNode;

		delete (node);
	}

	LRUPageNode *deleteLRU()
	{
		LRUPageNode *node = tail->prev;
		LRUPageNode *previousNode = node->prev;
		previousNode->next = tail;
		tail->prev = previousNode;
		return node;
	}

	void print()
	{
		LRUPageNode *temp = head;
		while (temp != NULL)
		{
			cout << "(" << temp->page << ")  ";
			if (temp->next != NULL)
				cout << " -> ";
			temp = temp->next;
		}
		cout << "\n";
	}

	void printList()
	{
		LRUPageNode *temp = head->next;
		while (temp->next != NULL)
		{
			cout << "|" << temp->page << "|";
			temp = temp->next;
		}
	}
};

class LRUPolicy
{
public:
	int frames;
	unordered_map<int, LRUPageNode *> pageMap;
	LRUPageLinkedList pageList;
	double hits = 0;
	double totalReference = 0;

	LRUPolicy(int frames)
	{
		this->frames = frames;
	}

	double getHits()
	{
		return hits;
	}
	double getMiss()
	{
		return totalReference - hits;
	}
	double hitRate()
	{
		return hits / totalReference;
	}

	double missRate()
	{
		return (1 - hitRate());
	}
	void printOutput(int insertedPage, int removedPage)
	{
		string msg = to_string(insertedPage) + "-->";
		printGREEN(msg.c_str());
		pageList.printList();
		if (removedPage != -1)
		{
			printRED("-->");
			msg = to_string(removedPage);
			printRED(msg.c_str());
		}

		cout << "\n";
	}

	void put(int page)
	{
		totalReference++;
		if (pageMap.find(page) != pageMap.end())
		{
			// page found
			LRUPageNode *node = pageMap[page];
			pageList.deleteNode(node);
			LRUPageNode *newNode = pageList.insertAfterHead(page);
			pageMap.erase(page);
			pageMap[page] = newNode;
			hits++;
			printOutput(page, -1);
		}
		else
		{
			if (frames <= pageMap.size())
			{
				LRUPageNode *deletedNode = pageList.deleteLRU();
				pageMap.erase(deletedNode->page);
				LRUPageNode *newNode = pageList.insertAfterHead(page);
				pageMap[page] = newNode;
				printOutput(page, deletedNode->page);
			}
			else
			{
				LRUPageNode *newNode = pageList.insertAfterHead(page);
				pageMap[page] = newNode;
				printOutput(page, -1);
			}
		}
	}
};

// *********************************************
// *********************************************

// *********************************************
// NFU (Not Frequently Used)
// *********************************************
class NFUPageNode
{
public:
	int page;
	int frequency;
	NFUPageNode *prev;
	NFUPageNode *next;

	NFUPageNode()
	{
		this->page = -9;
		this->frequency = 1;
		this->prev = NULL;
		this->next = NULL;
	}

	NFUPageNode(int page, int frequency)
	{
		this->page = page;
		this->frequency = frequency;
		this->prev = NULL;
		this->next = NULL;
	}
};

class NFUPageLinkedList
{
public:
	NFUPageNode *head = new NFUPageNode();
	NFUPageNode *tail = new NFUPageNode();
	NFUPageLinkedList()
	{
		head->next = tail;
		head->prev = NULL;
		tail->prev = head;
		tail->next = NULL;
	}
	NFUPageNode *insertAfterHead(int page, int frequency)
	{
		NFUPageNode *newNode = new NFUPageNode(page, frequency);
		NFUPageNode *tempNode = head->next;
		head->next = newNode;
		newNode->prev = head;
		newNode->next = tempNode;
		tempNode->prev = newNode;
		return newNode;
	}

	void deleteNode(NFUPageNode *node)
	{
		NFUPageNode *previousNode = node->prev;
		NFUPageNode *nextNode = node->next;
		previousNode->next = nextNode;
		nextNode->prev = previousNode;

		delete (node);
	}

	NFUPageNode *deleteLRU()
	{
		NFUPageNode *node = tail->prev;
		NFUPageNode *previousNode = node->prev;
		previousNode->next = tail;
		tail->prev = previousNode;
		return node;
	}

	void print()
	{
		NFUPageNode *temp = head;
		while (temp != NULL)
		{
			cout << "(" << temp->page << ")";
			if (temp->next != NULL)
				cout << " -> ";
			temp = temp->next;
		}
		cout << "\n";
	}

	bool isEmpty()
	{
		if (head->next->next == NULL)
			return true;
		else
			return false;
	}
	void printList()
	{
		NFUPageNode *temp = head->next;
		while (temp->next != NULL)
		{
			cout << "|" << temp->page << "|";
			temp = temp->next;
		}
	}
};

class NFUPolicy
{
public:
	int frames = 0;
	int minFrequency;
	double hits = 0;
	double totalReference = 0;
	unordered_map<int, NFUPageLinkedList *> frequencyList;
	unordered_map<int, NFUPageNode *> pageMap;

	NFUPolicy(int frames)
	{
		this->frames = frames;
		minFrequency = 0;
	}
	double hitRate()
	{
		return hits / totalReference;
	}

	void printPages()
	{
		unordered_map<int, NFUPageLinkedList *>::iterator iter;
		for (iter = frequencyList.begin(); iter != frequencyList.end(); iter++)
		{
			iter->second->printList();
		}
	}

	void printOutput(int insertedPage, int removedPage)
	{
		string msg = to_string(insertedPage) + "-->";
		printGREEN(msg.c_str());
		printPages();
		if (removedPage != -1)
		{
			printRED("-->");
			msg = to_string(removedPage);
			printRED(msg.c_str());
		}

		cout << "\n";
	}

	void put(int page)
	{
		totalReference++;

		if (pageMap.find(page) != pageMap.end())
		{
			// key found
			NFUPageNode *deletedNode = pageMap[page];
			int deletedNodeKey = deletedNode->page;
			int deletedNodeFrequency = deletedNode->frequency;
			frequencyList[deletedNode->frequency]->deleteNode(deletedNode);

			if (deletedNodeFrequency == minFrequency && frequencyList[deletedNodeFrequency]->isEmpty())
			{
				// frequencyList.erase(deletedNodeFrequency);
				minFrequency = minFrequency + 1;
			}
			if (frequencyList.find(deletedNodeFrequency + 1) != frequencyList.end())
			{
				NFUPageLinkedList *cacheList = frequencyList[deletedNodeFrequency + 1];
				NFUPageNode *newNode = cacheList->insertAfterHead(page, deletedNodeFrequency + 1);
				pageMap[page] = newNode;
			}
			else
			{
				NFUPageLinkedList *cacheList = new NFUPageLinkedList();
				NFUPageNode *newNode = cacheList->insertAfterHead(page, deletedNodeFrequency + 1);
				frequencyList[deletedNodeFrequency + 1] = cacheList;
				pageMap[page] = newNode;
			}
			hits++;
			printOutput(page, -1);
		}
		else
		{
			if (frames == 0)
				return;
			if (frames <= pageMap.size())
			{
				NFUPageNode *deletedNode = frequencyList[minFrequency]->deleteLRU();
				pageMap.erase(deletedNode->page);
				minFrequency = 1;
				NFUPageLinkedList *cacheList = frequencyList[minFrequency];
				pageMap[page] = cacheList->insertAfterHead(page, 1);
				printOutput(page, deletedNode->page);
			}
			else
			{
				minFrequency = 1;
				if (frequencyList.find(minFrequency) != frequencyList.end())
				{
					NFUPageLinkedList *list = frequencyList[1];
					NFUPageNode *newNode = list->insertAfterHead(page, minFrequency);
					pageMap[page] = newNode;
				}
				else
				{
					NFUPageLinkedList *cacheList = new NFUPageLinkedList();
					NFUPageNode *newNode = cacheList->insertAfterHead(page, minFrequency);
					frequencyList[minFrequency] = cacheList;
					pageMap[page] = newNode;
				}
				printOutput(page, -1);
			}
		}
	}

	// void printFrequencyList()
	// {
	// 	cout << "FREQUENCY LIST \n";
	// 	unordered_map<int, NFUPageLinkedList *>::iterator iter;
	// 	for (iter = frequencyList.begin(); iter != frequencyList.end(); iter++)
	// 	{
	// 		cout << iter->first << " : ";
	// 		NFUPageLinkedList *tempList = (iter->second);
	// 		tempList->print();
	// 		cout << "\n";
	// 	}
	// }

	// void print()
	// {
	// 	printFrequencyList();
	// 	cout << "------------------------------------\n";
	// 	cout << "MINIMUM FREQUENCY : " << minFrequency << endl;
	// 	cout << "------------------------------------\n";
	// }
};

// *********************************************
// *********************************************

// *********************************************
// NRU (Not Recently Used)
// *********************************************

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
		string msg = to_string(insertedPage) + "-->";
		printGREEN(msg.c_str());
		printPages();
		if (removedPage != -1)
		{
			printRED("-->");
			msg = to_string(removedPage);
			printRED(msg.c_str());
		}

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

// *********************************************
// *********************************************

// If page found, updates the second chance bit to true
static bool fifo_search(int pg, int arr[], int frames)
{
	int i;
	for (i = 0; i < frames; i++)
	{
		if (arr[i] == pg)
		{
			return true;
		}
	}
	q.push(pg);
	return false;
}

double fifo_printHitsAndFaults(string reference_string, int frames)
{
	int pointer, i, l = 0, pg;
	float pf;
	float ph;

	// initially we consider frame 0 is to be replaced
	int pos = 0;

	// number of page faults
	pf = 0;
	ph = 0;

	// Create a array to hold page numbers
	int arr[frames];

	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));

	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l] = word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
	int replace_pg;
	int index;
	// l=the length of array

	for (i = 0; i < l; i++)
	{
		pg = stoi(str[i]);

		// Finds if there exists a need to replace
		// any page at all
		cout << "\033[0;32;1m" << pg << "--->"
			 << "\033[0m";

		for (int i = 0; i < frames; i++)
			cout << "|" << arr[i];

		cout << "|";
		if (!fifo_search(pg, arr, frames))
		{
			if (pos < frames)
			{
				arr[pos] = pg;
				pos++;
				cout << endl;
			}
			else
			{
				replace_pg = q.front();
				q.pop();
				cout << "\033[0;31;1m"
					 << "--->" << replace_pg << "\033[0m" << endl;

				for (int i = 0; i < frames; i++)
				{
					if (arr[i] == replace_pg)
					{
						index = i;
						break;
					}
				}
				arr[index] = pg;
			}

			// Update page faults
			pf++;
		}
		else
		{
			cout << endl;
			ph++;
		}
	}
	// cout << "Total page faults = " << pf << "\n";
	// cout << "Total page hits = " << ph << "\n";
	double r = ph / (ph + pf);
	// cout << "Hit Rate : " << r << endl;
	return r;
}

double fifo_printHitsAndFaults_2(string reference_string, int frames)
{
	int pointer, i, l = 0, pg;
	float pf;
	float ph;

	// initially we consider frame 0 is to be replaced
	int pos = 0;

	// number of page faults
	pf = 0;
	ph = 0;

	// Create a array to hold page numbers
	int arr[frames];

	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));

	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l] = word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
	int replace_pg;
	int index;
	// l=the length of array

	for (i = 0; i < l; i++)
	{
		pg = stoi(str[i]);

		// Finds if there exists a need to replace
		// any page at all
		cout << "\033[0;32;1m" << pg << "--->"
			 << "\033[0m";

		for (int i = 0; i < frames; i++)
			cout << "|" << arr[i];

		cout << "|";
		if (!fifo_search(pg, arr, frames))
		{
			if (pos < frames)
			{
				arr[pos] = pg;
				pos++;
				cout << endl;
			}
			else
			{
				replace_pg = q.front();
				q.pop();
				cout << "\033[0;31;1m"
					 << "--->" << replace_pg << "\033[0m" << endl;

				for (int i = 0; i < frames; i++)
				{
					if (arr[i] == replace_pg)
					{
						index = i;
						break;
					}
				}
				arr[index] = pg;
			}

			// Update page faults
			pf++;
		}
		else
		{
			cout << endl;
			ph++;
		}
	}
	// cout << "Total page faults = " << pf << "\n";
	// cout << "Total page hits = " << ph << "\n";
	double r = ph / (ph + pf);
	// cout << "Hit Rate : " << r << endl;
	return r;
}

// // If page found, updates the second chance bit to true
static bool fifo_2nd_searchAndUpdate(int pg, int arr[], bool second_chance[], int frames)
{
	int i;
	for (i = 0; i < frames; i++)
	{
		if (arr[i] == pg)
		{
			second_chance[i] = true;
			return true;
		}
	}

	// Return 'false' so that a page for replacement is selected
	// as he reuested page doesn't exist in memory
	//	cout<<"FAULT"<<endl;
	q2.push(pg);
	return false;
}

double fifo_2nd_printHitsAndFaults(string reference_string,
								   int frames)
{
	int pointer, i, l = 0, pg;
	float pf, ph;
	// initially we consider frame 0 is to be replaced
	int pos = 0;
	int replace_pg;
	int index;

	// number of page faults
	pf = 0;
	ph = 0;

	// Create a array to hold page numbers
	int arr[frames];

	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));

	// Create second chance array.
	// Can also be a byte array for optimizing memory
	bool second_chance[frames];

	// Split the string into tokens,
	// that is page numbers, based on space

	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l] = word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
	// l=the length of array

	for (i = 0; i < l; i++)
	{
		pg = stoi(str[i]);

		// Finds if there exists a need to replace
		// any page at all
		// cout<<pg<<"---->";
		cout << "\033[0;32;1m" << pg << "--->"
			 << "\033[0m";

		for (int i = 0; i < frames; i++)
			cout << "|" << arr[i];

		cout << "|";
		if (!fifo_2nd_searchAndUpdate(pg, arr, second_chance, frames))
		{
			if (pos < frames)
			{
				arr[pos] = pg;
				pos++;
				cout << endl;
			}
			else
			{
				replace_pg = q2.front();

				for (int i = 0; i < frames; i++)
				{
					if (arr[i] == replace_pg)
					{
						index = i;
						break;
					}
				}
				while (true)
				{
					// We found the page to replace
					if (!second_chance[index])
					{
						arr[index] = pg;
						q2.pop();
						cout << "\033[0;31;1m"
							 << "--->" << replace_pg << "\033[0m" << endl;
						// cout<<"----->"<<replace_pg<<endl;
						break;
					}
					else
					{
						// Mark it 'false' as it got one chance
						second_chance[index] = false;
						q2.pop();
						q2.push(replace_pg);

						replace_pg = q2.front();

						for (int i = 0; i < frames; i++)
						{
							if (arr[i] == replace_pg)
							{
								index = i;
								break;
							}
						}
					}
				}
			}

			// Update page faults
			pf++;
		}

		else
		{
			cout << endl;
			ph++;
		}
	}
	// cout << "Total page faults = " << pf << "\n";
	// cout << "Total page hits = " << ph << "\n";
	double r = ph / (pf + ph);
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed);
	// cout << "Hit Rate : " << r << endl;
	return r;
}

double fifo_2nd_printHitsAndFaults_2(string reference_string,
								   int frames)
{
	int pointer, i, l = 0, pg;
	float pf, ph;
	// initially we consider frame 0 is to be replaced
	int pos = 0;
	int replace_pg;
	int index;

	// number of page faults
	pf = 0;
	ph = 0;

	// Create a array to hold page numbers
	int arr[frames];

	// No pages initially in frame,
	// which is indicated by -1
	memset(arr, -1, sizeof(arr));

	// Create second chance array.
	// Can also be a byte array for optimizing memory
	bool second_chance[frames];

	// Split the string into tokens,
	// that is page numbers, based on space

	string str[100];
	string word = "";
	for (auto pg : reference_string)
	{
		if (pg == ' ')
		{
			str[l] = word;
			word = "";
			l++;
		}
		else
		{
			word = word + pg;
		}
	}
	str[l] = word;
	l++;
	// l=the length of array

	for (i = 0; i < l; i++)
	{
		pg = stoi(str[i]);

		// Finds if there exists a need to replace
		// any page at all
		// cout<<pg<<"---->";
		cout << "\033[0;32;1m" << pg << "--->"
			 << "\033[0m";

		for (int i = 0; i < frames; i++)
			cout << "|" << arr[i];

		cout << "|";
		if (!fifo_2nd_searchAndUpdate(pg, arr, second_chance, frames))
		{
			if (pos < frames)
			{
				arr[pos] = pg;
				pos++;
				cout << endl;
			}
			else
			{
				replace_pg = q2.front();

				for (int i = 0; i < frames; i++)
				{
					if (arr[i] == replace_pg)
					{
						index = i;
						break;
					}
				}
				while (true)
				{
					// We found the page to replace
					if (!second_chance[index])
					{
						arr[index] = pg;
						q2.pop();
						cout << "\033[0;31;1m"
							 << "--->" << replace_pg << "\033[0m" << endl;
						// cout<<"----->"<<replace_pg<<endl;
						break;
					}
					else
					{
						// Mark it 'false' as it got one chance
						second_chance[index] = false;
						q2.pop();
						q2.push(replace_pg);

						replace_pg = q2.front();

						for (int i = 0; i < frames; i++)
						{
							if (arr[i] == replace_pg)
							{
								index = i;
								break;
							}
						}
					}
				}
			}

			// Update page faults
			pf++;
		}

		else
		{
			cout << endl;
			ph++;
		}
	}
	// cout << "Total page faults = " << pf << "\n";
	// cout << "Total page hits = " << ph << "\n";
	double r = ph / (pf + ph);
	std::cout.precision(2);
	std::cout.setf(std::ios::fixed);
	// cout << "Hit Rate : " << r << endl;
	return r;
}

bool optimal_search(int key, int fr[], int fn)
{
	for (int i = 0; i < fn; i++)
		if (fr[i] == key)
			return true;
	return false;
}

// Function to find the frame that will not be used
// recently in future after given index in pg[0..pn-1]
int predict(string pg[], int fr[], int pn, int index, int fn)
{
	// Store the index of pages which are going
	// to be used recently in future
	int res = -1, farthest = index;
	int i = 0;
	while (i < fn)
	{
		int j;
		j = index;
		while (j < pn)
		{
			if (fr[i] == stoi(pg[j]))
			{
				if (j > farthest)
				{
					farthest = j;
					res = i;
				}
				break;
			}
			j++;
		}

		// If a page is never referenced in future,
		// return it.
		if (j == pn)
			return i;

		i++;
	}
	// If all of the frames were not in future,
	// return any of them, we return 0. Otherwise
	// we return res.
	return (res == -1) ? 0 : res;
}

double optimalPage(string reference_string, int fn)
{
	// cout << "|" << reference_string << "|\n";
	// cout << fn;
	// Create an array for given number of
	// frames and initialize it as empty.
	// vector<int> fr;
	int fr[fn];

	string str[100];
	string word = "";
	int l = 0;
	int pos = 0;
	memset(fr, -1, sizeof(fr));

	for (auto x : reference_string)
	{
		if (x == ' ')
		{
			str[l] = word;
			word = "";
			l++;
		}
		else
		{
			word = word + x;
		}
	}
	str[l] = word;
	l++;

	// Traverse through page reference array
	// and check for miss and hit.
	float ph = 0;
	float pf = 0;
	for (int i = 0; i < l; i++)
	{

		int x = stoi(str[i]);
		cout << "\033[0;32;1m" << x << "--->"
			 << "\033[0m";
		// cout<<x<<"---->";

		for (int i = 0; i < fn; i++)
		{
			if (fr[i] != -1)
				cout << "|" << fr[i];
		}

		cout << "|";
		// Page found in a frame : HIT
		if (optimal_search(x, fr, fn))
		{

			ph++;
			cout << endl;
			continue;
		}

		// Page not found in a frame : MISS

		// If there is space available in frames.
		if (pos < fn)
		{
			// cout<<"FAULT"<<endl;
			pf++;
			fr[pos] = x;
			pos++;
			// fr.push_back(x);
			cout << endl;
		}

		// Find the page to be replaced.
		else
		{
			pf++;
			// cout<<"FAULT"<<endl;
			int j = predict(str, fr, l, i + 1, fn);
			// cout<<"Replace "<<fr[j]<<" with new page "<<x<<endl;
			cout << "\033[0;31;1m"
				 << "--->" << fr[j] << "\033[0m" << endl;
			// cout<<"----->"<<fr[j]<<endl;
			fr[j] = x;
		}
	}
	// cout << "No. of hits = " << ph << endl;
	// cout << "Total page faults were = " << pf << endl;
	double r = ph / (ph + pf);
	// cout << "Hit Rate : " << r << endl;
	return r;
}

// *********************************************
// AGING
// *********************************************

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

	void printPages()
	{
		cout << "|";
		for (auto page : vec)
		{
			cout << page.first << "|";
		}
	}

	void printOutput(int insertedPage, int removedPage)
	{
		string msg = to_string(insertedPage) + "-->";
		printGREEN(msg.c_str());
		printPages();
		if (removedPage != -1)
		{
			printRED("-->");
			msg = to_string(removedPage);
			printRED(msg.c_str());
		}

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
				printOutput(entry, -1);
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
						ex = it->first;
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
						ex = it->first;
						vec.erase(it);
					}
				}
				printOutput(entry, ex);
			}
		}
	}
};

// *********************************************
// *********************************************
void writeToFile(vector<double> &vec)
{
	ofstream fp;
	fp.open("hitRate.txt");
	if (!fp)
	{
		cout << "Unable to open file\n";
	}

	for (auto it : vec)
	{
		fp << it << ",";
	}

	fp.close();
}

void simulateAllPolicies()
{
	int frames;
	string input = "";
	double hitRate = 0;
	PageReplacementScheme *pr;
	vector<int> pageSequence;
	takeInput(frames, input);
	tokenize(input, pageSequence);
	vector<double> hitRates;

	// 1.Random
	cout << "Simulating Random\n";
	RandomPolicy randomPolicy(frames);
	for (auto page : pageSequence)
	{
		randomPolicy.put(page);
	}
	cout << " Hit Rate : " << randomPolicy.hitRate() << "\n";
	hitRates.push_back(randomPolicy.hitRate());

	// 2.Optimal
	cout << "Simulating Optimal\n";
	hitRate = optimalPage(input, frames);
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	// 3. NRU
	cout << "Simulating NRU (Not Recently Used)\n";
	NRUPolicy nru(frames);
	for (auto page : pageSequence)
	{
		nru.put(page);
	}
	cout << "Hit Rate : " << nru.hitRate() << "\n";
	hitRates.push_back(nru.hitRate());

	// 4. FIFO
	cout << "Simulating FIFO\n";
	//queue<int> empty;
	q=queue<int>();
	//q.swap(queue<int>());
   	//std::swap( q, empty );
	hitRate = fifo_printHitsAndFaults_2(input, frames);
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	// 5. FIFO with 2nd chance
	cout << "Simulating FIFO with second chance\n";
	q2=queue<int>();
    //std:swap( q2, empty );
	hitRate = fifo_2nd_printHitsAndFaults_2(input, frames);
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	// 6. Clock
	pr = new Clock();
	pr->simulatePageReplacement(pageSequence, frames);
	hitRate = pr->getHitRate();
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	// 7. NRU
	cout << "Simulating LRU(Least Recently Used)\n";
	LRUPolicy lru(frames);
	for (auto page : pageSequence)
	{
		lru.put(page);
	}
	cout << " Hit Rate : " << lru.hitRate() << "\n";
	hitRates.push_back(lru.hitRate());

	// 8. NFU
	cout << "Simulating NFU(Not Frequently Used)\n";
	NRUPolicy nfu(frames);
	for (auto page : pageSequence)
	{
		nfu.put(page);
	}
	cout << " Hit Rate : " << nfu.hitRate() << "\n";
	hitRates.push_back(nfu.hitRate());

	// 9. Working Set
	pr = new WS();
	pr->simulatePageReplacement(pageSequence, frames);
	hitRate = pr->getHitRate();
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	// 10. Aging
	cout << "Simulating Aging\n";
	Aging aging(frames);
	for (auto page : pageSequence)
	{
		aging.put(page);
	}
	cout << "Hit Rate : " << aging.hitRate() << "\n";
	hitRates.push_back(aging.hitRate());

	// 11. WorkingSet Clock
	pr = new WSClock();
	pr->simulatePageReplacement(pageSequence, frames);
	hitRate = pr->getHitRate();
	cout << "Hit Rate : " << hitRate << "\n";
	hitRates.push_back(hitRate);

	writeToFile(hitRates);
}

int system(const char *command);

int main(int argc, const char *argv[])
{
	int frames;
	string input = "";
	int choice;
	int algoChoice;
	do
	{
		printMenu();
		cout << "Enter choice : ";
		cin >> choice;
		if (choice == 0)
			break;

		if (choice == 2)
		{
			simulateAllPolicies();
			system("python3 plotGraph.py");
		}

		if (choice == 1)
		{
			printAlgoMenu();
			cout << "Enter the choice for algorithm : ";
			cin >> algoChoice;
			PageReplacementScheme *pr;

			switch (algoChoice)
			{
			case 1:
			{
				// Random
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);

				RandomPolicy randomPolicy(frames);
				for (auto page : pageSequence)
				{
					randomPolicy.put(page);
				}
				cout << " Hit Rate : " << randomPolicy.hitRate() << "\n";
			}
			break;
			case 2:
			{
				// Optimal
				takeInput(frames, input);
				double hitRate = optimalPage(input, frames);
				cout << "Hit Rate : " << hitRate << "\n";
			}
			break;
			case 3:
			{
				// NRU (Not recently used)
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);

				NRUPolicy nru(frames);
				for (auto page : pageSequence)
				{
					nru.put(page);
				}
				cout << " Hit Rate : " << nru.hitRate() << "\n";
			}
			break;
			case 4:
			{
				// FIFO
				takeInput(frames, input);
				q=queue<int>();
				double hitRate = fifo_printHitsAndFaults(input, frames);
				cout << "Hit Rate : " << hitRate << "\n";
				
				
			}
			break;
			case 5:
			{
				// FIFO with second chance
				takeInput(frames, input);
				q2=queue<int>();
				double hitRate = fifo_2nd_printHitsAndFaults(input, frames);
				cout << "Hit Rate : " << hitRate << "\n";
			}
			break;
			case 6:
			{
				// // Clock
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);
				pr = new Clock();
				pr->simulatePageReplacement(pageSequence, frames);
				cout << "Hit Rate : " << pr->getHitRate() << "\n";
			}
			break;
			case 7:
			{
				// LRU (Least Recently Used)
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);

				LRUPolicy lru(frames);
				for (auto page : pageSequence)
				{
					lru.put(page);
				}
				cout << " Hit Rate : " << lru.hitRate() << "\n";
			}
			break;
			case 8:
			{
				// NFU (Not Frequently Used)
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);

				NFUPolicy nfu(frames);
				for (auto page : pageSequence)
				{
					nfu.put(page);
				}
				cout << " Hit Rate : " << nfu.hitRate() << "\n";
			}
			break;
			case 9:
			{
				// Working set
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);
				pr = new WS();
				pr->simulatePageReplacement(pageSequence, frames);
				cout << "Hit Rate : " << pr->getHitRate() << "\n";
			}
			break;
			case 10:
			{
				// Aging
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);

				Aging aging(frames);
				for (auto page : pageSequence)
				{
					aging.put(page);
				}
				cout << " Hit Rate : " << aging.hitRate() << "\n";
			}
			break;
			case 11:
			{
				// WorkingSet Clock
				vector<int> pageSequence;
				takeInput(frames, input);
				tokenize(input, pageSequence);
				pr = new WSClock();
				pr->simulatePageReplacement(pageSequence, frames);
				cout << "Hit Rate : " << pr->getHitRate() << "\n";
			}
			break;
			default:
			{
				cout << "Enter the choice from the menu.\n";
			}
			}
		}
	} while (choice != 0);
}
