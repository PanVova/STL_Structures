#include <iostream>
#include <string>
#include <time.h>
using namespace std;

struct Phone {
	string mark;
	int color;
	string phone;

	Phone(string mark1, int color1, string phone1)
	{
		mark = mark1;
		color = color1;
		phone = phone1;
	}
	Phone() {}
};

struct Node 
{
	Phone data;
	Node *next;
};

struct LinkedList {
	int size=0;
	Node * node, *head, *tail;

	LinkedList(){
		head = NULL;
		tail = NULL;
	} 

	void push_back(Phone phone)
	{
		Node *temp = new Node;
		temp->data = phone;
		temp->next = NULL;

		if (head == NULL) head = temp;
		else tail->next = temp;
		
		tail = temp;
		temp->next = NULL;
		size++;
	}
	
	void push_front(Phone phone)
	{
		Node* new_node = new Node();
		new_node->data = phone;
		if (head == NULL) tail = new_node;
		new_node->next = head;
		head = new_node;
		size++;
	}

	Phone get(int n)
	{
		if (n > size){
			cout << "Error this element doesn't exist";
			return{};
		}

		Node* temp = head;
		for (int i = 0; i < n; i++) temp = temp->next;

		return temp->data;
	}

	int get_Size()
	{
		return size;
	}

	void print() {
		if (head == NULL) { cout << "List is empty" << endl; }
		else {
			int i = 0;
			Node *ne = head;
			while (ne != NULL) {
				cout << i + 1 << ")\nMark:" << ne->data.mark << endl;
				cout << "Color:" << ne->data.color << endl;
				cout << "Phone:" << ne->data.phone << endl;
				ne = ne->next;
				i++;
			}	
		}
	}
	
	Phone pop_back() {
		if (head == NULL)
			return{};

		Phone p;
		size--;

		if (head->next == NULL) {
			p = head->data;
			delete head;
			head = NULL;
			tail = NULL;
			return{};
		}

		Node* temp = head;
		while (temp->next->next != NULL) temp = temp->next;
		tail = temp;
		p = temp->next->data;
		temp->next = NULL;
		//cout << p.phone << " is removed." << endl;
		delete temp->next;
		return p;
	}


	Phone pop_front()
	{
		if (head)
		{
			Node *tmp = head;
			head = head->next;
			size--;
			delete tmp;
		}

		if (head == NULL) return{};
		if (head->next == NULL) tail = NULL;

		Phone p = head->data;
		Node* temp = head;
		head = head->next;
		//cout << p.phone << " is removed." << endl;
		delete temp;
		size--;
		return p;
	}

	~LinkedList() {
		Node* temp = head;
		Node* next;
		while (temp != NULL) {
			next = temp->next;
			delete temp;
			temp = next;
		}
	}
};


float timer(clock_t start)
{
	clock_t end = clock();
	float seconds = (float(end - start)) / CLOCKS_PER_SEC;
	cout << seconds << endl;
	return seconds;
}

int main()
{
	srand(time(NULL));
	clock_t start = clock();
	float sum=0;
	LinkedList arr;
	Phone p;

	for (int i = 0; i<50000; i++)
	{
		p = Phone("Example", 5, "Marshall");
		arr.push_back(p);
	}
	cout << "1)" << endl;
	sum += timer(start);

	start = clock();
	for (int i = 0; i<100; i++) //100 , а не 10000
	{
		p = Phone("Example", 5, "Marshall");
		arr.push_front(p);
	}
	cout << "2)" << endl;
	sum += timer(start);

	start = clock();
	for (int i = 0; i<20000; i++) {
		arr.get(rand() % 20000);
	}
	cout << "3)" << endl;
	sum += timer(start);

	start = clock();
	for (int i = 0; i < 5000; i++)
	{
		arr.pop_front();
	}
	cout << "4)" << endl;
	sum += timer(start);


	start = clock();
	for (int i = 0; i < 5000; i++)
	{
		arr.pop_back();
	}
	cout << "5)" << endl;
	sum += timer(start);

	cout << "Sum: " << sum << endl;

	system("pause");
}
