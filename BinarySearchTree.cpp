#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <set>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
#include <string>
using namespace std;

struct Data // Player
{
	string nickname;
	int rank;
	int donate;

	Data(string nickname, int rank, int donate)
	{
		this->nickname = nickname;
		this->donate = donate;
		this->rank = rank;
	}

	Data()
	{
		this->nickname = "abcdef"[rand() % 6];
		this->donate = rand() % 9999 + 1000;
		this->rank = rand() % 100 + 1;
	}
};

bool operator<(Data const &a, Data const &b)
{
	if (a.nickname != b.nickname) {
		return a.nickname < b.nickname;
	}
	else if (a.rank != b.rank) {
		return a.rank < b.rank;
	}
	else
	{
		return a.donate < b.donate;
	}
}

bool operator>(Data const &a, Data const &b)
{
	if (a.nickname != b.nickname)
	{
		return a.nickname > b.nickname;
	}
	else if (a.rank != b.rank) {
		return a.rank > b.rank;
	}
	else return a.donate > b.donate;
}

bool operator==(Data const &a, Data const &b) {
	return a.donate == b.donate && a.nickname == b.nickname &&
		a.rank == b.rank;
}

bool operator<=(Data const &c1, Data const &c2)
{
	return max((c1 < c2), (c1 == c2));
}

bool operator>=(Data const &c1, Data const &c2)
{
	return max((c1 > c2), (c1 == c2));
}

struct Node {
	Data data;
	Node *left;
	Node *right;

	Node(Data data) {
		this->data = data;
		left = right = nullptr;
	}
};

struct BinarySearchTree {

	Node *head;
	int capacity;

	BinarySearchTree() {
		head = nullptr;
		capacity = 0;
	}

	int size() const { return capacity; }

	bool innerInsert(Node *temp, Data &data) { // наша внутрішня реалізація , яка приймає два параметри
		if (temp->data == data) {// якщо дані повторюються з попереднім елементом
			return false; // і повертаємо різні значення true або false, відповідно , якщо вийшло додати елемент чи ні
		}

		else if (data < temp->data) {// якщо вхідні дані меньше
			if (!temp->left) {
				temp->left = new Node(data);
				capacity++;
				return true;
			}
			return innerInsert(temp->left, data);
		} 
		else if (data > temp->data) { // якщо вхідні дані більше
			if (!temp->right) {
				temp->right = new Node(data);
				capacity++;
				return true;
			}
			return innerInsert(temp->right, data);
		}

		return false; // якщо жодна з операцій не була виконана
	}

	void insert(Data &data) {
		if (!head) {
			head = new Node(data);
			capacity++;
		}else{
			innerInsert(head, data);
		}
		
	}

	int innerHeight(Node *temp) {
		if (temp != nullptr) return 0;
		else {
			int l_height = innerHeight(temp->left);
			int r_height = innerHeight(temp->right);
			if (l_height > r_height)
			{
				return (l_height + 1);
			}
			else {
				return (r_height + 1);
			}
		}
	}

	int height() {
		return innerHeight(head);
	}

	Node *innerFind(Node *temp, Data &data) {
		if (temp == nullptr || temp->data == data)
		{
			return temp;
		}
		return data < temp->data ? innerFind(temp->left, data) : innerFind(temp->right, data);
	}

	bool find(Data &data) {
		return innerFind(head, data);
	}

	Node *innerErase(Node *root, Data &data, bool &deleted) {
		if (root == nullptr) return root;
		else if (data < root->data) {
			root->left = innerErase(root->left, data, deleted);
		}
		else if (data > root->data) {
			root->right = innerErase(root->right, data, deleted);
		}
		else {
			// у нас немає жодних дітей
			if (root->left == nullptr && root->right == nullptr) {
				root = nullptr;
			}
			//у нас є 1 дитя
			else if (root->left == nullptr) {
				Node *temp = root;
				root = root->right;
				delete temp;// удаляем указатель и освобождаем память
			}
			else if (root->right == nullptr) {
				Node *temp = root;
				root = root->left;
				delete temp; // удаляем указатель и освобождаем память
			}
			else {
				//у нас є 2 дитя
				Node *temp = minimalValueNode(root->right);
				root->data = temp->data;
				root->right = innerErase(root->right, temp->data, deleted);
			}
			deleted = true;
		}
		return root;
	}

	void erase(Data &data) {
		bool success = false;
		innerErase(head, data, success);
		if (success) {
			capacity--;
		}
	}

	Node *minimalValueNode(Node *temp) {  // функція для знаходження самого лівого елемента , тобто найменшого
		while (temp->left)
			temp = temp->left;
		return temp;
	}

	void print() {
		cout << "Head: " << "(" << head->data.donate << ", " << head->data.nickname << ", "
		<< head->data.rank << ")\nInorder:\n";
		inorder(head);
		cout << endl << endl;
	}

	void inorder(Node *temp) {
		if (temp != nullptr) {
			inorder(temp->left);
			cout << "Element: " << "(" << temp->data.donate << ", " << temp->data.nickname << ", "
				<< temp->data.rank << ")\n";
			inorder(temp->right);
		}
	}

	void innerFindInRange(Node *temp, Data min, Data max, vector<Data> &finds) {
		if (temp == nullptr) return;
		if (min < temp->data) {
			innerFindInRange(temp->left, min, max, finds);
		}

		if (max > temp->data) { 
			innerFindInRange(temp->right, min, max, finds);
		}

		if (min <= temp->data && max >= temp->data) {
			finds.push_back(temp->data);
		}
	}

	vector<Data> findInRange(Data minObject, Data maxObject) {
		vector<Data> finds;
		innerFindInRange(head, minObject, maxObject, finds);
		return finds;
	}
};

long generateRandLong() {
	long int randMax = 30000000;
	long int randMin = 1000000;
	return rand() % (randMax - randMin) + randMin;
}

bool testBinarySearchTree() {
	srand(time(nullptr));
	const int iters = 8000;
	const int keysAmount = iters * 2;
	const int itersToRangeQueries = 1000;
	vector<Data> data(keysAmount);
	vector<Data> dataToInsert(iters);
	vector<Data> dataToErase(iters);
	vector<Data> dataToFind(iters);
	vector<pair<Data, Data>>
		dataToRangeQueries;
	for (int i = 0; i < iters; i++) {
		dataToInsert[i] = data[generateRandLong() % keysAmount];
		dataToErase[i] = data[generateRandLong() % keysAmount];
		dataToFind[i] = data[generateRandLong() % keysAmount];
	}
	for (int i = 0; i < itersToRangeQueries; i++) {
		Data minData = Data();
		Data maxData = Data();
		if (maxData < minData) {
			swap(minData, maxData);
		}
		dataToRangeQueries.push_back({ minData, maxData });
	}
	BinarySearchTree myTree;
	clock_t myStart = clock();
	for (int i = 0; i < iters; i++) {
		myTree.insert(dataToInsert[i]);
	}
	int myInsertSize = myTree.size();
	int myTreeHeight = myTree.height();
	int optimalTreeHeight = log2(myInsertSize) + 1;
	for (int i = 0; i < iters; i++) {
		myTree.erase(dataToErase[i]);
	}
	int myEraseSize = myTree.size();
	int myFoundAmount = 0;
	for (int i = 0; i < iters; i++) {
		if (myTree.find(dataToFind[i])) {
			myFoundAmount++;
		}
	}
	clock_t myEnd = clock();
	float myTime = (float(myEnd - myStart)) / CLOCKS_PER_SEC;
	set<Data> stlTree;
	clock_t stlStart = clock();
	for (int i = 0; i < iters; i++) {
		stlTree.insert(dataToInsert[i]);
	}
	int stlInsertSize = stlTree.size();
	for (int i = 0; i < iters; i++) {
		stlTree.erase(dataToErase[i]);
	}
	int stlEraseSize = stlTree.size();
	int stlFoundAmount = 0;
	for (int i = 0; i < iters; i++) {
		if (stlTree.find(dataToFind[i]) != stlTree.end())
			stlFoundAmount++;
	}


	clock_t stlEnd = clock();
	float stlTime = (float(stlEnd - stlStart)) / CLOCKS_PER_SEC;
	clock_t myRangeStart = clock();
	int myRangeFoundAmount = 0;
	for (int i = 0; i < itersToRangeQueries; i++) {
		myRangeFoundAmount += myTree.findInRange(dataToRangeQueries[i].first,
			dataToRangeQueries[i].second).size();
	}

	clock_t myRangeEnd = clock();
	float myRangeTime = (float(myRangeEnd - myRangeStart)) / CLOCKS_PER_SEC;
	clock_t stlRangeStart = clock();
	int stlRangeFoundAmount = 0;
	for (int i = 0; i < itersToRangeQueries; i++) {
		const auto &low = stlTree.lower_bound(dataToRangeQueries[i].first);
		const auto &up = stlTree.upper_bound(dataToRangeQueries[i].second);
		stlRangeFoundAmount += distance(low, up);
	}
	clock_t stlRangeEnd = clock();
	float stlRangeTime = (float(stlRangeEnd - stlRangeStart)) / CLOCKS_PER_SEC;
	cout << "My BinaryTree height: " << myTreeHeight << ", optimal height = " <<
		optimalTreeHeight << endl;
	cout << "Time: " << myTime << ", size: " << myInsertSize << " - " << myEraseSize << ",found amount: "
		<< myFoundAmount << endl;
	cout << "Range time: " << myRangeTime << ", range found amount: " << myRangeFoundAmount
		<< endl << endl;
	cout << "STL Tree:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlInsertSize << " - " << stlEraseSize <<
		", found amount: " << stlFoundAmount << endl;
	cout << "Range time: " << stlRangeTime << ", range found amount: " <<
		stlRangeFoundAmount << endl << endl;
	if (myInsertSize == stlInsertSize && myEraseSize == stlEraseSize &&
		myFoundAmount == stlFoundAmount && myRangeFoundAmount == stlRangeFoundAmount) {
		cout << "The lab is completed" << endl;
		return true;
	}
	cerr << ":(" << endl;
	return false;
}

int main() {
	testBinarySearchTree();
	system("pause");
}