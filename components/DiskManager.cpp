#include "DiskManager.h"

//--------------------------------------------Constructors--------------------------------------------

DiskManager::DiskManager():
    diskState_{IDLE},
    diskQueue_{},
    currentProcess_{}
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

void DiskManager::setCurrentProcess(const FileReadRequest& currentProcess)
{
    currentProcess_ = currentProcess;
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

FileReadRequest DiskManager::getCurrentProcess()
{
    switch (diskState_)
    {
    case IDLE:
        clearCurrentProcess();
        return currentProcess_;     
    case BUSY:
        return currentProcess_;
    }
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
        currentProcess_ = diskQueue_.front();
        diskQueue_.pop_front();
    }
    else
        throw std::logic_error("The disk queue is empty, so there is no other process to run!");
}

void DiskManager::clearCurrentProcess()
{
    currentProcess_.fileName = "";
    currentProcess_.PID = 0;
}