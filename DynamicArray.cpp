#include <iostream>
#include <string>
#include <time.h>
using namespace std;


struct Phone {
	string mark;
	int color;
	string phone;

	Phone(string mark1, int color1 ,string phone1) 
	{
		mark = mark1;
		color = color1;
		phone = phone1;
	}
	Phone() {}
};

struct DynamicArray {
private:
	int n;
	int size;
	Phone * phoneArr;
	Phone *temp;

public:

	DynamicArray(int size1)
	{										
		n = size1 <= 0 ? 1 : size1;
		phoneArr = new Phone[n];
		size = 0;
	}

	void resize()
	{
		temp = new Phone[n * 2];
		for (int i = 0; i < n; i++) { temp[i] = phoneArr[i]; }
		delete[] phoneArr;
		phoneArr = temp;
		n *= 2;	
	}

	void push_back(Phone phone)
	{
		phoneArr[size] = phone;
		size++;
		if (size == n)
			resize();
	}

	Phone pop_back()
	{
		if (size < 0) {
			cout << "Array is empty" << endl;
			return{};
		}
		else {
			size--;
			//cout << phoneArr[size--].phone << " deleted" << endl;
			return phoneArr[size];
		}
	}

	void push_front(Phone phone)
	{
		if (size == n)
		{
			resize();
		}
		if (size > 0) {
			for (int i = size - 1; i >= 0; i--) phoneArr[i + 1] = phoneArr[i];
		}
		phoneArr[0] = phone;
		size++;
	}

	Phone pop_front()
	{
		if (size == 0)
		{
			cout << "Empty" << endl;
			return{};
		}
		else {
			Phone phone = phoneArr[0];
			for (int i = 0; i < size; i++) phoneArr[i] = phoneArr[i + 1];
			size--;
			return phoneArr[size];
		}
	}

	Phone get(int n)
	{
		if (n > size)
		{
			cout << "Error this element doesn't exist";
			return{};
		}
		else
		{
			return phoneArr[n];
		}
	}

	int get_Size()
	{
		return size;
	}

	void print()
	{
			for (int i = 0; i < size; i++)
			{
				cout << i + 1 << ")\nMark:" << phoneArr[i].mark << endl;
				cout << "Color:" << phoneArr[i].color << endl;
				cout << "Phone:" << phoneArr[i].phone << endl;
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
	int input;
	cout << "Enter number of elements: ";
	cin >> input;
	cout << endl;

	clock_t start = clock();
	float sum = 0;
	DynamicArray arr(input);
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
	for (int i = 0; i < 50; i++) // 50 , а не 5000
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
