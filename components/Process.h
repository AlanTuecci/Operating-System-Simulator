#ifndef PROCESS_H
#define PROCESS_H
#include <vector>

constexpr int NO_PROCESS{0};    //Placeholder value used to indicate that there is no process
constexpr int NEW{1};           //Indicates that the process was just created and has not yet entered the ready-queue
constexpr int READY{2};         //Indicates that the process is in the ready queue
constexpr int RUNNING{3};       //Indicates that the process is currently running
constexpr int WAITING{4};       //Indicates that the process is in an I/O queue (most likely the disk queue)
constexpr int TERMINATED{5};    //Indicates that the process is terminated. Terminated processes hold no memory or disk space

enum Type
{
    REGULAR,    //Indicates that the process has no parent and no children
    PARENT,     //Indicates that the process has at least 1 child
    CHILD,      //Indicates that the process has a parent
    ZOMBIE      //Indicates that the process is a child who's parent terminated without calling wait
};

class Process
{
public:
    //Constructors
    Process();
    Process(const int& processID);
    Process(const int& processID, const int& processState);
    Process(const int& processID, const int& processState, const Type& processType);
    Process(const int& processID, const int& processState, const Type& processType, const int& parentProcessID);
    Process(const Process& rhs);              
    Process& operator=(const Process& rhs);   
    Process(Process&& rhs);                   
    Process& operator=(Process&& rhs);

    //Setters
    void setProcessID(const int& processID);
    void setProcessState(const int& processState);
    void setProcessType(const Type& processType);
    void addChildProcessID(const int& childProcessID);
    void removeChildProcessFromRecord(const int& childProcessID);

    //Getters
    int getProcessID() const;
    int getProcessState() const;
    Type getProcessType() const;
    std::vector<int> getChildProcesses() const;
    int getParentProcessID() const;

    //Overloaded Operators
    bool operator==(const Process& rhs) const;
private:
    int processID_;
    int processState_;
    Type processType_;
    std::vector<int> childProcesses_;   //contains the PIDs of the process's children
    int parentProcessID_;
};

// It is ok for a process to run without having any pages in this simulation
// Not in real life though

#endif