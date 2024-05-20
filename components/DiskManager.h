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
    void setDiskQueue(const std::deque<FileReadRequest>& diskQueue);
    void setCurrentFileReadRequest(const FileReadRequest& fileReadRequest);

    //Getters
    std::deque<FileReadRequest> getDiskQueue() const;
    FileReadRequest getCurrentFileReadRequest() const;

    //Utilities
    void addToQueue(const FileReadRequest& job);
    void serveNextProcess();
    void clearCurrentJob();
private:
    FileReadRequest currentJob_;
    std::deque<FileReadRequest> diskQueue_;
};
#endif