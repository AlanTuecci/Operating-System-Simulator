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

    //Setters
    void setReadyQueue(const std::deque<int>& readyQueue);

    //Getters
    bool isBusy() const;
    int getCurrentProcessID() const;
    std::deque<int> getReadyQueue() const;

    //Utility Functions
    int addProcess(const int& process);
    std::pair<int, int> runFirstProcess();
    void killRunningProcess();
    std::pair<int, int> timerInterrupt();
private:
    std::deque<int> readyQueue_;
    int currentProcess_;
};

#endif