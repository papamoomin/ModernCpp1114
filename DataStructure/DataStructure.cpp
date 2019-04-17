#include<iostream>
using namespace std;

//자연수에만 대응되게 짰음을 알림

#pragma region
class Node
{
public:
	int value;

	Node *next;
	Node *prev;

	Node();
	Node(int a);
	~Node();
};

Node::Node()
{
	value = 0;
	next = nullptr;
	prev = nullptr;
}

Node::Node(int a)
{
	value = a;
	next = nullptr;
	prev = nullptr;
}

Node::~Node()
{
}
#pragma endregion Node

#pragma region
class SinglyLinkedList
{
private:
	Node *head, *tail;

public:

	SinglyLinkedList();
	~SinglyLinkedList();

	void Insert(int num);
	void Remove(int num);
	void Print();
	void RemoveAll();
};

SinglyLinkedList::SinglyLinkedList()
{
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->next = tail;
}

void SinglyLinkedList::Insert(int num)
{
	Node *temp = new Node(num);
	Node *cursor = head;

	while (cursor->next != tail)
		cursor = cursor->next;

	cursor->next = temp;
	temp->next = tail;
}

void SinglyLinkedList::Remove(int num)
{
	Node *cursor = head;

	while (cursor->next->value != num)
	{
		if (cursor->next == tail)
		{
			cout << "리스트에 해당 값이 없습니다." << endl;
			return;
		}

		cursor = cursor->next;
	}

	Node *temp = cursor->next;
	cursor->next = temp->next;
	delete temp;
}

void SinglyLinkedList::Print()
{
	Node *cursor = head->next;

	while (cursor != tail)
	{
		cout << cursor->value << " ";
		cursor = cursor->next;
	}

	cout << endl;
}

void SinglyLinkedList::RemoveAll()
{
	Node *cursor = head->next;
	Node *temp = head->next;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
}

SinglyLinkedList::~SinglyLinkedList()
{
	RemoveAll();
	delete head;
	delete tail;
}
#pragma endregion SinglyLinkedList

#pragma region
class DoublyLinkedList
{
private:
	Node *head, *tail;

public:

	DoublyLinkedList();
	~DoublyLinkedList();

	void InsertFront(int a);
	void InsertBack(int a);
	void Remove(int a);
	void RemoveAll();
	void Print();
	void PrintBack();
};

DoublyLinkedList::DoublyLinkedList()
{
	head = new Node;
	tail = new Node;
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

void DoublyLinkedList::InsertFront(int a)
{
	Node *temp = new Node(a);
	temp->next = head->next;
	temp->prev = head;
	head->next = temp;
	temp->next->prev = temp;
}

void DoublyLinkedList::InsertBack(int a)
{
	Node *temp = new Node(a);
	temp->prev = tail->prev;
	temp->next = tail;
	tail->prev->next = temp;
	tail->prev = temp;
}

void DoublyLinkedList::Remove(int a)
{
	Node *cursor = head;
	while (cursor->next->value != a)
	{
		if (cursor->next == tail)
		{
			cout << "리스트에 해당 수가 없습니다" << endl;
			return;
		}

		cursor = cursor->next;
	}

	Node *temp = cursor->next;
	cursor->next = temp->next;
	temp->next->prev = cursor;
	delete temp;
}

void DoublyLinkedList::RemoveAll()
{
	Node *cursor = head->next;
	Node *temp = head->next;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
	tail->prev = head;
}

void DoublyLinkedList::Print()
{
	Node *cursor = head->next;

	while (cursor != tail)
	{
		cout << cursor->value << " ";
		cursor = cursor->next;
	}

	cout << endl;
}

void DoublyLinkedList::PrintBack()
{
	Node *cursor = tail->prev;

	while (cursor != head)
	{
		cout << cursor->value << " ";
		cursor = cursor->prev;
	}

	cout << endl;
}

DoublyLinkedList::~DoublyLinkedList()
{
	RemoveAll();
	delete head;
	delete tail;
}
#pragma endregion DoublyLinkedList

#pragma region
class ArrayStack
{
private:
	int *stack;
	int stackSize;
	int count;

public:
	ArrayStack();
	~ArrayStack();

	void Push(int a);
	int Pop();
	int Top();
	void Clear();
	int Count();
};

ArrayStack::ArrayStack()
{
	stackSize = 10;
	stack = new int[stackSize];

	for (int i = 0; i < stackSize; ++i)
		stack[i] = 0;

	count = 0;
}

void ArrayStack::Push(int a)
{
	if (count == stackSize)
	{
		stackSize += 10;
		int *temp = stack;
		stack = new int[stackSize];

		for (int i = 0; i < stackSize; ++i)
			stack[i] = (i < stackSize - 10) ? temp[i] : 0;

		delete[] temp;
	}

	stack[count++] = a;
}

int ArrayStack::Pop()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}
	else if (--count < stackSize - 10)
	{
		stackSize -= 10;
		int *temp = stack;
		stack = new int[stackSize];

		for (int i = 0; i < stackSize; ++i)
			stack[i] = temp[i];

		delete[] temp;
	}

	int num = stack[count];
	stack[count] = 0;
	return num;
}

int ArrayStack::Top()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	return stack[count - 1];
}

void ArrayStack::Clear()
{
	if (stackSize != 10)
	{
		stackSize = 10;
		delete[] stack;
		stack = new int[stackSize];
	}

	for (int i = 0; i < stackSize; ++i)
		stack[i] = 0;

	count = 0;
}

int ArrayStack::Count()
{
	return count;
}

ArrayStack::~ArrayStack()
{
	delete[] stack;
}
#pragma endregion ArrayStack

#pragma region
class ListStack
{
private:
	Node *head, *tail;
	int count;

public:
	ListStack();
	~ListStack();

	void Push(int a);
	int Pop();
	int Top();
	void Clear();
	int Count();
};

ListStack::ListStack()
{
	head = new Node();
	tail = new Node();
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
	count = 0;
}

void ListStack::Push(int a)
{
	Node *temp = new Node(a);
	temp->next = tail;
	temp->prev = tail->prev;
	tail->prev->next = temp;
	tail->prev = temp;
	++count;
}

int ListStack::Pop()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	--count;
	Node *temp = tail->prev;
	int num = temp->value;
	tail->prev = temp->prev;
	temp->prev->next = tail;
	delete temp;

	return num;
}

int ListStack::Top()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	return tail->prev->value;
}

void ListStack::Clear()
{
	Node *cursor = head->next;
	Node *temp = cursor;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
	tail->prev = head;
	count = 0;
}

int ListStack::Count()
{
	return count;
}

ListStack::~ListStack()
{
	Clear();
	delete head;
	delete tail;
}
#pragma endregion ListStack

#pragma region
class ArrayQueue
{
private:
	int *queue;
	int queueSize;
	int pushCursor;
	int popCursor;
	int count;

public:
	ArrayQueue();
	~ArrayQueue();

	void Push(int a);
	int Pop();
	int Top();
	void Clear();
	int Count();
};

ArrayQueue::ArrayQueue()
{
	queueSize = 10;
	queue = new int[queueSize];

	for (int i = 0; i < 10; ++i)
		queue[i] = 0;

	pushCursor = 0;
	popCursor = 0;
	count = 0;
}

ArrayQueue::~ArrayQueue()
{
	delete[] queue;
}

void ArrayQueue::Push(int a)
{
	if (pushCursor == popCursor && count > 0)
	{
		int *temp = queue;
		queue = new int[queueSize + 10];

		for (int i = pushCursor; i < queueSize; ++i)
			queue[i - pushCursor] = temp[i];

		for (int i = 0; i < pushCursor; ++i)
			queue[i + (queueSize - pushCursor)] = temp[i];

		for (int i = queueSize; i < queueSize + 10; ++i)
			queue[i] = 0;

		pushCursor = queueSize;
		popCursor = 0;
		queueSize += 10;

		delete[] temp;
	}
	queue[pushCursor++] = a;
	pushCursor %= queueSize;
	++count;
}

int ArrayQueue::Pop()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}

	int num = queue[popCursor];
	--count;
	queue[popCursor++] = 0;
	popCursor %= queueSize;

	if (count < queueSize - 10)
	{
		int *temp = queue;
		queue = new int[queueSize - 10];


		if (popCursor < pushCursor)
		{
			for (int i = popCursor; i < pushCursor; ++i)
				queue[i - popCursor] = temp[i];

		}
		else
		{
			for (int i = popCursor; i < queueSize; ++i)
				queue[i - popCursor] = temp[i];

			for (int i = 0; i < pushCursor; ++i)
				queue[queueSize - popCursor] = temp[i];
		}

		queueSize -= 10;
		pushCursor = queueSize - 1;
		queue[pushCursor] = 0;
		popCursor = 0;
	}

	return num;
}

int ArrayQueue::Top()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	return queue[popCursor];
}

void ArrayQueue::Clear()
{
	delete[] queue;
	queueSize = 10;
	queue = new int[queueSize];

	for (int i = 0; i < 10; ++i)
		queue[i] = 0;

	popCursor = 0;
	pushCursor = 0;
	count = 0;
}

int ArrayQueue::Count()
{
	return count;
}
#pragma endregion ArrayQueue

#pragma region
class ListQueue
{
private:
	Node *head, *tail;
	int count;

public:
	ListQueue();
	~ListQueue();

	void Push(int a);
	int Pop();
	int Top();
	void Clear();
	int Count();
};

ListQueue::ListQueue()
{
	head = new Node;
	tail = new Node;
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
	count = 0;
}

ListQueue::~ListQueue()
{
	Clear();
	delete head;
	delete tail;
}

void ListQueue::Push(int a)
{
	Node *temp = new Node(a);
	temp->prev = head;
	temp->next = head->next;
	head->next = temp;
	temp->next->prev = temp;
	++count;
}

int ListQueue::Pop()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	Node *temp = tail->prev;
	int num = temp->value;
	temp->prev->next = tail;
	tail->prev = temp->prev;
	--count;
	delete temp;

	return num;
}

int ListQueue::Top()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	return tail->prev->value;
}

void ListQueue::Clear()
{
	Node *cursor = head->next;
	Node *temp = cursor;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
	tail->prev = head;
	count = 0;
}

int ListQueue::Count()
{
	return count;
}
#pragma endregion ListQueue

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);
}