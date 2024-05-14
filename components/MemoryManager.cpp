// Alan Tuecci
#include "MemoryManager.h"

/*
    @post   Memory Manager is initialized with no RAM, 0 pages, and a page size of 0.
*/
MemoryManager::MemoryManager():
    amountOfRam_{0},
    pageSize_{0},
    numPages_{0}
{
}

/*
    @param  A const Lvalue reference to the amount of RAM.
    @param  A const Lvalue reference to the page size.
    @post   Memory Manager is initalized with the provided arguments.
            Placeholder frames are added to a queue of memory items. 
*/
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

/*
    @param  A const lvalue reference to the amount of RAM.
    @post   The amount of RAM member is set to the value of the parameter.
*/
void MemoryManager::setAmountOfRAM(const unsigned long long& amountOfRam)
{
    amountOfRam_ = amountOfRam;
}

/*
    @param  A const lvalue reference to the page size.
    @post   The page size member is set to the value of the parameter.
*/
void MemoryManager::setPageSize(const unsigned int& pageSize)
{
    pageSize_ = pageSize;
}

/*
    @param  A const lvalue reference to a vector of MemoryItem objects.
    @post   The vector of MemoryItem objects member is set to be an exact copy of the parameter.
*/
void MemoryManager::setMemoryUsage(const MemoryUsage& memory)
{
    memory_ = memory;
}

/*
    @return The amount of RAM
*/
unsigned long long MemoryManager::getAmountOfRAM() const
{
    return amountOfRam_;
}

/*
    @return The page size
*/
unsigned int MemoryManager::getPageSize() const
{
    return pageSize_;
}

/*
    @return The vector of MemoryItem objects
*/
MemoryUsage MemoryManager::getMemoryUsage() const
{
    return memory_;
}

/*
    @param  A const lvalue reference to the process ID.
    @param  A const lvalue reference to the logical address to be accessed.
    @post   If the page associated with the provided address is already loaded, only the previouslyUsedFrames queue is updated so that it reflects that the frame was just used.
            If it isn't loaded, then it becomes loaded.
*/
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

/*
    @param  A const lvalue reference to the logical address.
    @return The page number associated with that address.
*/
unsigned long long MemoryManager::getPageNumber(const unsigned long long& address)
{
    return address / pageSize_;
}

/*
    @param  A const lvalue reference to the process ID.
    @post   Any frames used by a process with a PID that's the same as the argument are immediately deleted.
*/
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