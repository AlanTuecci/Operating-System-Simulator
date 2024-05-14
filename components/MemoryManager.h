// Alan Tuecci
#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include "Process.h"

struct MemoryItem
{
    unsigned long long pageNumber;
    unsigned long long frameNumber;
    int PID; // PID of the process using this frame of memory
};

using MemoryUsage = std::vector<MemoryItem>;

class MemoryManager
{
public:
    //Constructors
    MemoryManager();
    MemoryManager(const unsigned long long& amountOfRam, const unsigned int& pageSize);
    
    //Setters
    void setAmountOfRAM(const unsigned long long& amountOfRam);
    void setPageSize(const unsigned int& pageSize);
    void setMemoryUsage(const MemoryUsage& memory);

    //Getters
    unsigned long long getAmountOfRAM() const;
    unsigned int getPageSize() const;
    MemoryUsage getMemoryUsage() const;

    //Utilites
    void accessMemoryAtAddress(const int& processID, const unsigned long long& address);
    unsigned long long getPageNumber(const unsigned long long& address);
    void findAndClearMemoryUsedByAProcess(const int& processID);
private:
    unsigned long long amountOfRam_;
    unsigned int pageSize_;
    unsigned int numPages_;
    
    MemoryUsage memory_;
    std::deque<unsigned long long> previouslyUsedFrames_;
};
#endif