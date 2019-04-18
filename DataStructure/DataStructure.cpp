#include<iostream>
using namespace std;

#pragma region
template <typename T>
class Node
{
public:
	T value;

	Node<T> *next;
	Node<T> *prev;

	Node();
	Node(T a);
};

template <typename T>
Node<T>::Node()
{
	value = T();
	next = nullptr;
	prev = nullptr;
}

template <typename T>
Node<T>::Node(T a)
{
	value = a;
	next = nullptr;
	prev = nullptr;
}
#pragma endregion Node

#pragma region
template <typename T>
class SinglyLinkedList
{
private:
	Node<T> *head, *tail;

public:

	SinglyLinkedList();
	~SinglyLinkedList();

	void Insert(T num);
	void Remove(T num);
	void Print();
	void RemoveAll();
};

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList()
{
	head = new Node<T>;
	tail = new Node<T>;
	head->next = tail;
	tail->next = tail;
}

template <typename T>
void SinglyLinkedList<T>::Insert(T num)
{
	Node<T> *temp = new Node<T>(num);
	Node<T> *cursor = head;

	while (cursor->next != tail)
		cursor = cursor->next;

	cursor->next = temp;
	temp->next = tail;
}

template <typename T>
void SinglyLinkedList<T>::Remove(T num)
{
	Node<T> *cursor = head;

	while (cursor->next->value != num)
	{
		if (cursor->next == tail)
		{
			cout << "리스트에 해당 값이 없습니다." << endl;
			return;
		}

		cursor = cursor->next;
	}

	Node<T> *temp = cursor->next;
	cursor->next = temp->next;
	delete temp;
}

template <typename T>
void SinglyLinkedList<T>::Print()
{
	Node<T> *cursor = head->next;

	while (cursor != tail)
	{
		cout << cursor->value << " ";
		cursor = cursor->next;
	}

	cout << endl;
}

template <typename T>
void SinglyLinkedList<T>::RemoveAll()
{
	Node<T> *cursor = head->next;
	Node<T> *temp = head->next;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
	RemoveAll();
	delete head;
	delete tail;
}
#pragma endregion SinglyLinkedList

#pragma region
template <typename T>
class DoublyLinkedList
{
private:
	Node<T> *head, *tail;

public:

	DoublyLinkedList();
	~DoublyLinkedList();

	void InsertFront(T a);
	void InsertBack(T a);
	void Remove(T a);
	void RemoveAll();
	void Print();
	void PrintBack();
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
{
	head = new Node<T>;
	tail = new Node<T>;
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
}

template <typename T>
void DoublyLinkedList<T>::InsertFront(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->next = head->next;
	temp->prev = head;
	head->next = temp;
	temp->next->prev = temp;
}

template <typename T>
void DoublyLinkedList<T>::InsertBack(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->prev = tail->prev;
	temp->next = tail;
	tail->prev->next = temp;
	tail->prev = temp;
}

template <typename T>
void DoublyLinkedList<T>::Remove(T a)
{
	Node<T> *cursor = head;
	while (cursor->next->value != a)
	{
		if (cursor->next == tail)
		{
			cout << "리스트에 해당 수가 없습니다" << endl;
			return;
		}

		cursor = cursor->next;
	}

	Node<T> *temp = cursor->next;
	cursor->next = temp->next;
	temp->next->prev = cursor;
	delete temp;
}

template <typename T>
void DoublyLinkedList<T>::RemoveAll()
{
	Node<T> *cursor = head->next;
	Node<T> *temp = head->next;

	while (cursor != tail)
	{
		cursor = cursor->next;
		delete temp;
		temp = cursor;
	}

	head->next = tail;
	tail->prev = head;
}

template <typename T>
void DoublyLinkedList<T>::Print()
{
	Node<T> *cursor = head->next;

	while (cursor != tail)
	{
		cout << cursor->value << " ";
		cursor = cursor->next;
	}

	cout << endl;
}

template <typename T>
void DoublyLinkedList<T>::PrintBack()
{
	Node<T> *cursor = tail->prev;

	while (cursor != head)
	{
		cout << cursor->value << " ";
		cursor = cursor->prev;
	}

	cout << endl;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	RemoveAll();
	delete head;
	delete tail;
}
#pragma endregion DoublyLinkedList

#pragma region
template <typename T>
class ArrayStack
{
private:
	T *stack;
	int stackSize;
	int count;

public:
	ArrayStack();
	~ArrayStack();

	void Push(T a);
	T Pop();
	T Top();
	void Clear();
	int Count();
};

template <typename T>
ArrayStack<T>::ArrayStack()
{
	stackSize = 10;
	stack = new T[stackSize];

	for (int i = 0; i < stackSize; ++i)
		stack[i] = T();

	count = 0;
}

template <typename T>
void ArrayStack<T>::Push(T a)
{
	if (count == stackSize)
	{
		stackSize += 10;
		T *temp = stack;
		stack = new T[stackSize];

		for (int i = 0; i < stackSize; ++i)
			stack[i] = (i < stackSize - 10) ? temp[i] : 0;

		delete[] temp;
	}

	stack[count++] = a;
}

template <typename T>
T ArrayStack<T>::Pop()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}
	else if (--count < stackSize - 10)
	{
		stackSize -= 10;
		T *temp = stack;
		stack = new T[stackSize];

		for (int i = 0; i < stackSize; ++i)
			stack[i] = temp[i];

		delete[] temp;
	}

	int num = stack[count];
	stack[count] = 0;
	return num;
}

template <typename T>
T ArrayStack<T>::Top()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	return stack[count - 1];
}

template <typename T>
void ArrayStack<T>::Clear()
{
	if (stackSize != 10)
	{
		stackSize = 10;
		delete[] stack;
		stack = new T[stackSize];
	}

	for (int i = 0; i < stackSize; ++i)
		stack[i] = 0;

	count = 0;
}

template <typename T>
int ArrayStack<T>::Count()
{
	return count;
}

template <typename T>
ArrayStack<T>::~ArrayStack()
{
	delete[] stack;
}
#pragma endregion ArrayStack

#pragma region
template <typename T>
class ListStack
{
private:
	Node<T> *head, *tail;
	int count;

public:
	ListStack();
	~ListStack();

	void Push(T a);
	T Pop();
	T Top();
	void Clear();
	int Count();
};

template <typename T>
ListStack<T>::ListStack()
{
	head = new Node<T>();
	tail = new Node<T>();
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
	count = 0;
}

template <typename T>
void ListStack<T>::Push(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->next = tail;
	temp->prev = tail->prev;
	tail->prev->next = temp;
	tail->prev = temp;
	++count;
}

template <typename T>
T ListStack<T>::Pop()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	--count;
	Node<T> *temp = tail->prev;
	T num = temp->value;
	tail->prev = temp->prev;
	temp->prev->next = tail;
	delete temp;

	return num;
}

template <typename T>
T ListStack<T>::Top()
{
	if (count == 0)
	{
		cout << "스택에 든 게 없습니다." << endl;
		return 0;
	}

	return tail->prev->value;
}

template <typename T>
void ListStack<T>::Clear()
{
	Node<T> *cursor = head->next;
	Node<T> *temp = cursor;

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

template <typename T>
int ListStack<T>::Count()
{
	return count;
}

template <typename T>
ListStack<T>::~ListStack()
{
	Clear();
	delete head;
	delete tail;
}
#pragma endregion ListStack

#pragma region
template <typename T>
class ArrayQueue
{
private:
	T *queue;
	int queueSize;
	int pushCursor;
	int popCursor;
	int count;

public:
	ArrayQueue();
	~ArrayQueue();

	void Push(T a);
	T Pop();
	T Top();
	void Clear();
	int Count();
};

template <typename T>
ArrayQueue<T>::ArrayQueue()
{
	queueSize = 10;
	queue = new T[queueSize];

	for (int i = 0; i < 10; ++i)
		queue[i] = 0;

	pushCursor = 0;
	popCursor = 0;
	count = 0;
}

template <typename T>
ArrayQueue<T>::~ArrayQueue()
{
	delete[] queue;
}

template <typename T>
void ArrayQueue<T>::Push(T a)
{
	if (pushCursor == popCursor && count > 0)
	{
		T *temp = queue;
		queue = new T[queueSize + 10];

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

template <typename T>
T ArrayQueue<T>::Pop()
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
		T *temp = queue;
		queue = new T[queueSize - 10];


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

template <typename T>
T ArrayQueue<T>::Top()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	return queue[popCursor];
}

template <typename T>
void ArrayQueue<T>::Clear()
{
	delete[] queue;
	queueSize = 10;
	queue = new T[queueSize];

	for (int i = 0; i < 10; ++i)
		queue[i] = 0;

	popCursor = 0;
	pushCursor = 0;
	count = 0;
}

template <typename T>
int ArrayQueue<T>::Count()
{
	return count;
}
#pragma endregion ArrayQueue

#pragma region
template <typename T>
class ListQueue
{
private:
	Node<T> *head, *tail;
	int count;

public:
	ListQueue();
	~ListQueue();

	void Push(T a);
	T Pop();
	T Top();
	void Clear();
	int Count();
};

template <typename T>
ListQueue<T>::ListQueue()
{
	head = new Node<T>;
	tail = new Node<T>;
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
	count = 0;
}

template <typename T>
ListQueue<T>::~ListQueue()
{
	Clear();
	delete head;
	delete tail;
}

template <typename T>
void ListQueue<T>::Push(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->prev = head;
	temp->next = head->next;
	head->next = temp;
	temp->next->prev = temp;
	++count;
}

template <typename T>
T ListQueue<T>::Pop()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	Node<T> *temp = tail->prev;
	T num = temp->value;
	temp->prev->next = tail;
	tail->prev = temp->prev;
	--count;
	delete temp;

	return num;
}

template <typename T>
T ListQueue<T>::Top()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다." << endl;
		return 0;
	}
	return tail->prev->value;
}

template <typename T>
void ListQueue<T>::Clear()
{
	Node<T> *cursor = head->next;
	Node<T> *temp = cursor;

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

template <typename T>
int ListQueue<T>::Count()
{
	return count;
}
#pragma endregion ListQueue

#pragma region
template <typename T>
class ListDeque
{
private:
	int count;
	Node<T> *head, *tail;


public:
	ListDeque();
	~ListDeque();

	void PushFront(T a);
	void PushEnd(T a);
	T PopFront();
	T PopEnd();
	T TopFront();
	T TopEnd();
	void Clear();
	int Count();
};

template <typename T>
ListDeque<T>::ListDeque()
{
	head = new Node<T>;
	tail = new Node<T>;
	head->prev = head;
	head->next = tail;
	tail->prev = head;
	tail->next = tail;
	count = 0;
}

template <typename T>
ListDeque<T>::~ListDeque()
{
	Clear();
	delete head;
	delete tail;
}

template <typename T>
void ListDeque<T>::PushFront(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->prev = head;
	temp->next = head->next;
	head->next = temp;
	temp->next->prev = temp;
	++count;
}

template <typename T>
void ListDeque<T>::PushEnd(T a)
{
	Node<T> *temp = new Node<T>(a);
	temp->prev = tail->prev;
	temp->next = tail;
	temp->prev->next = temp;
	tail->prev = temp;
	++count;
}

template <typename T>
T ListDeque<T>::PopFront()
{
	if (count == 0)
	{
		cout << "덱이 비어있습니다.";
		return 0;
	}

	Node<T> *temp = head->next;
	int num = temp->value;
	head->next = temp->next;
	temp->next->prev = head;
	--count;
	delete temp;

	return num;
}

template <typename T>
T ListDeque<T>::PopEnd()
{
	if (count == 0)
	{
		cout << "덱이 비어있습니다.";
		return 0;
	}

	Node<T> *temp = tail->prev;
	int num = temp->value;
	tail->prev = temp->prev;
	temp->prev->next = tail;
	--count;
	delete temp;

	return num;
}

template <typename T>
T ListDeque<T>::TopFront()
{
	if (count == 0)
	{
		cout << "덱이 비어있습니다.";
		return 0;
	}

	return head->next->value;
}

template <typename T>
T ListDeque<T>::TopEnd()
{
	if (count == 0)
	{
		cout << "덱이 비어있습니다.";
		return 0;
	}

	return tail->prev->value;
}

template <typename T>
void ListDeque<T>::Clear()
{
	Node<T> *temp = head->next;
	Node<T> *cursor = temp;

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

template <typename T>
int ListDeque<T>::Count()
{
	return count;
}
#pragma endregion ListDeque

#pragma region
template <typename T>
class TreeNode
{
public:
	TreeNode *parent, *left, *right;
	T value;

	TreeNode();
	TreeNode(T a);
};

template <typename T>
TreeNode<T>::TreeNode()
{
	parent = nullptr;
	left = nullptr;
	right = nullptr;
	value = T();
}

template <typename T>
TreeNode<T>::TreeNode(T a)
{
	parent = nullptr;
	left = nullptr;
	right = nullptr;
	value = a;
}
#pragma endregion TreeNode

#pragma region
template <typename T>
class BinaryTree
{
private:
	TreeNode<T> *head, *tail;
	void ClearOneNode(TreeNode<T> *cursor);
	void DFS(TreeNode<T> *cursor);

public:
	BinaryTree();
	~BinaryTree();

	void Insert(T a);
	void Remove(T a);
	void Browse();
	void Clear();
	void BFS();
	void DFS();
	void Print();
};

template <typename T>
BinaryTree<T>::BinaryTree()
{
	head = new TreeNode<T>();
	tail = new TreeNode<T>();
	head->parent = head;
	head->left = tail;
	head->right = tail;
	tail->parent = head;
	tail->left = tail;
	tail->right = tail;
}

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	Clear();
	delete head;
	delete tail;
}

template <typename T>
void BinaryTree<T>::Insert(T a)
{
	TreeNode<T> *temp = new TreeNode<T>(a);
	temp->left = tail;
	temp->right = tail;

	if (head->left == tail)
	{
		temp->parent = head;
		head->left = temp;
		head->right = temp;
	}
	else
	{
		TreeNode<T> *cursor = head->left;
		bool isLeft = false;
		bool isRight = false;
		while (!isLeft && !isRight)
		{
			if (cursor->value == a)
			{
				if (cursor->left == tail)
					isLeft = true;
				else if (cursor->right == tail)
					isRight = true;
				else
					cursor = cursor->right;
			}
			else if (cursor->value < a)
			{
				if (cursor->right == tail)
					isRight = true;
				else
					cursor = cursor->right;
			}
			else if (cursor->left == tail)
				isLeft = true;
			else
				cursor = cursor->left;
		}

		if (isLeft)
		{
			temp->parent = cursor;
			cursor->left = temp;
		}
		else if (isRight)
		{
			temp->parent = cursor;
			cursor->right = temp;
		}
	}
}

template <typename T>
void BinaryTree<T>::Remove(T a)
{
	if (head->left == tail)
	{
		cout << "트리가 비어있습니다." << endl;
		return;
	}

	TreeNode<T> *cursor = head->left;

	while (cursor->value != a)
	{
		if (cursor->value < a)
		{
			if (cursor->right == tail)
			{
				cout << "지울 값이 트리에 없습니다." << endl;
				return;
			}
			else
				cursor = cursor->right;
		}
		else if (cursor->left == tail)
		{
			cout << "지울 값이 트리에 없습니다." << endl;
			return;
		}
		else
			cursor = cursor->left;
	}

	while (1)
	{
		if (cursor->left != tail)
		{
			cursor->value = cursor->left->value;
			cursor = cursor->left;
		}
		else if (cursor->right != tail)
		{
			cursor->value = cursor->right->value;
			cursor = cursor->right;
		}
		else
		{
			if (cursor->parent->left == cursor)
				cursor->parent->left = tail;
			else
				cursor->parent->right = tail;

			delete cursor;
			return;
		}
	}
}

template <typename T>
void BinaryTree<T>::Browse()
{
	if (head->left == tail)
	{
		cout << "트리가 비어있습니다." << endl;
		return;
	}

	int command = 0;
	TreeNode<T> *cursor = head->left;

	while (1)
	{
		if (cursor->parent != head)
			cout << endl << "\t " << cursor->parent->value << endl;
		else
			cout << endl << "\t x" << endl;

		cout << "\t[" << cursor->value << ']' << endl;

		if (cursor->left != tail)
			cout << cursor->left->value;
		else
			cout << 'x';

		cout << "\t\t";

		if (cursor->right != tail)
			cout << cursor->right->value;
		else
			cout << 'x';

		cout << endl << "1 - 부모로 이동 / 2 - 좌측 자식 노드 / 3 - 우측 자식 노드 / 4 - 탐색 종료\n커맨드 입력 : ";
		cin >> command;

		if (command == 4)
			return;
		else if (command == 1)
		{
			if (cursor->parent == head)
				cout << "최상위 노드입니다." << endl;
			else
				cursor = cursor->parent;
		}
		else if (command == 2)
		{
			if (cursor->left == tail)
				cout << "좌측 자식 노드가 비어있습니다." << endl;
			else
				cursor = cursor->left;
		}
		else if (command == 3)
		{
			if (cursor->right == tail)
				cout << "우측 자식 노드가 비어있습니다." << endl;
			else
				cursor = cursor->right;
		}
		else
			cout << "잘못 입력하셨습니다." << endl;
	}
}

template <typename T>
void BinaryTree<T>::ClearOneNode(TreeNode<T> *cursor)
{
	if (cursor != tail)
	{
		ClearOneNode(cursor->left);
		ClearOneNode(cursor->right);
		delete cursor;
	}
}

template <typename T>
void BinaryTree<T>::Clear()
{
	ClearOneNode(head->left);
	head->left = tail;
	head->right = tail;
	tail->parent = head;
}

template <typename T>
void BinaryTree<T>::BFS()
{
	if (head->left == tail)
		cout << "트리가 비어있습니다.";
	else
	{
		ListQueue<TreeNode<T>> l;
		TreeNode<T> *cursor;
		l.Push(*(head->left));

		while (l.Count())
		{
			cursor = &(l.Pop());

			if (cursor->left != tail)
				l.Push(*(cursor->left));

			if (cursor->right != tail)
				l.Push(*(cursor->right));

			cout << cursor->value << " ";
		}
	}
}

template <typename T>
void BinaryTree<T>::DFS()
{
	if (head->left == tail)
		cout << "트리가 비어있습니다.";
	else
		DFS(head->left);

	cout << endl;
}

template <typename T>
void BinaryTree<T>::DFS(TreeNode<T> *cursor)
{
	if (cursor != tail)
	{
		cout << cursor->value << " ";
		DFS(cursor->left);
		DFS(cursor->right);
	}
}

template <typename T>
void BinaryTree<T>::Print()
{
	if (head->left == tail)
		cout << "트리가 비어있습니다.";
	else
	{
		ListStack<TreeNode<T>> l;
		TreeNode<T> *cursor = head->left;

		while (1)
		{
			if (cursor != tail)
			{
				l.Push(*cursor);
				cursor = cursor->left;
			}
			else if (l.Count() > 0)
			{
				cursor = &(l.Pop());
				cout << cursor->value << " ";
				cursor = cursor->right;
			}
			else
				break;
		}
	}

	cout << endl;
}
#pragma endregion BinaryTree

#pragma region
template <typename T>
class PriorityQueue
{
private:
	TreeNode<T> *head, *tail;
	int count;

	void ClearOneNode(TreeNode<T> *node);

public:
	PriorityQueue();
	~PriorityQueue();

	void Push(T a);
	T Pop();
	T Top();
	void Clear();
	int Count();
};

template <typename T>
PriorityQueue<T>::PriorityQueue()
{
	head = new TreeNode<T>();
	tail = new TreeNode<T>();
	head->parent = head;
	head->left = tail;
	head->right = tail;
	tail->parent = head;
	tail->left = tail;
	tail->right = tail;

	count = 0;
}

template <typename T>
PriorityQueue<T>::~PriorityQueue()
{
	Clear();
	delete head;
	delete tail;
}

template <typename T>
void PriorityQueue<T>::Push(T a)
{
	++count;

	if (head->left == tail)
	{
		TreeNode<T> *temp = new TreeNode<T>(a);
		head->left = temp;
		head->right = temp;
		temp->parent = head;
		temp->left = tail;
		temp->right = tail;
	}
	else
	{
		TreeNode<T> *cursor = head->left;
		T tempT = a;

		while (1)
		{
			if (tempT > cursor->value)
			{
				int t = cursor->value;
				cursor->value = tempT;
				tempT = t;
			}

			bool isLeft = false, isRight = false;

			if (cursor->left == tail && cursor->right == tail)
				isLeft = true;
			else if (cursor->left != tail && cursor->right != tail)
			{
				if (cursor->left->value > cursor->right->value)
					cursor = cursor->right;
				else
					cursor = cursor->left;
			}
			else if (cursor->left == tail)
				isLeft = true;
			else
				isRight = true;

			if (isLeft || isRight)
			{
				TreeNode<T> *temp = new TreeNode<T>(tempT);
				temp->left = tail;
				temp->right = tail;
				temp->parent = cursor;

				if (isLeft)
					cursor->left = temp;
				else
					cursor->right = temp;

				break;
			}
		}
	}
}

template <typename T>
T PriorityQueue<T>::Pop()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다.";
		return 0;
	}

	--count;
	int num = head->left->value;
	TreeNode<T> *cursor = head->left;

	while (1)
	{
		bool isLeft = false, isRight = false;

		if (cursor->left != tail && cursor->right != tail)
		{
			if (cursor->left->value > cursor->right->value)
				isLeft = true;
			else
				isRight = true;
		}
		else if (cursor->left != tail)
			isLeft = true;
		else if (cursor->right != tail)
			isRight = true;
		else if (cursor->parent->left == cursor)
			cursor->parent->left = tail;
		else
			cursor->parent->right = tail;

		if (isLeft)
		{
			cursor->value = cursor->left->value;
			cursor = cursor->left;
		}
		else if (isRight)
		{
			cursor->value = cursor->right->value;
			cursor = cursor->right;
		}
		else
		{
			delete cursor;
			break;
		}
	}
	return num;
}

template <typename T>
T PriorityQueue<T>::Top()
{
	if (count == 0)
	{
		cout << "큐가 비어있습니다.";
		return 0;
	}

	return head->left->value;
}

template <typename T>
void PriorityQueue<T>::ClearOneNode(TreeNode<T> *node)
{
	if (node != tail)
	{
		ClearOneNode(node->left);
		ClearOneNode(node->right);
		delete node;
	}
}

template <typename T>
void PriorityQueue<T>::Clear()
{
	ClearOneNode(head->left);
	head->left = tail;
	head->right = tail;
	tail->parent = head;
	count = 0;
}

template <typename T>
int PriorityQueue<T>::Count()
{
	return count;
}
#pragma endregion PriorityQueue

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);
}