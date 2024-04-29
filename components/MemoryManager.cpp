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

MemoryItem MemoryManager::accessMemoryAtAddress(const int& processID, const unsigned long long& address)
{
    MemoryItem memoryItem;
    memoryItem.pageNumber = address / pageSize_;
    memoryItem.frameNumber = -1;
    memoryItem.PID = processID;
    
    for(std::vector<MemoryItem>::iterator i = memory_.begin(); i != memory_.end(); i++)
    {
        if((i->pageNumber == memoryItem.pageNumber) && (i->PID == memoryItem.PID))
        {
            memoryItem.frameNumber = i->frameNumber;
            return memoryItem;
        }
    }

    if(memoryItem.frameNumber == -1)
        throw std::out_of_range("The frame number could not be found for this process. (This should NEVER happen so if you get this error, check the logic).");
}