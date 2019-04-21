#include<iostream>
#include<vector>
#include<ctime>
using namespace std;

#pragma region
template<typename T>
void Swap(T &a, T &b)
{
	T c = a;
	a = b;
	b = c;
}
#pragma endregion Swap

#pragma region
template <typename T>
void BubbleSort(vector<T> &arr) // 10000개 : 23650ms
{
	for (int i = arr.size(); i > 0; --i)
		for (int j = 0; j < i - 1; ++j)
			if (arr[j] > arr[j + 1])
				Swap(arr[j], arr[j + 1]);
}

template <typename T>
void BubbleSort(T *arr, int size) // 10000개 : 606ms
{
	for (int i = size; i > 0; --i)
		for (int j = 0; j < i - 1; ++j)
			if (arr[j] > arr[j + 1])
				Swap(arr[j], arr[j + 1]);
}

#pragma endregion BubbleSort

#pragma region
template <typename T>
void SelectionSort(vector<T> &arr) // 10000개 : 27400ms
{
	for (int i = 0; i < arr.size() - 1; ++i)
		for (int j = i + 1; j < arr.size(); ++j)
			if (arr[i] > arr[j])
				Swap(arr[i], arr[j]);
}

template <typename T>
void SelectionSort(T *arr, int size) // 10000개 : 582ms
{
	for (int i = 0; i < size - 1; ++i)
		for (int j = i + 1; j < size; ++j)
			if (arr[i] > arr[j])
				Swap(arr[i], arr[j]);
}
#pragma endregion SelectionSort

#pragma region
template <typename T>
void InsertionSort(vector<T> &arr) // 10000개 : 21096ms
{
	for (int i = 1, temp, t; i < arr.size(); ++i)
	{
		temp = arr[i];
		t = i;

		for (int j = i - 1; j > -1; --j)
		{
			if (arr[j] < temp)
			{
				t = j + 1;
				break;
			}
			else
			{
				arr[j + 1] = arr[j];
				t = j;
			}
		}

		arr[t] = temp;
	}
}

template <typename T>
void InsertionSort(T *arr, int size) // 10000개 : 78ms
{
	for (int i = 1, temp, t; i < size; ++i)
	{
		temp = arr[i];
		t = i;

		for (int j = i - 1; j > -1; --j)
		{
			if (arr[j] < temp)
			{
				t = j + 1;
				break;
			}
			else
			{
				arr[j + 1] = arr[j];
				t = j;
			}
		}

		arr[t] = temp;
	}
}
#pragma endregion InsertionSort

#pragma region
template <typename T>
void MergeSort(vector<T> &arr) // 10000개 : 10ms
{
	T *arr2 = new T[arr.size()];

	for (int i = 0; i < arr.size(); ++i)
		arr2[i] = arr[i];

	Merge(arr2, 0, arr.size() - 1);

	for (int i = 0; i < arr.size(); ++i)
		arr[i] = arr2[i];

	delete[] arr2;
}

template <typename T>
void MergeSort(T *arr, int size) // 10000개 : 7ms
{
	Merge(arr, 0, size - 1);
}

template <typename T>
void Merge(T *arr, int left, int right)
{
	int mid = (left + right) / 2;
	int size = right - left + 1;
	int *arr2 = new int[size];

	if (mid > left)
	{
		Merge(arr, left, mid);
		Merge(arr, mid + 1, right);
	}

	for (int i = 0, leftCursor = left, rightCursor = mid + 1; i < size; ++i)
	{
		if (leftCursor > mid)
			arr2[i] = arr[rightCursor++];
		else if (rightCursor > right)
			arr2[i] = arr[leftCursor++];
		else if (arr[leftCursor] > arr[rightCursor])
			arr2[i] = arr[rightCursor++];
		else
			arr2[i] = arr[leftCursor++];
	}

	for (int i = left; i <= right; ++i)
		arr[i] = arr2[i - left];
}
#pragma endregion MergeSort

#pragma region
template <typename T>
void QuickSort(vector<T> &arr) // 10000개 : 9ms
{
	int *arr2 = new int[arr.size()];

	for (int i = 0; i < arr.size(); ++i)
		arr2[i] = arr[i];

	Quick(arr, 0, arr.size() - 1);

	for (int i = 0; i < arr.size(); ++i)
		arr1[i] = arr2[i];
}

template <typename T>
void QuickSort(T *arr, int size) // 10000개 : 4ms
{
	Quick(arr, 0, size - 1);
}

template <typename T>
void Quick(T *arr, int left, int right)
{
	if (left >= right)
		break;

	int pivot = left + right / 2, leftCursor = left, rightCursor = right;

	while (leftCursor >= rightCursor)
	{
		if (arr[pivot] > arr[leftCursor])
			++leftCursor;
		else if (arr[pivot] < arr[rightCursor])
			--rightCursor;
		else
		{
			Swap(arr[leftCursor], arr[rightCursor]);
			++leftCursor;
			--rightCursor;
		}
	}

	Swap(arr[leftCursor], arr[pivot]);
	Quick(arr, left, leftCursor - 1);
	Quick(arr, leftCursor + 1, right);
}
#pragma endregion QuickSort

int main()
{
	ios_base::sync_with_stdio(false);
	cout.tie(NULL);

	srand(time(NULL));
	vector<int> a;
	int size = 10000;
	int *b = new int[size];
	clock_t start, end;

	//for (int i = 0; i < size; ++i)
	//{
	//	int j = rand() % 10000;
	//	b[i] = j;
	//	cout << j << endl;
	//}
	//cout << endl << endl;
	//start = clock();
	//InsertionSort(b, size);
	//end = clock();
	//int error = 0;
	//for (int i = 0; i < size; ++i)
	//{
	//	if (i > 0)
	//		if (b[i - 1] > b[i])
	//			++error;
	//	cout << i << " : " << b[i] << endl;
	//}
	//cout << "error : " << error << endl;
	//double f1 = (double)(end - start);
	//cout << endl << endl << f1 << endl;



	for (int i = 0; i < size; ++i)
	{
		int j = rand() % 100000;
		a.push_back(j);
		cout << j << endl;
	}
	cout << endl << endl;

	start = clock();
	InsertionSort(a);
	end = clock();

	int error = 0;
	for (int i = 0; i < size; ++i)
	{
		if (i > 0)
			if (a[i - 1] > a[i])
				++error;
		cout << i << " : " << a[i] << endl;
	}
	cout << "error : " << error << endl;

	double f2 = (double)(end - start);
	cout << endl << endl << f2 << endl;
}