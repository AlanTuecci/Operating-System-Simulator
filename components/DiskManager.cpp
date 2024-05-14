// Alan Tuecci
#include "DiskManager.h"

//--------------------------------------------Constructors--------------------------------------------

/*
    @post   Disk Manager object initialized with placeholder values.
*/
DiskManager::DiskManager() : diskQueue_{}
{
    FileReadRequest noJobFile{0, ""};
    Process noJobProcess(0, NO_PROCESS);
    currentJob_.first = noJobFile;
    currentJob_.second = noJobProcess;
}

//--------------------------------------------Setters--------------------------------------------

/*
    @param  A const lvalue reference to deque of pairs of FileReadRequest and Process objects.
    @post   The disk queue is updated to be an exact copy of the parameter.
*/
void DiskManager::setDiskQueue(const std::deque<std::pair<FileReadRequest, Process>> &diskQueue)
{
    diskQueue_ = diskQueue;
}

/*
    @param  A const lvalue reference to a FileReadRequest object.
    @post   The current FileReadRequest object is updated to be an exact copy of the parameter.
*/
void DiskManager::setCurrentFileReadRequest(const FileReadRequest &fileReadRequest)
{
    currentJob_.first = fileReadRequest;
}

/*
    @param  A const lvalue reference to a Process object.
    @post   The current Process object is updated to be an exact copy of the parameter.
*/
void DiskManager::setCurrentProcess(const Process &process)
{
    currentJob_.second = process;
}

/*
    @param  A const lvalue reference to a pair of FileReadRequests and Processes.
    @post   The current Job pair is updated to be an exact copy of the parameter.
*/
void DiskManager::setCurrentJob(const std::pair<FileReadRequest, Process> &job)
{
    currentJob_ = job;
}

//--------------------------------------------Getters--------------------------------------------

/*
    @return The disk queue.
*/
std::deque<std::pair<FileReadRequest, Process>> DiskManager::getDiskQueue() const
{
    return diskQueue_;
}

/*
    @return The current FileReadRequest object.
*/
FileReadRequest DiskManager::getCurrentFileReadRequest()
{
    return currentJob_.first;
}

/*
    @return The current Process object.
*/
Process DiskManager::getCurrentProcess() const
{
    return currentJob_.second;
}

/*
    @return The current pair of FileReadRequests and Processes.
*/
std::pair<FileReadRequest, Process> DiskManager::getCurrentJob() const
{
    return currentJob_;
}

/*
    @return A deque of waiting FileReadRequests.
*/
std::deque<FileReadRequest> DiskManager::getWaitingFileReadRequests()
{
    std::deque<FileReadRequest> waitingFileReadRequests;
    for(std::deque<std::pair<FileReadRequest, Process>>::iterator i = diskQueue_.begin(); i != diskQueue_.end(); i++)
    {
        waitingFileReadRequests.push_back(i->first);
    }
    return waitingFileReadRequests;
}

//--------------------------------------------Utilities--------------------------------------------

/*
    @param  A const lvalue reference to a pair of FileReadRequests and Processes.
    @post   The job is either handled immediately or is sent to the IO queue depending on whether or not theres already a job being handled.
*/
void DiskManager::addToQueue(const std::pair<FileReadRequest, Process> &job)
{
    if(currentJob_.second.getProcessState() == NO_PROCESS)
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
    currentJob_.first.fileName = "";
    currentJob_.first.PID = 0;

    Process emptyProcess(0, NO_PROCESS);
    currentJob_.second = emptyProcess;
}