#ifndef SIMOS_H
#define SIMOS_H
#include <vector>
#include <deque>
#include "./components/CPUManager.h"
#include "./components/DiskManager.h"
#include "./components/MemoryManager.h"
#include "./components/Process.h"

class SimOS
{
public:
    SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize);
    void NewProcess();
    void SimFork();
    void SimExit();
    void SimWait();
    void TimerInterrupt();
    void DiskReadRequest(int diskNumber, std::string fileName);
    void DiskJobCompleted(int diskNumber);
    void AccessMemoryAddress(unsigned long long address);
    int GetCPU();
    std::deque<int> GetReadyQueue();
    MemoryUsage GetMemory();
    FileReadRequest GetDisk(int diskNumber);
    std::deque<FileReadRequest> GetDiskQueue(int diskNumber);

    //Helper functions
    void findParentProcessAndResumeIt();
    void findChildrenProcessesAndTerminateThem();
    std::vector<Process>::iterator findProcess(const Process& process);
private:
    CPUManager cpu_;
    std::vector<DiskManager> disks_;
    MemoryManager ram_;

    std::vector<Process> allProcesses_;
    std::vector<Process> waitingProcesses_;
};
#endif