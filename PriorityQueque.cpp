#include <iostream>
#include <queue>
#include <time.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cstring>

#define iterations 200000
using namespace std;

struct Enemy {
	float health, damage, skill;

	Enemy()
	{
		this->health = rand() % 100 + 1;
		this->damage = rand() % 10 + 1;
		this->skill = rand() % 20 + 5;
	}

	Enemy(float health, float damage, float skill)
	{
		this->health = health;
		this->damage = damage;
		this->skill = skill;
	}
};

bool operator<(Enemy const &c1, Enemy const &c2)
{
	float formula1 = (c1.health + c1.damage + c1.skill) / 3;
	float formula2 = (c2.health + c2.damage + c2.skill) / 3;
	return  (formula1 < formula2) ? true : false;
}

bool operator>(Enemy const &c1, Enemy const &c2)
{
	float formula1 = (c1.health + c1.damage + c1.skill) / 3;
	float formula2 = (c2.health + c2.damage + c2.skill) / 3;
	return  (formula1 > formula2) ? true : false;
}

bool operator== (Enemy const &c1, Enemy const &c2) {

	float formula1 = (c1.health + c1.damage + c1.skill) / 3;
	float formula2 = (c2.health + c2.damage + c2.skill) / 3;
	return  (formula1 == formula2) ? true : false;
}

bool operator>= (Enemy const &c1, Enemy const &c2) {

	float formula1 = (c1.health + c1.damage + c1.skill) / 3;
	float formula2 = (c2.health + c2.damage + c2.skill) / 3;
	return  (formula1 >= formula2) ? true : false;
}

bool operator<= (Enemy const &c1, Enemy const &c2) {

	float formula1 = (c1.health + c1.damage + c1.skill) / 3;
	float formula2 = (c2.health + c2.damage + c2.skill) / 3;
	return  (formula1 <= formula2) ? true : false;
}

void our_swap(Enemy *a, Enemy *b) {
	Enemy temp = *a;
	*a = *b;
	*b = temp;
}

struct BinaryHeap {

protected:
	Enemy *our_array;
	int size, capacity;

	int getParent(int index) { return (index - 1) / 2; }

	int getLeftChild(int index) { return (2 * index + 1); }

	int getRightChild(int index) { return ((2 * index) + 2); }

	void shiftUp(int index)
	{
		while (index > 0 && our_array[getParent(index)] < our_array[index]) {
			our_swap(&our_array[getParent(index)], &our_array[index]);
			index = getParent(index);
		}
	}

	void shiftDown(int index)
	{
		int maxIndex = index;
		int l = getLeftChild(index);
		int r = getRightChild(index);

		if (l < size && our_array[l] > our_array[maxIndex]) {
			maxIndex = l;
		}

		if (r < size && our_array[r] > our_array[maxIndex]) {
			maxIndex = r;
		}

		if (index != maxIndex) {
			our_swap(&our_array[index], &our_array[maxIndex]);
			shiftDown(maxIndex);
		}
	}
};

struct PriorityQueue : BinaryHeap {

	PriorityQueue() { size = 0, capacity = 0; }

	void resize()
	{
		capacity *=2;
		Enemy *temp = new Enemy[capacity];
		for (int i = 0; i < size; i++) { temp[i] = our_array[i]; }

		delete[] our_array;
		our_array = temp;
	}

	void push(Enemy object) {
		if (size == 0) {
			our_array = new Enemy[++capacity];
			our_array[size++] = object;
			return;
		}
		if (size == capacity) resize();
		our_array[size++] = object;
		shiftUp(size - 1);
	}

	Enemy pop() {
		Enemy max = our_array[0];
		our_swap(&our_array[0], &our_array[--size]);
		shiftDown(0);
		return max;
	}

	Enemy top() { return our_array[0]; }

	int getSize() { return size; }

	bool empty() { return size == 0; }

};

struct HeapSort : BinaryHeap {

	void Heapify(Enemy *array, int size, int index) {
		int l = getLeftChild(index);
		int r = getRightChild(index);
		int largest = index;

		if (l < size && array[l] > array[largest])
		{
			largest = l;
		}
		if (r < size && array[r] > array[largest]) {
			largest = r;
		}

		if (largest != index) {
			our_swap(&array[index], &array[largest]);
			Heapify(array, size, largest);
		}
	}

	void build_heap(Enemy arr[], int size)
	{
		for (int i = size / 2; i >= 0; i--) Heapify(arr, size, i);
	}

	void heapSort(Enemy arr[], int size) {
		
		build_heap(arr, size);

		for (int i = size - 1; i > 0; i--) {
			our_swap(&arr[0], &arr[i]);
			Heapify(arr, i, 0);
		}
	}
};

template<typename T> float testPriorityQueueSpeed(T &&priorityQueue) {
	const int iters = iterations;
	clock_t timeStart = clock();
	for (int i = 0; i < iters; i++) {
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++) {
			priorityQueue.push(Enemy());
		}
		priorityQueue.top();
		priorityQueue.pop();
	}
	clock_t timeEnd = clock();
	float time = (float(timeEnd - timeStart)) / CLOCKS_PER_SEC;
	return time;
}

bool testHeapSort() {
	const int iters = iterations;

	Enemy *array = new Enemy[iters];
	for (int i = 0; i < iters; i++)
		array[i] = Enemy();
	Enemy *mySortedArray = array;
	Enemy *stlSortedArray = array;
	Enemy *stlHeapSortedArray = array;

	HeapSort HP;
	clock_t my_start = clock();
	HP.heapSort(mySortedArray, iters);
	clock_t my_end = clock();

	clock_t stl_start = clock();
	sort(stlSortedArray, stlSortedArray + iters);
	clock_t stl_end = clock();

	clock_t stlH_start = clock();
	sort_heap(stlHeapSortedArray, stlHeapSortedArray + iters);
	clock_t stlH_end = clock();

	float myTime = (float(my_end - my_start)) / CLOCKS_PER_SEC;
	float stlTime = (float(stl_end - stl_start)) / CLOCKS_PER_SEC;
	float stlHTime = (float(stlH_end - stlH_start)) / CLOCKS_PER_SEC;

	cout << "My HeapSort:" << endl;
	cout << "Time: " << myTime << endl;
	cout << "STL Sort:" << endl;
	cout << "Time: " << stlTime << endl;
	cout << "STL Heap Sort:" << endl;
	cout << "Time: " << stlHTime << endl << endl;
	delete[] array;
	if (mySortedArray == stlSortedArray && mySortedArray == stlHeapSortedArray) {
		cout << "The lab is completed" << endl;
		return true;
	}
	cerr << ":(" << endl << endl;
	return false;

}

bool testPriorityQueue() {
	srand(time(NULL));
	const int iters = iterations;
	PriorityQueue myPriorQueue;
	priority_queue<Enemy> stlPriorQueue;
	bool isDataEqual = true;
	for (int i = 0; i < iters; i++) {
		int insertDataAmount = rand() % 6 + 5;
		for (int j = 0; j < insertDataAmount; j++) {
			Enemy randData = Enemy();
			myPriorQueue.push(randData);
			stlPriorQueue.push(randData);
		}
		if (!(myPriorQueue.top() == stlPriorQueue.top())) {
			isDataEqual = false;
			cerr << "Comparing failed on iteration " << i << endl << endl;
			break;
		}
		int removeDataAmount = rand() % insertDataAmount;
		for (int j = 0; j < removeDataAmount; j++) {
			myPriorQueue.pop();
			stlPriorQueue.pop();
		}
	}
	int myQueueSize = myPriorQueue.getSize();
	int stlQueueSize = stlPriorQueue.size();
	float stlTime = testPriorityQueueSpeed<priority_queue<Enemy>>(priority_queue<Enemy>());
	float myTime = testPriorityQueueSpeed<PriorityQueue>(PriorityQueue());
	cout << "My PriorityQueue:" << endl;
	cout << "Time: " << myTime << ", size: " << myQueueSize << endl;
	cout << "STL priority_queue:" << endl;
	cout << "Time: " << stlTime << ", size: " << stlQueueSize << endl << endl;
	if (isDataEqual && myQueueSize == stlQueueSize) {
		cout << "The lab is completed" << endl << endl;
		return true;
	}
	cerr << ":(" << endl << endl;
	return false;
}


int main() {
	testPriorityQueue();
	testHeapSort();
	system("pause");
}