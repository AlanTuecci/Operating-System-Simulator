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
#include <iostream>

//Used to clear the terminal
#ifndef CLEARTERMINAL_HPP
#define CLEARTERMINAL_HPP
    #include <iostream>
    //Running Windows?
    #ifdef _WIN32
        #include <windows.h>
    //Running Linux?
    #else
        #include <cstdlib>
    #endif
    void clearTerminal() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
#endif

int main()
{
    int numHardDisks{0};
    unsigned long long ramSize{0};
    unsigned int pageSize{0};
    short int numProcesses{0};
    bool inputFlag{false};

    clearTerminal();

    std::cout << "Enter the amount of hard disks the system should have (you might want to make this a number above 1): ";
    std::cin >> numHardDisks;
    std::cout << std::endl;

    std::cout << "Enter the page size (this should be something simple, like a multiple of 10): ";
    std::cin >> pageSize;
    std::cout << std::endl;

    std::cout << "Enter the amount of RAM the system should have (this should be a multiple of the page size, otherwise the test won't work): ";
    std::cin >> ramSize;
    std::cout << std::endl;

    std::cout << "Enter the amount of processes the system should be tested with: ";
    std::cin >> numProcesses;
    std::cout << std::endl;

    std::cout << "Enter 1 to start (entering something else might kill the program unexpectedly): ";
    std::cin >> inputFlag;
    std::cout << std::endl;

    if(!inputFlag || ramSize%pageSize != 0)  //Kill the program if the user didn't listen >:(
    {
        return 0;
    }

    SimOS sim{numHardDisks, ramSize, pageSize};

//--------Testing NewProcess() with 10 processes
//--------Testing TimerInterrupt() by cycling through all the newly created processes
//--------Testing GetCPU() by checking to see if the correct PID is being returned
//--------Testing GetReadyQueue() by checking to see if the order of processes is correct
    clearTerminal();
    std::cout << "Checking NewProcess(), TimerInterrupt(), and GetCPU()" << std::endl;
    std::cout << "You should see processes numbered 1 -> " << numProcesses << " appear in order\n" << std::endl;
    
    std::cout << "Expected order of processes: \t| ";
    for(short int i = 1; i <= numProcesses; i++)
    {
        sim.NewProcess();
        std::cout << i << " | ";
    }
    std::cout << std::endl;

    std::cout << "Actual order of processes: \t| ";
    for(short int i = 0; i < numProcesses; i++)
    {
        std::cout << sim.GetCPU() <<  " | ";
        sim.TimerInterrupt();
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "Checking current and ready processes: " << std::endl;
    std::cout << "\tCurrent Process: | " << sim.GetCPU() << " | " << std::endl;
    std::deque<int> readyQueue = sim.GetReadyQueue();
    std::cout << "\tReady Processes: | ";
    for(auto i : readyQueue)
        std::cout << i << " | ";
    std::cout << std::endl << std::endl;

    std::cout << "The created and checked processes should be in the same order. If there is a mismatch, check NewProcess(), TimerInterrupt(), and GetCPU()" << std::endl;

    std::cout << std::endl;

//--------Testing DiskReadRequest()
//--------Testing GetDisk()
//--------Testing GetDiskQueue()
//--------Testing DiskJobCompleted()
    std::cout << "To proceed with testing, enter 1 (entering something else could unexpectedly terminate the program): ";
    std::cin >> inputFlag;
    std::cout << std::endl;

    if(!inputFlag)
        return 0;
    clearTerminal();

    std::cout << "Checking DiskReadRequest(), GetDisk(), GetDiskQueue(), DiskJobCompleted()" << std::endl;
    std::cout << "The processes will be the same processes used in the previous round of testing" << std::endl;
    std::cout << std::endl;
    std::cout << "Beginning the flood of DiskReadRequests. Every process will make a DiskReadRequest" << std::endl;
    std::cout << "The requested files will follow the form: 'Process-/PID/_FileNumber-/SomeNumber/" << std::endl;
    std::cout << "All disks are going to be used" << std::endl << std::endl;

    std::string tempString;
    for(short int i = 1; i <= numProcesses; i++)
    {
        tempString = "Process-" + std::to_string(sim.GetCPU()) + "_FileNumber-" + std::to_string(i);
        sim.DiskReadRequest((i-1)%numHardDisks, tempString);
    }

    std::cout << "All processes have made a FileReadRequest. There should be no process using the CPU now" << std::endl;
    std::cout << "Current Process: " << sim.GetCPU() << std::endl;

    std::cout << std::endl;
    for(int i = 0; i < numHardDisks; i++)
    {
        std::cout << "Checking the state of Hard Disk: " << i << std::endl;
        std::cout << "\tStatus\t| FileName" << std::endl;
        std::cout << "\t----------------------------------------" << std::endl;
        std::cout << "\tCurrent | " << sim.GetDisk(i).fileName << std::endl;
        std::cout << "\t----------------------------------------" << std::endl;
        for(auto x : sim.GetDiskQueue(i))
            std::cout << "\tWaiting | " << x.fileName << std::endl;
        std::cout << std::endl;
    }

    for(short int i = 1; i <= numProcesses; i++)
    {
        sim.DiskJobCompleted((i-1)%numHardDisks);
    }
    std::cout << "Completing all disk jobs" << std::endl;

    std::cout << "Checking order of processes: " << std::endl;
    std::cout << "\tCurrent Process: | " << sim.GetCPU() << " | " << std::endl;
    std::deque<int> readyQueue2 = sim.GetReadyQueue();
    std::cout << "\tReady Processes: | ";
    for(auto i : readyQueue2)
        std::cout << i << " | ";
    std::cout << std::endl << std::endl;

    std::cout << "End Test" << std::endl;
    return 0;
}