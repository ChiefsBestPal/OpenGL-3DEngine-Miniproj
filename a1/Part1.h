#ifndef PART1_H
#define PART1_H

#include <stddef.h>

template <typename T>
T* createArray(size_t size);

template <typename T>
void initializeArray(T* arr, size_t size);

template <typename T>
void deleteArray(T* arr, size_t size) ;

int part1_main();

#endif //PART1_H