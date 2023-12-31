﻿#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Phonebook.h"

typedef struct PhonebookNote
{
    char name[100];
    char number[100];
    struct PhonebookNote* next;
} PhonebookNote;

struct Phonebook
{
    PhonebookNote* head;
    PhonebookNote* back;
};

Phonebook* createPhonebook(void)
{
    return calloc(1, sizeof(Phonebook));
}

bool isEmpty(const Phonebook* const phonebook, PhonebookErrorCode* errorCode)
{
    if (phonebook == NULL)
    {
        *errorCode = nullptr;
        return true;
    }
    return phonebook->head == NULL;
}

PhonebookErrorCode add(Phonebook* const phonebook, const char* const name, const char* const number)
{
    PhonebookNote* newNote = calloc(1, sizeof(PhonebookNote));
    PhonebookErrorCode errorCode = ok;
    if (newNote == NULL)
    {
        return memoryError;
    }
    strcpy_s(newNote->name, 100, name);
    strcpy_s(newNote->number, 100, number);
    if (isEmpty(phonebook, &errorCode))
    {
        if (errorCode != ok)
        {
            free(newNote);
            return errorCode;
        }
        phonebook->head = newNote;
        phonebook->back = newNote;
        return ok;
    }
    phonebook->back->next = newNote;
    phonebook->back = newNote;
    return ok;
}

PhonebookErrorCode print(const Phonebook* const phonebook)
{
    PhonebookErrorCode errorCode = ok;
    if (isEmpty(phonebook, &errorCode))
    {
        return (errorCode == ok) ? nullptr : errorCode;
    }
    PhonebookNote* current = phonebook->head;
    while (current != NULL)
    {
        printf("Имя - %s, номер - %s\n", current->name, current->number);
        current = current->next;
    }
    printf("\n");
    return ok;
}

PhonebookErrorCode getFileData(const char fileName[], Phonebook* phonebook, size_t* size)
{
    FILE* file = NULL;
    fopen_s(&file, fileName, "r");
    if (file == NULL)
    {
        return fileOpeningError;
    }
    int column = 0;
    if (phonebook == NULL)
    {
        fclose(file);
        return nullptr;
    }
    char name[100] = { 0 };
    char number[100] = { 0 };
    while (!feof(file))
    {
        char buffer[100] = { 0 };
        const int readBytes = fscanf_s(file, "%s", buffer, 100);
        if (readBytes < 0)
        {
            break;
        }
        if (column == 0)
        {
            strcpy_s(name, 100, buffer);
        }
        else
        {
            strcpy_s(number, 100, buffer);
            add(phonebook, name, number);
            ++(*size);
        }
        column ^= 1;
    }
    fclose(file);
    return ok;
}

int compare(PhonebookNote* first, PhonebookNote* second, const int parameter)
{
    return (parameter == 1) ? strcmp(first->name, second->name) : strcmp(first->number, second->number);
}

PhonebookErrorCode merge(Phonebook* phonebook, const size_t leftBorder, const size_t middle, const size_t rightBorder, const int parameter)
{
    PhonebookNote* current = phonebook->head;
    size_t leftIndex = leftBorder;
    size_t rightIndex = middle;
    for (size_t i = 0; i < leftBorder; ++i)
    {
        current = current->next;
        if (current == NULL)
        {
            return invalidIndex;
        }
    }
    PhonebookNote* leftSubPhonebookElement = current;
    PhonebookNote* rightSubPhonebookElement = NULL;
    for (size_t i = leftBorder; i < middle; ++i)
    {
        current = current->next;
        if (current == NULL)
        {
            return invalidIndex;
        }
    }
    rightSubPhonebookElement = current;
    Phonebook* sortedPhonebook = createPhonebook();
    while (leftIndex < middle && rightIndex < rightBorder)
    {
        if (compare(leftSubPhonebookElement, rightSubPhonebookElement, parameter) > 0)
        {
            add(sortedPhonebook, rightSubPhonebookElement->name, rightSubPhonebookElement->number);
            rightSubPhonebookElement = rightSubPhonebookElement->next;
            ++rightIndex;
        }
        else
        {
            add(sortedPhonebook, leftSubPhonebookElement->name, leftSubPhonebookElement->number);
            leftSubPhonebookElement = leftSubPhonebookElement->next;
            ++leftIndex;
        }
    }
    while (leftIndex < middle)
    {
        add(sortedPhonebook, leftSubPhonebookElement->name, leftSubPhonebookElement->number);
        leftSubPhonebookElement = leftSubPhonebookElement->next;
        ++leftIndex;
    }
    while (rightIndex < rightBorder)
    {
        add(sortedPhonebook, rightSubPhonebookElement->name, rightSubPhonebookElement->number);
        ++rightIndex;
        rightSubPhonebookElement = rightSubPhonebookElement->next;
    }
    current = phonebook->head;
    for (size_t i = 0; i < leftBorder; ++i)
    {
        current = current->next;
        if (current == NULL)
        {
            deletePhonebook(&sortedPhonebook);
            return invalidIndex;
        }
    }
    PhonebookErrorCode errorCode = ok;
    for (size_t i = leftBorder; i < rightBorder; ++i)
    {
        if (isEmpty(sortedPhonebook, &errorCode))
        {
            deletePhonebook(&sortedPhonebook);
            return nullptr;
        }
        strcpy_s(current->name, 100, sortedPhonebook->head->name);
        strcpy_s(current->number, 100, sortedPhonebook->head->number);
        current = current->next;
        PhonebookNote* trash = sortedPhonebook->head;
        sortedPhonebook->head = sortedPhonebook->head->next;
        free(trash);
    }
    free(sortedPhonebook);
    return ok;
}

void mergeSort(Phonebook* phonebook, const size_t leftBorder, const size_t rightBorder, const int parameter, PhonebookErrorCode* errorCode)
{
    if (leftBorder + 1 >= rightBorder)
    {
        return;
    }
    if (isEmpty(phonebook, errorCode))
    {
        *errorCode = nullptr;
        return;
    }
    const size_t middle = (leftBorder + rightBorder) / 2;
    mergeSort(phonebook, leftBorder, middle, parameter, errorCode);
    mergeSort(phonebook, middle, rightBorder, parameter, errorCode);
    merge(phonebook, leftBorder, middle, rightBorder, parameter);
}

void deletePhonebook(Phonebook** phonebook)
{
    PhonebookErrorCode errorCode = ok;
    while (!isEmpty(*phonebook, &errorCode))
    {
        PhonebookNote* trash = (*phonebook)->head;
        (*phonebook)->head = (*phonebook)->head->next;
        free(trash);
    }
    free(*phonebook);
    *phonebook = NULL;
}

bool checkSort(const Phonebook* const phonebook, const int parameter)
{
    PhonebookErrorCode errorCode = ok;
    if (isEmpty(phonebook, &errorCode))
    {
        return true;
    }
    PhonebookNote* current = phonebook->head->next;
    PhonebookNote* previous = phonebook->head;
    while (current != nullptr)
    {
        const int comparison = compare(previous, current, parameter);
        if (comparison < 0)
        {
            return false;
        }
        previous = current;
        current = current->next;
    }
    return true;
}