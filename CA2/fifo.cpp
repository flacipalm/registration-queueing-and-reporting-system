//
//  alist.cpp
//  
//
//  Created by KD on 14.01.21.
//

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "CA1.h"
using std::cout;
using std::cin;
using std::endl;

fifo::fifo(int size)
{
    maxsize = size;
    if (size < 10)
        minsize = size;
    else
        minsize = 10;
    numitems = 0;
    first = 0;
    last = -1;
    reprarray = new Person*[maxsize];
}

//template<class T> fifo<T>::~fifo()
//{
//    delete[] *reprarray;
//}

Person* fifo::operator[](int index)
{
    if ((1 <= index) && (index <= numitems))
    {
        int relindex = (index + first -1) % maxsize;
        return reprarray[relindex];
    }
    else
    {
        cout << "Error: index " << index << " out of range.\n";
        exit(EXIT_FAILURE);
    }
}

int fifo::getlength(void)
{
    return numitems;
}

bool fifo::isempty(void)
{
    if (numitems == 0)
        return true;
    else
        return false;
}

Person* fifo::front(void)
{
    if (isempty() == false)
        return reprarray[first];
    else
    {
        cout << "The queue is empty.\n";
        exit(EXIT_FAILURE);
    }
}

Person* fifo::back(void)
{
    if (isempty() == false)
    {
        int relindex = (first + numitems - 1) % maxsize;
        return reprarray[relindex];
    }
    else
    {
        cout << "The queue is empty.\n";
        exit(EXIT_FAILURE);
    }
}

 void fifo::pushback(Person *value)
{
    if (numitems == maxsize)
        allocate();
    last = ++last % maxsize;
    reprarray[last] = value;
    ++numitems;
    return;
}

Person* fifo::popfront(void)
{
    if ((numitems == maxsize / 4) && (maxsize > minsize))
        deallocate();
    if (isempty() == false)
    {
        Person* frontelement = reprarray[first];
        first = ++first % maxsize;
        --numitems;
        return frontelement;
    }
    else
    {
        cout << "The queue is empty.\n";
        exit(EXIT_FAILURE);
    }
}

void fifo::allocate(void)
{
    int newsize = 2 * maxsize;
    Person* *newarray = new Person*[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        int pos = (i + first) % maxsize;
        newarray[i] = reprarray[pos];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    first = 0;
    last = numitems -1;
    return;
}

void fifo::deallocate(void)
{
    int newsize = maxsize / 2;
    Person* *newarray = new Person*[newsize];
    for (int i = 0; i < numitems; ++i)
    {
        int pos = (i + first) % maxsize;
        newarray[i] = reprarray[pos];
    }
    delete[] reprarray;
    reprarray = newarray;
    maxsize = newsize;
    first = 0;
    last = numitems -1;
    return;
}

void fifo::deletion(Person *value){
    int find = 0;
    //to make the reprarray start from index 0 and end at index numitems-1
    Person* *newarray = new Person* [maxsize];
    for(int i=0; i<numitems; ++i){
        int pos = (i + first) % maxsize;
        newarray[i] = reprarray[pos];
    }
    delete[] reprarray;
    reprarray = newarray;
    //delete the value
    for(int i=0; i<numitems; i++){
        if(reprarray[i]->operator==(value)){
            find=1;
            for(int j = i; j<numitems-1; j++){
                reprarray[j]=reprarray[j+1];
            }
            break;
        }
    }
    //if we don't find the value
    if(find == 0){
        cout<<"the item isn't in the queue"<<endl;      //update the "first" and "last"
        first=0;
        last = numitems-1;    
        return;
    }
    first = 0;
    numitems--;     //update the "first" and "last"
    last = numitems-1;
    return;
}