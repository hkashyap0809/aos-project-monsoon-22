// NFU.cpp
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

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
	void printList(){
		NFUPageNode *temp = head->next;
		while(temp->next != NULL){
			cout<<"|"<<temp->page<<"|";
			temp=temp->next;
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
	double hitRate(){
		return hits/totalReference;
	}
	
	void printPages(){
		unordered_map<int,NFUPageLinkedList*> ::iterator iter;
		for(iter = frequencyList.begin(); iter != frequencyList.end(); iter++){
			iter->second->printList();
		}
	}
	
	void printOutput(int insertedPage, int removedPage)
	{
		cout << insertedPage << "-->";
		printPages();
		if (removedPage != -1)
			cout << "-->" << removedPage;
		
		cout<<"\n";
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
			printOutput(page,-1);
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
				printOutput(page,deletedNode->page);
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
				printOutput(page,-1);
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

void takeInput(int &frames, string &input)
{
	cout << "Enter the number of frames : ";
	cin >> frames;
	cin.ignore();
	cout<<"\n";
	cout << "Enter the page sequence : ";
	getline(cin, input);
}


int main()
{
	int frames;
	string input;
	vector<int> pageSequence;


	takeInput(frames,input);
	tokenize(input, pageSequence);


	NFUPolicy nfu(frames);

	for (auto page : pageSequence)
	{
		nfu.put(page);
	}

	cout<<"HIT RATE : "<<nfu.hitRate()<<"\n";
	
}
