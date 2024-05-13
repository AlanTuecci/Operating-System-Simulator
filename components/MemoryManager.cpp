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
    numPages_{amountOfRam/pageSize}
{
    MemoryItem placeHolder{0, 0, 0};
    for(unsigned int i = 0; i < numPages_; i++)
    {  
        previouslyUsedFrames_.push_back(i);
    }
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
    MemoryItem memoryAccess;
    memoryAccess.pageNumber = address / pageSize_;
    memoryAccess.frameNumber = previouslyUsedFrames_.front();
    memoryAccess.PID = processID;

    //Send the frame to the back since it's now the most recently used frame
    previouslyUsedFrames_.push_back(memoryAccess.frameNumber);
    previouslyUsedFrames_.pop_front();

    //Check to see if the frame we're about to use is currently being used, and if so, replace its page with the new page
    bool frameIsBeingUsed{false};
    for(MemoryUsage::iterator i = memory_.begin(); i != memory_.end(); i++)
    {
        if(i->frameNumber == memoryAccess.frameNumber){
            frameIsBeingUsed = true;
            *i = memoryAccess;
            break;
        }
    }

    //Frame was not currently being used so it was added to memory_
    if(!frameIsBeingUsed)
        memory_.push_back(memoryAccess);
}

unsigned long long MemoryManager::getPageNumber(const unsigned long long& address)
{
    return address / pageSize_;
}

void MemoryManager::findAndClearMemoryUsedByAProcess(const int& processID)
{
    for(MemoryUsage::iterator i = memory_.begin(); i != memory_.end(); )
    {
        if(i->PID == processID)
            i = memory_.erase(i);
        else
            ++i;
    }
}