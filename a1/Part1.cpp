#include <iostream>
#include "Part1.h"

template <typename T>
T* ArrayFactory::createArray(const size_t size)
{
    T* arr = new T[size];

    return arr;
}

template <typename T>
void ArrayFactory::initializeArray(T* arr, const size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = i;
    }
}

template <typename T>
void ArrayFactory::deleteArray(T* arr)
{
    delete[] arr;
    std::cout << "Successfully deleted array";
}

int part1_main()
{
    int size = 0;

    while(size <= 0)
    {
        std::cout << "Enter size of the array: ";
        std::cin >> size;
    }

    ArrayFactory array_factory;

    int* arr = array_factory.createArray<int>(size);
    ArrayFactory::initializeArray(arr, size);

    std::cout << "Array after initialization: " << std::endl;
    for (size_t i = 0; i < size; i++)
    {
        std::cout << arr[i] << std::endl;
    }

    ArrayFactory::deleteArray(arr);

    return 0;
}