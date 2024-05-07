#include "DiskManager.h"

//--------------------------------------------Constructors--------------------------------------------

DiskManager::DiskManager():
    diskState_{IDLE},
    diskQueue_{},
    currentFileReadRequest_{}
{
}

//--------------------------------------------Setters--------------------------------------------

void DiskManager::setDiskState(const DiskState& diskState)
{
    diskState_ = diskState;
}

void DiskManager::setDiskQueue(const std::deque<FileReadRequest>& diskQueue)
{
    diskQueue_ = diskQueue;
}

void DiskManager::setCurrentFileReadRequest(const FileReadRequest& fileReadRequest)
{
    currentFileReadRequest_ = fileReadRequest;
}

void DiskManager::setCurrentProcess(const Process& process)
{
    currentProcess_ = process;
}

//--------------------------------------------Getters--------------------------------------------

DiskState DiskManager::getDiskState() const
{
    return diskState_;
}

std::deque<FileReadRequest> DiskManager::getDiskQueue() const
{
    return diskQueue_;
}

FileReadRequest DiskManager::getCurrentFileReadRequest()
{
    switch (diskState_)
    {
    case IDLE:
        clearCurrentFileReadRequest();
        return currentFileReadRequest_;     
    case BUSY:
        return currentFileReadRequest_;
    }
}

Process DiskManager::getCurrentProcess() const
{
    return currentProcess_;
}

//--------------------------------------------Utilities--------------------------------------------

void DiskManager::addToQueue(const FileReadRequest& fileReadRequest)
{
    diskQueue_.push_back(fileReadRequest);
}

void DiskManager::serveNextProcess()
{
    if(!diskQueue_.empty())
    {
        currentFileReadRequest_ = diskQueue_.front();
        diskQueue_.pop_front();
    }
    else
        throw std::logic_error("The disk queue is empty, so there is no other process to run!");
}

void DiskManager::clearCurrentFileReadRequest()
{
    currentFileReadRequest_.fileName = "";
    currentFileReadRequest_.PID = 0;
}

void DiskManager::addProcess(const Process& process)
{
    processQueue_.push_back(process);
}