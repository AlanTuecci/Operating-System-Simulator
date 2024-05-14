// Alan Tuecci
#ifndef CPUMANAGER_H
#define CPUMANAGER_H
#include <deque>
#include <list>
#include <iostream>
#include "Process.h"

class CPUManager
{
public:
    //Constructors
    CPUManager();
    CPUManager(const CPUManager& rhs);              
    CPUManager& operator=(const CPUManager& rhs);   
    CPUManager(CPUManager&& rhs);                   
    CPUManager& operator=(CPUManager&& rhs);        
    
    //Utility Functions
    void addProcess(Process& process);
    void runFirstProcess();
    void killRunningProcess();
    void timerInterrupt();

    //Getters
    bool isBusy() const;
    Process getCurrentProcess() const;
    int getCurrentProcessID() const;
    std::deque<Process> getReadyQueue() const;
private:
    std::deque<Process> readyQueue_;
    std::list<Process> waitingProcesses_;
    Process currentProcess_;
    int cpu_;
};

#endif