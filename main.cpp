#include "SimOS.h"
#include "SimOS.cpp"
#include "./components/CPUManager.h"
#include "./components/CPUManager.cpp"
#include "./components/DiskManager.h"
#include "./components/DiskManager.cpp"
#include "./components/MemoryManager.h"
#include "./components/MemoryManager.cpp"
#include "./components/Process.h"
#include "./components/Process.cpp"
#include <deque>

int main()
{
    SimOS sim{3, 1000, 10};
    bool allTestsClean{true};

    if(sim.GetCPU() != NO_PROCESS) //Checking behavior when there is no process
    {
        allTestsClean = false;
        std::cout << "Test on line 18 fails!" << std::endl;
        std::cout << "Remember, no processes have been created, so there shouldn't be anything running!" << std::endl;
    }

    sim.NewProcess();
    if(sim.GetCPU() != 1) //Checking behavior when a process is created
    {
        allTestsClean = false;
        std::cout << "Test on line 26 fails!" << std::endl;
        std::cout << "\tRemember, when a new process is created, if there are no currently running processes, it should begin running immediately!" << std::endl;
    }

    sim.DiskReadRequest(0, "file1.txt");
    if(sim.GetCPU() != NO_PROCESS)  //Checking to see if the process is removed from CPU usage when it requests to use the disk
    {
        allTestsClean = false;
        std::cout << "Test on line 34 fails!" << std::endl;
        std::cout << "\tRemember, when a process requests to use a disk, it immediately stops running and goes into the disk Queue!" << std::endl;
    }

    FileReadRequest request{sim.GetDisk(0)};    //Checking to see if the process is sent to the Disk Queue when it requests to use the disk
    if(request.PID != 1 || request.fileName != "file1.txt")
    {
        allTestsClean = false;
        std::cout << "Test on line 42 fails!" << std::endl;
        std::cout << "\tRemember, there were no other processes attempting to use the disk, so the disk queue is empty. That means that the current disk request will be the one the process just made with PID 1 and fileName 'file1.txt'!" << std::endl;
    }

    std::deque<FileReadRequest> ioQueue0{sim.GetDiskQueue(0)};
    if(ioQueue0.size() != 0)
    {
        allTestsClean = false;
        std::cout << "Test on line 50 fails!" << std::endl;
        std::cout << "\tRemember, since there are no other processes waiting to use the disk, the disk Queue should be empty!" << std::endl;
    }

    sim.DiskJobCompleted(0);
    request = sim.GetDisk(0);
    if(request.PID != NO_PROCESS || request.fileName != "")
    {
        allTestsClean = false;
        std::cout << "Test on line 59 fails!" << std::endl;
        std::cout << "\tRemember, the disk job is completed, so the disk should not have anything using it (unless there was a process in the disk queue)!" << std::endl;
    }

    if(sim.GetCPU() != 1)
    {
        allTestsClean = false;
        std::cout << "Test on line 66 fails!" << std::endl;
        std::cout << "\tRemember, since Process 1 is done using the disk, it goes back to the ready queue. Since the ready queue is empty, Process 1 should be run immediately!" << std::endl;
    }

    std::deque<int> readyQueue{sim.GetReadyQueue()};
    if(readyQueue.size() != 0)
    {
        allTestsClean = false;
        std::cout << "Test on line 74 fails!" << std::endl;
        std::cout << "\tRemember, Process 1 just finished using the disk. There are no other processes so Process 1 begins using the CPU immediately. This means that the ready queue should be empty!" << std::endl;
    }

    sim.SimFork();
    readyQueue = sim.GetReadyQueue();
    if(readyQueue[0] != 2)
    {
        allTestsClean = false;
        std::cout << "Test on line 83 fails!" << std::endl;
        std::cout << "\tRemember, Process 1 is still running. The fork instruction just made a copy of Process 1. This new process cannot be run until process 1 is stopped!" << std::endl;
    }

    sim.TimerInterrupt();
    readyQueue = sim.GetReadyQueue();
    if(sim.GetCPU() != 2 || readyQueue[0] != 1)
    {
        allTestsClean = false;
        std::cout << "Test on line 92 fails!" << std::endl;
        std::cout << "\tRemember, timer interrupts immediately pause process execution and send them to the back of the ready queue. Since Process 1 got removed from execution, the first process in the ready-queue took its place. This means that now the current process is Process 2 and the only process in the ready queue is process 1!" << std::endl;
    }

    sim.SimExit();
    readyQueue = sim.GetReadyQueue();
    if(sim.GetCPU() != 1 || readyQueue.size() != 0)
    {
        allTestsClean = false;
        std::cout << "Test on line 101 fails!" << std::endl;
        std::cout << "\tRemember, Process 2 is currently running, and it was just killed. Process 2 is a child of Process 1, so Process 1 should immediately resume execution. So that means the only remaining process will be Process 1!" << std::endl;
    }
    
    sim.AccessMemoryAddress(140);
    MemoryUsage ram{sim.GetMemory()};
    if(ram[0].pageNumber != 14 || ram[0].PID != 1)
    {
        allTestsClean = false;
        std::cout << "Test on line 110 fails!" << std::endl;
        std::cout << "\tRemember, the page size is 10, so an address of 140 maps to frame 14, and since the current process is Process 1, the PID should be 1!" << std::endl;
    }

    sim.SimWait();
    if(sim.GetCPU() != 1)
    {
        allTestsClean = false;
        std::cout << "Test on line 118 fails!" << std::endl;
        std::cout << "\tRemember, Process 1 has a zombie-child already, so it should resume execution immediately!" << std::endl;
    }

    sim.SimExit();
    ram = sim.GetMemory();
    if(sim.GetCPU() != NO_PROCESS || ram.size() != 0)
    {
        allTestsClean = false;
        std::cout << "Test on line 127 fails!" << std::endl;
        std::cout << "\tRemember, Process 1 was the last process runnning. This means there are no other processes to replace it. Also, since there are no processes at all, there should be no ram usage!" << std::endl;
    }

    if(allTestsClean)
        std::cout << "These preliminary tests have passed!" << std::endl;

    return 0;
}