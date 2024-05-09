#include "DiskManager.h"

//--------------------------------------------Constructors--------------------------------------------

DiskManager::DiskManager():
    diskQueue_{}
{
    FileReadRequest noJobFile{0, ""};
    Process noJobProcess(0, NO_PROCESS);
    currentJob_.first = noJobFile;
    currentJob_.second = noJobProcess;
}

//--------------------------------------------Setters--------------------------------------------

void DiskManager::setDiskQueue(const std::deque<std::pair<FileReadRequest, Process>>& diskQueue)
{
    diskQueue_ = diskQueue;
}

void DiskManager::setCurrentFileReadRequest(const FileReadRequest& fileReadRequest)
{
    currentJob_.first = fileReadRequest;
}

void DiskManager::setCurrentProcess(const Process& process)
{
    currentJob_.second = process;
}

void DiskManager::setCurrentJob(const std::pair<FileReadRequest, Process>& job)
{
    currentJob_ = job;
}

//--------------------------------------------Getters--------------------------------------------

std::deque<std::pair<FileReadRequest,Process>> DiskManager::getDiskQueue() const
{
    return diskQueue_;
}

FileReadRequest DiskManager::getCurrentFileReadRequest()
{
    return currentJob_.first;
}

Process DiskManager::getCurrentProcess() const
{
    return currentJob_.second;
}

std::pair<FileReadRequest, Process> DiskManager::getCurrentJob() const
{
    return currentJob_;
}

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

void DiskManager::addToQueue(const std::pair<FileReadRequest,Process>& job)
{
    if(currentJob_.second.getProcessState() == NO_PROCESS)
        currentJob_ = job;
    else
        diskQueue_.push_back(job);
}

void DiskManager::serveNextProcess()
{
    if(!diskQueue_.empty())
    {
        currentJob_ = diskQueue_.front();
        diskQueue_.pop_front();
    }
    else
    {
        std::cout << "There are no other jobs waiting. Disk is now idle!" << std::endl;
        clearCurrentJob();
    }
}

void DiskManager::clearCurrentJob()
{
    currentJob_.first.fileName = "";
    currentJob_.first.PID = 0;

    Process emptyProcess(0, NO_PROCESS);
    currentJob_.second = emptyProcess;
}