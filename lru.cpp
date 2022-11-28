#include <iostream>
#include <bits/stdc++.h>
using namespace std;
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
		cout << insertedPage << "-->";
		pageList.printList();
		if (removedPage != -1)
			cout << "-->" << removedPage;

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


int main()
{
	int frames;
	string input;
	vector<int> pageSequence;


	cout << "Enter the number of frames : ";
	cin >> frames;
	cout << "Enter the page sequence : ";
	cin.ignore();
	getline(cin, input);
	tokenize(input, pageSequence);

	LRUPolicy lru(frames);

	for (auto page : pageSequence)
	{
		lru.put(page);
	}
	cout << lru.hits << "\n";
	cout << lru.totalReference << "\n";
	cout << lru.hitRate();

	return 0;
}
