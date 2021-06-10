#include <iostream>
#include <ctime>
#include <cstring>
#include <unordered_map>
#include <cmath>
#define LI long long
using namespace std;

struct Data {
	string name;
	int number;
	int cvv2;

	Data()
	{
		name = "abcdef"[rand() % 6];
		number = rand() % 9999 + 1000;
		cvv2 = rand() % 9999 + 1000;
	}

	Data(string name1, int number1, int cvv2_1)
	{
		name = name1;
		number = number1;
		cvv2 = cvv2_1;
	}
};

struct HashNode {
	Data data;
	HashNode* next, *prev;
	string key;
	HashNode() { next = prev = NULL; }
	HashNode(string key1, Data data1, HashNode *tail) { data = data1; key = key1; next = NULL; prev = tail; }
};

struct LinkedList
{
	HashNode *head, *tail;

	LinkedList() { head = tail = NULL; }

	void push_back(Data data, string key)
	{
		HashNode *temp = new HashNode(key, data, tail);

		if (head == NULL)
		{
			tail = head = temp;
		}
		else {
			tail->next = temp;
			tail = temp;
		}
	}

	Data deleteNode(string key)
	{
		HashNode *nodeDelete = searchNode(key);
		if (nodeDelete == NULL) return{ " ",0,0 };

		Data dataDelete = nodeDelete->data;

		if (nodeDelete == head) {
			if (head->next) head->next->prev = NULL;
			head = head->next;
		}

		else if (nodeDelete == tail) {
			tail->prev->next = NULL;
			tail = tail->prev;
		}

		else {
			nodeDelete->prev->next = nodeDelete->next;
			nodeDelete->next->prev = nodeDelete->prev;
		}

		delete nodeDelete;
		return dataDelete;
	}

	HashNode *searchNode(string key)
	{
		HashNode *current = head;
		while (current != NULL)
		{
			if (current->key == key) break;
			current = current->next;
		}
		return current;
	}
};

struct HashTable
{
	LinkedList *bucketsArray;
	int actual_size, buckets;
	float loadFactor = 0.8f;

	LI hash(string key) { 
		unsigned int value = 0;
		for (int i = 0; i < key.length(); i++)
			value = 37 * value + key[i];
		LI last = value % buckets;
		return last;
	}

	HashTable(int size_buckets = 60) // start default size
	{
		actual_size = 0;
		buckets = size_buckets;
		bucketsArray = new LinkedList[buckets];
	}

	int size() { return actual_size; }

	void insert(string key, Data data)
	{
		LI hash_key = hash(key);
		HashNode *insert_data = bucketsArray[hash_key].searchNode(key);

		if (insert_data) // not NULL
		{
			insert_data->data = data;
			return;
		}

		bucketsArray[hash_key].push_back(data, key);
		actual_size++;
		if (actual_size / buckets >= loadFactor) { resize(); }
	}

	HashNode *find(string key)
	{
		LI hash_key = hash(key);
		HashNode *node = bucketsArray[hash_key].searchNode(key);
		if (node) { return node; }
		return NULL;
	}

	Data erase(string key)
	{
		LI hash_key = hash(key);
		Data delete_data = bucketsArray[hash_key].deleteNode(key);
		if (delete_data.number != 0) actual_size--;
		return delete_data;
	}

	void resize()
	{
		LinkedList *temp = bucketsArray;
		buckets *= 2;
		bucketsArray = new LinkedList[buckets];
		actual_size = 0;
		resizeLoop(buckets / 2, temp);
	}

	void resizeLoop(int tempSize, LinkedList *temp) {
		for (int i = 0; i < tempSize; i++)
		{
			if (temp[i].head != NULL)
			{
				HashNode *hash = temp[i].head;
				while (hash != NULL)
				{
					insert(hash->key, hash->data);
					hash = hash->next;
				}
			}
		}
		delete[] temp;
	}

	~HashTable() {
		HashNode *prev = NULL;
		for (int i = 0; i < buckets; i++)
		{
			if (bucketsArray[i].head != NULL)
			{
				HashNode *current = bucketsArray[i].head;
				while (current != NULL)
				{
					prev = current;
					current = current->next;
					delete prev;
				}
			}
		}
		delete[] bucketsArray;
	}
};

LI getLongRandom() {
	return rand() % 99999999 + 10000000;
}
string getString() 
{
	string str, str1;
	for (int i = 0; i < 16; i++)
	{
		str1 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"[rand() % 61];
		str.append(str1);
	}
	return str;
}

bool testHashTable()
{
	const int iters = 5000;
	const int keysAmount = iters * 1;

	// generate random keys:
	auto* keys = new string[keysAmount];

	auto* keysToInsert = new string[iters];
	auto* keysToErase = new string[iters];
	auto* keysToFind = new string[iters];

	for (int i = 0; i < keysAmount; i++)
	{
		keys[i] = getString();
	}
	for (int i = 0; i < iters; i++)
	{
		keysToInsert[i] = keys[getLongRandom() % keysAmount];
		keysToErase[i] = keys[getLongRandom() % keysAmount];
		keysToFind[i] = keys[getLongRandom() % keysAmount];
	}

	// test my HashTable:
	HashTable hashTable;

	clock_t myStart = clock();
	for (int i = 0; i < iters; i++)
	{
		hashTable.insert(keysToInsert[i], Data());
	}
	int myInsertSize = hashTable.size();
	for (int i = 0; i < iters; i++)
	{
		hashTable.erase(keysToErase[i]);
	}
	int myEraseSize = hashTable.size();
	int myFoundAmount = 0;
	for (int i = 0; i < iters; i++)
	{
		if (hashTable.find(keysToFind[i]) != nullptr)
		{
			myFoundAmount++;
		}
	}
	clock_t myEnd = clock();
	float myTime = (float(myEnd - myStart)) / CLOCKS_PER_SEC;

	// test STL hash table:
	unordered_map<string, Data> unorderedMap;

	clock_t stlStart = clock();
	for (int i = 0; i < iters; i++)
	{
		unorderedMap.insert({ keysToInsert[i], Data() });
	}
	int stlInsertSize = unorderedMap.size();
	for (int i = 0; i < iters; i++)
	{
		unorderedMap.erase(keysToErase[i]);
	}
	int stlEraseSize = unorderedMap.size();
	int stlFoundAmount = 0;
	for (int i = 0; i < iters; i++)
	{
		if (unorderedMap.find(keysToFind[i]) != unorderedMap.end())
		{
			stlFoundAmount++;
		}
	}
	clock_t stlEnd = clock();
	float stlTime = (float(stlEnd - stlStart)) / CLOCKS_PER_SEC;

	cout << "My HashTable:" << endl;
	cout << "Time: " << myTime << ", size: " << myInsertSize << " - " << myEraseSize << ", found amount: " << myFoundAmount << endl;
	cout << "STL unordered_map:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlInsertSize << " - " << stlEraseSize << ", found amount: " << stlFoundAmount << endl << endl;

	delete[] keys;
	delete[] keysToInsert;
	delete[] keysToErase;
	delete[] keysToFind;

	if (myInsertSize == stlInsertSize && myEraseSize == stlEraseSize && myFoundAmount == stlFoundAmount)
	{
		cout << "The lab is completed" << endl;
		return true;
	}

	cerr << ":(" << endl;
	return false;
}

int main() {
	srand(time(NULL));
	testHashTable();
	system("pause");
}