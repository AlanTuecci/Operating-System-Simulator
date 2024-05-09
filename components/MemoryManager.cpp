#include "MemoryManager.h"

MemoryManager::MemoryManager():
    amountOfRam_{0},
    pageSize_{0},
    numPages_{0}
{
}

MemoryManager::MemoryManager(const unsigned long long& amountOfRam, const unsigned int& pageSize):
    amountOfRam_{amountOfRam},
    pageSize_{pageSize},
    numPages_{amountOfRam / pageSize}
{
}

void MemoryManager::setAmountOfRAM(const unsigned long long& amountOfRam)
{
    amountOfRam_ = amountOfRam;
}

void MemoryManager::setPageSize(const unsigned int& pageSize)
{
    pageSize_ = pageSize;
}

void MemoryManager::setMemoryUsage(const MemoryUsage& memory)
{
    memory_ = memory;
}

unsigned long long MemoryManager::getAmountOfRAM() const
{
    return amountOfRam_;
}

unsigned int MemoryManager::getPageSize() const
{
    return pageSize_;
}

MemoryUsage MemoryManager::getMemoryUsage() const
{
    return memory_;
}

void MemoryManager::accessMemoryAtAddress(const int& processID, const unsigned long long& address)
{
    recentlyUsed_.pageNumber = address / pageSize_;
    recentlyUsed_.frameNumber = -1;
    recentlyUsed_.PID = processID;
}

unsigned long long MemoryManager::getPageNumber(const unsigned long long& address)
{
    return address / pageSize_;
}