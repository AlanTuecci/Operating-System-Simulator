// Alan Tuecci
#include "MemoryManager.h"

//--------------------------------------------Constructors--------------------------------------------

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
            Placeholder frames are added to a list of memory items. 
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

//--------------------------------------------Setters--------------------------------------------

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

//--------------------------------------------Getters--------------------------------------------

/*
    @return The amount of RAM.
*/
unsigned long long MemoryManager::getAmountOfRAM() const
{
    return amountOfRam_;
}

/*
    @return The page size.
*/
unsigned int MemoryManager::getPageSize() const
{
    return pageSize_;
}

/*
    @return The vector of MemoryItem objects.
*/
MemoryUsage MemoryManager::getMemoryUsage() const
{
    return memory_;
}

//--------------------------------------------Utilities--------------------------------------------

/*
    @param  A const lvalue reference to the process ID.
    @param  A const lvalue reference to the logical address to be accessed.
    @post   If the page associated with the provided address is already loaded, only the previouslyUsedFrames list is updated so that it reflects that the frame was just used.
            If it isn't loaded, then the system checks to see if the least recently used frame is already allocated, and if it is, then the new page replaces the existing one.
*/
void MemoryManager::accessMemoryAtAddress(const int& processID, const unsigned long long& address)
{
    //If the page is already loaded, find out what frame it's in and update the previouslyUsedFrames list accordingly
    for(MemoryUsage::iterator i = memory_.begin(); i != memory_.end(); i++)
    {
        if(i->pageNumber == (address/pageSize_))
        {
            findAFrameAndSendItToBack(i->frameNumber);
            return;
        }
    }

    //Create a memoryItem object with the frame number being that of the frame that was least recently used
    MemoryItem memoryAccess{(address/pageSize_), previouslyUsedFrames_.front(), processID};

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
            The freed frames are sent to the front of the recently used frames list.
*/
void MemoryManager::findAndClearMemoryUsedByAProcess(const int& processID)
{
    for(MemoryUsage::iterator i = memory_.begin(); i < memory_.end(); )
    {
        if(i->PID == processID)
        {
            findAFrameAndSendItToFront(i->frameNumber);
            i = memory_.erase(i);
        }
        else
            ++i;
    }
}

/*
    @param  The number of the frame to mark as least recently used.
    @post   Marks the number of the frame as least recently used by sending it to the front of the previously used frames list.
*/
void MemoryManager::findAFrameAndSendItToFront(const unsigned long long& frameNumber)
{
    for(std::list<unsigned long long>::iterator i = previouslyUsedFrames_.begin(); i != previouslyUsedFrames_.end(); i++)
    {
        if(*i == frameNumber)
        {
            previouslyUsedFrames_.erase(i);
            previouslyUsedFrames_.push_front(frameNumber);
            break;
        }
    }
}

/*
    @param  The number of the frame to mark as most recently used.
    @post   Marks the number of the frame as most recently used by sending it to the back of the previously used frames list.
*/
void MemoryManager::findAFrameAndSendItToBack(const unsigned long long& frameNumber)
{
    for(std::list<unsigned long long>::iterator i = previouslyUsedFrames_.begin(); i != previouslyUsedFrames_.end(); i++)
    {
        if(*i == frameNumber)
        {
            previouslyUsedFrames_.erase(i);
            previouslyUsedFrames_.push_back(frameNumber);
            break;
        }
    }
}