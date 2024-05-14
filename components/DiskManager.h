// Alan Tuecci
#ifndef DISKMANAGER_H
#define DISKMANAGER_H
#include "Process.h"
#include <iostream>
#include <deque>

struct FileReadRequest
{
    int PID{0};
    std::string fileName{""};
};

class DiskManager
{
public:
    //Constructors
    DiskManager();

    //Setters
    void setDiskQueue(const std::deque<std::pair<FileReadRequest,Process>>& diskQueue);
    void setCurrentFileReadRequest(const FileReadRequest& fileReadRequest);
    void setCurrentProcess(const Process& process);
    void setCurrentJob(const std::pair<FileReadRequest,Process>& job);

    //Getters
    std::deque<std::pair<FileReadRequest,Process>> getDiskQueue() const;
    FileReadRequest getCurrentFileReadRequest();
    Process getCurrentProcess() const;
    std::pair<FileReadRequest, Process> getCurrentJob() const;
    std::deque<FileReadRequest> getWaitingFileReadRequests();

    //Utilities
    void addToQueue(const std::pair<FileReadRequest, Process>& job);
    void serveNextProcess();
    void clearCurrentJob();
private:
    std::pair<FileReadRequest, Process> currentJob_;
    std::deque<std::pair <FileReadRequest, Process>> diskQueue_;
};
#endif