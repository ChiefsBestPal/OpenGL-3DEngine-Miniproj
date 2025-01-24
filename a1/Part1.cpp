#include <iostream>
#include "Part1.h"

template <typename T>
T* createArray(const size_t size)
{
    T* arr = new T[size];

    return arr;
}

template <typename T>
void initializeArray(T* arr, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

template <typename T>
void deleteArray(T* arr, const size_t size)
{
    delete[] arr;
}

int part1_main()
{
    size_t size;
    std::cout << "Enter size of the array: ";
    std::cin >> size;

    int* arr = createArray<int>(size);
    initializeArray(arr, size);

    std::cout << "Array after initialization: " << std::endl;
    for (size_t i = 0; i < size; i++)
    {
        std::cout << arr[i] << std::endl;
    }

    deleteArray(arr, size);

    return 0;
}