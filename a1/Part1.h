#ifndef PART1_H
#define PART1_H

#include <stddef.h>

class ArrayFactory
{
public:
    template <typename T>
    T* createArray(size_t size);

    template <typename T>
    static void initializeArray(T* arr, size_t size);

    template <typename T>
    static void deleteArray(T* arr) ;
};

int part1_main();

#endif //PART1_H