#ifndef DISKMANAGER_H
#define DISKMANAGER_H
#include <iostream>
#include <deque>

struct FileReadRequest
{
    int PID{0};
    std::string fileName{""};
};

enum DiskState
{
    IDLE,
    BUSY
};

class DiskManager
{
public:
    //Constructors
    DiskManager();

    //Setters
    void setDiskState(const DiskState& diskState);
    void setDiskQueue(const std::deque<FileReadRequest>& diskQueue);
    void setCurrentFileReadRequest(const FileReadRequest& fileReadRequest);
    void setCurrentProcess(const Process& process);

    //Getters
    DiskState getDiskState() const;
    std::deque<FileReadRequest> getDiskQueue() const;
    FileReadRequest getCurrentFileReadRequest();
    Process getCurrentProcess() const;

    //Utilities
    void addToQueue(const FileReadRequest& fileReadRequest);
    void serveNextProcess();
    void clearCurrentFileReadRequest();
    void addProcess(const Process& process);
private:
    DiskState diskState_;
    std::deque<FileReadRequest> diskQueue_;
    FileReadRequest currentFileReadRequest_;
    std::deque<Process> processQueue_;
    Process currentProcess_;
};
#endif

/*Note to self: You need to figure out a better way to manage processes
                Having a bunch of queues in a bunch of places make tracking things harder than they need to be
  Possible fix: Try making a vector of pairs in the SimOS class
                The pairs will have a process object and its location
  --Sunday, April 28, 2024
*/            