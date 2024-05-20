// Alan Tuecci
#include "DiskManager.h"

//--------------------------------------------Constructors--------------------------------------------

/*
    @post   Disk Manager object initialized with placeholder values.
*/
DiskManager::DiskManager() : diskQueue_{}
{
    FileReadRequest noJobFile{0, ""};
    currentJob_ = noJobFile;
}

//--------------------------------------------Setters--------------------------------------------

/*
    @param  A const lvalue reference to deque of pairs of FileReadRequest and Process objects.
    @post   The disk queue is updated to be an exact copy of the parameter.
*/
void DiskManager::setDiskQueue(const std::deque<FileReadRequest> &diskQueue)
{
    diskQueue_ = diskQueue;
}

/*
    @param  A const lvalue reference to a FileReadRequest object.
    @post   The current FileReadRequest object is updated to be an exact copy of the parameter.
*/
void DiskManager::setCurrentFileReadRequest(const FileReadRequest &fileReadRequest)
{
    currentJob_ = fileReadRequest;
}

//--------------------------------------------Getters--------------------------------------------

/*
    @return The disk queue.
*/
std::deque<FileReadRequest> DiskManager::getDiskQueue() const
{
    return diskQueue_;
}

/*
    @return The current FileReadRequest object.
*/
FileReadRequest DiskManager::getCurrentFileReadRequest() const
{
    return currentJob_;
}

//--------------------------------------------Utilities--------------------------------------------

/*
    @param  A const lvalue reference to a pair of FileReadRequests and Processes.
    @post   The job is either handled immediately or is sent to the IO queue depending on whether or not theres already a job being handled.
*/
void DiskManager::addToQueue(const FileReadRequest &job)
{
    if(currentJob_.PID == 0)
        currentJob_ = job;
    else
        diskQueue_.push_back(job);
}

/*
    @post   The next request is handled or if there are no waiting requests, then the current request is cleared.
*/
void DiskManager::serveNextProcess()
{
    if(!diskQueue_.empty())
    {
        currentJob_ = diskQueue_.front();
        diskQueue_.pop_front();
    }
    else
        clearCurrentJob();
}

/*
    @post   The current request is cleared.
*/
void DiskManager::clearCurrentJob()
{
    currentJob_.fileName = "";
    currentJob_.PID = 0;
}