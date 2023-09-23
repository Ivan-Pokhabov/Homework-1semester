﻿#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ARRAYLENGTH 10000

void insertionSort(int array[], const int leftBorder, const int rightBorder) {
    for (int k = leftBorder; k < rightBorder + 1; k++)
    {
        for (int i = k; i > 0 && array[i - 1] > array[i]; i--)
        {
            const int swap = array[i];
            array[i] = array[i - 1];
            array[i - 1] = swap;
        }
    }
}

void qsort(int array[], const int leftBorder, const int rightBorder)
{
    if (rightBorder - leftBorder <= 9)
    {
        insertionSort(array, leftBorder, rightBorder);
        return;
    }
    const int supportElement = array[(rightBorder + leftBorder) / 2];
    int leftIndex = leftBorder;
    int rightIndex = rightBorder;
    do
    {
        while (supportElement > array[leftIndex]) leftIndex++;
        while (supportElement < array[rightIndex]) rightIndex--;
        if (leftIndex <= rightIndex)
        {
            const int swap = array[leftIndex];
            array[leftIndex] = array[rightIndex];
            array[rightIndex] = swap;
            leftIndex++;
            rightIndex--;
        }
    } while (leftIndex <= rightIndex);
    if (leftBorder < rightIndex)
    {
        qsort(array, leftBorder, rightIndex);
    }
    if (leftIndex < rightBorder)
    {
        qsort(array, leftIndex, rightBorder);
    }
}

bool isSortedTest(int array[], const int arrayLength)
{
    for (int i = 0; i < arrayLength; i++)
    {
        if (array[i] < array[i - 1])
        {
            return false;
        }
        return true;
    }
}

bool insertionSortTest(void)
{
    int array1[ARRAYLENGTH] = { 0 };
    int array2[ARRAYLENGTH] = { 0 };
    int array3[ARRAYLENGTH] = { 0 };
    for (int i = 0; i < ARRAYLENGTH; i++)
    {
        array1[i] = i;
        array2[i] = 100000 - i;
        array3[i] = rand() % 1000;
    }
    insertionSort(array1, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array1, ARRAYLENGTH))
    {
        printf("Insertion sort is not working with array1");
        return false;
    }
    insertionSort(array2, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array2, ARRAYLENGTH))
    {
        printf("Insertion sort is not working with array2");
        return false;
    }
    insertionSort(array3, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array3, ARRAYLENGTH))
    {
        printf("Insertion sort is not working with array3(random numbers) :\n");
        for (int i = 0; i < ARRAYLENGTH; i++)
        {
            printf("%d ", array3[i]);
        }
        return false;
    }
    return true;
}

bool qsortTest(void)
{
    int array1[ARRAYLENGTH] = { 0 };
    int array2[ARRAYLENGTH] = { 0 };
    int array3[ARRAYLENGTH] = { 0 };
    for (int i = 0; i < ARRAYLENGTH; i++)
    {
        array1[i] = i;
        array2[i] = 100000 - i;
        array3[i] = rand() % 1000;
    }
    qsort(array1, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array1, ARRAYLENGTH))
    {
        printf("Qsort is not working with array1");
        return false;
    }
    qsort(array2, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array2, ARRAYLENGTH))
    {
        printf("Qsort is not working with array2");
        return false;
    }
    qsort(array3, 0, ARRAYLENGTH - 1);
    if (!isSortedTest(array3, ARRAYLENGTH))
    {
        printf("Qsort is not working with array3(random numbers) :\n");
        for (int i = 0; i < ARRAYLENGTH; i++)
        {
            printf("%d ", array3[i]);
        }
        return false;
    }
    return true;
}

bool binarySearch(int array[], const int arrayLength, const int searchingElement)
{
    int leftBorder = 0;
    int rightBorder = arrayLength;
    while (leftBorder < rightBorder - 1)
    {
        const int middle = (leftBorder + rightBorder) / 2;
        if (array[middle] > searchingElement)
        {
            rightBorder = middle;
        }
        else
        {
            leftBorder = middle;
        }
    }
    return array[leftBorder] == searchingElement;
}

bool linearSearch(int array[], const int arrayLength, const int searchingElement)
{
    for (int i = 0; i < arrayLength; i++)
    {
        if (array[i] == searchingElement)
        {
            return true;
        }
    }
    return false;
}

bool binarySearchTest(void)
{
    int array1[15] = { 1, 3, 5, 7, 11, 13, 14, 19, 20, 21, 21, 23, 25, 27, 29 };
    int array2[15] = { 0 };
    for (int i = 0; i < 15; i++)
    {
        array2[i] = rand() % 30;
    }
    int searchingElements[30] = { 0 };
    for (int i = 0; i < 30; i++)
    {
        searchingElements[i] = rand() % 35;
    }
    qsort(array2, 0, 14);
    for (int i = 0; i < 30; i++)
    {
        if (linearSearch(array1, 15, searchingElements[i]) != binarySearch(array1, 15, searchingElements[i]))
        {
            printf("Binary search is not working with array1 and number - %d", searchingElements[i]);
            return false;
        }
    }
    for (int i = 0; i < 30; i++)
    {
        searchingElements[i] = rand() % 35;
    }
    for (int i = 0; i < 30; i++)
    {
        if (linearSearch(array2, 15, searchingElements[i]) != binarySearch(array2, 15, searchingElements[i]))
        {
            printf("Binary search is not working with array2 :\n");
            for (int i = 0; i < 15; i++)
            {
                printf("%d ", array2[i]);
            }
            printf("\nAnd number - %d", searchingElements[i]);
            return false;
        }
    }
}

int main()
{
    srand(time(NULL));
    if (!qsortTest() || !insertionSortTest() || !binarySearchTest())
    {
        return 0;
    }
    printf("xxx");
}