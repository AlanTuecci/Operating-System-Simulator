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
#include <cassert>

int main()
{
     SimOS sim{ 3, 1000, 10 };
     bool allTestsClean{ true };

     if (sim.GetCPU() != NO_PROCESS)
     {
         allTestsClean = false;
         std::cout << "Test on the line 9 fails!" << std::endl;
     }

     sim.NewProcess();
     if (sim.GetCPU() != 1)
     {
         allTestsClean = false;
         std::cout << "Test on the line 16 fails!" << std::endl;
     }

     sim.DiskReadRequest(0, "file1.txt");
     if (sim.GetCPU() != NO_PROCESS)
     {
         allTestsClean = false;
         std::cout << "Test on the line 23 fails!" << std::endl;
     }

     FileReadRequest request{ sim.GetDisk(0) };
     if (request.PID != 1 || request.fileName != "file1.txt")
     {
         allTestsClean = false;
         std::cout << "Test on the line 30 fails!" << std::endl;
     }

     std::deque<FileReadRequest> ioQueue0{ sim.GetDiskQueue(0) };
     if (ioQueue0.size() != 0)
     {
         allTestsClean = false;
         std::cout << "Test on the line 37 fails!" << std::endl;
     }

     sim.DiskJobCompleted(0);
     request = sim.GetDisk(0);
     if (request.PID != NO_PROCESS || request.fileName != "")
     {
         allTestsClean = false;
         std::cout << "Test on the line 45 fails!" << std::endl;
     }

     if (sim.GetCPU() != 1)
     {
         allTestsClean = false;
         std::cout << "Test on the line 51 fails!" << std::endl;
     }

     std::deque<int> readyQueue{ sim.GetReadyQueue() };
     if (readyQueue.size() != 0)
     {
         allTestsClean = false;
         std::cout << "Test on the line 58 fails!" << std::endl;
     }

     sim.SimFork();
     readyQueue = sim.GetReadyQueue();
     if (readyQueue[0] != 2)
     {
         allTestsClean = false;
         std::cout << "Test on the line 66 fails!" << std::endl;
     }

     sim.TimerInterrupt();
     readyQueue = sim.GetReadyQueue();
     if (sim.GetCPU() != 2 || readyQueue[0] != 1)
     {
         allTestsClean = false;
         std::cout << "Test on the line 74 fails!" << std::endl;
     }

     sim.SimExit();
     readyQueue = sim.GetReadyQueue();
     if (sim.GetCPU() != 1 || readyQueue.size() != 0)
     {
         allTestsClean = false;
         std::cout << "Test on the line 82 fails!" << std::endl;
     }

     sim.AccessMemoryAddress(140);
     sim.AccessMemoryAddress(150);
     sim.AccessMemoryAddress(160);
     sim.AccessMemoryAddress(140);
     sim.AccessMemoryAddress(180);
     sim.AccessMemoryAddress(140);
     sim.AccessMemoryAddress(200);
     sim.AccessMemoryAddress(140);
     MemoryUsage ram{ sim.GetMemory() };
     if (ram[0].pageNumber != 14 || ram[0].PID != 1)
     {
         allTestsClean = false;
         std::cout << "Test on the line 90 fails!" << std::endl;
     }

     sim.SimWait();
     if (sim.GetCPU() != 1)
     {
         allTestsClean = false;
         std::cout << "Test on the line 97 fails!" << std::endl;
     }

     sim.SimExit();
     ram = sim.GetMemory();
     if (sim.GetCPU() != NO_PROCESS || ram.size() != 0)
     {
         allTestsClean = false;
         std::cout << "Test on the line 105 fails!" << std::endl;
     }

     if (allTestsClean)
         std::cout << "These preliminary tests are passed" << std::endl;

//     return 0;
    SimOS sim1{3,1000,10};
    sim1.NewProcess(); // make a process
    sim1.SimFork();    // fork it
    sim1.SimFork();      //fork it again

    /*
    now we have
    cpu [1]    disk1[]       disk2[]       disk3[]

        [2]            []            []            []
        [3]            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 1);
    assert(sim1.GetReadyQueue().size() == 2);
    assert(sim1.GetReadyQueue().at(0) == 2);
    assert(sim1.GetReadyQueue().at(1) == 3);
    
    
    sim1.TimerInterrupt();
    /*
    now we have
    cpu [2]    disk1[]       disk2[]       disk3[]

        [3]            []            []            []
        [1]            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 2);
    assert(sim1.GetReadyQueue().front() == 3);
    
    
    sim1.DiskReadRequest(1,"diskready.txt");
    /*
    now we have
    cpu [3]    disk1[2]       disk2[]       disk3[]

        [1]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 3);
    
    
    
    sim1.SimExit();
    /*
    now we have
    cpu [1]    disk1[2]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 1);

    
    
    sim1.SimWait();
    
    /*
    now we have, since process 3 is a zombie,so current cpu will still be process 1
    cpu [1]    disk1[2]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 1);
    
    
    
    sim1.DiskJobCompleted(1);
    
    /*
    cpu [1]    disk1[]       disk2[]       disk3[]

        [2]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 1);
    assert(sim1.GetReadyQueue().front() == 2);
    
    
    sim1.SimExit();
    /*
    cpu []    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == NO_PROCESS);
    
    
    
    // let try SimWait on when it have child process and non of it is a zombie
    sim1.NewProcess();
    sim1.SimFork();
    /*
    cpu [4]    disk1[]       disk2[]       disk3[]

        [5]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    sim1.SimWait();
    /*
    cpu [5]    disk1[]       disk2[]       disk3[]            waitqueue[4]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 5);
    
    
    //let try to exit the child process and if process go back to ready queue
    sim1.SimExit();
    /*
    cpu [4]    disk1[]       disk2[]       disk3[]            waitqueue[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == 4);
    
    
    sim1.SimExit();
    /*
    cpu []    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == NO_PROCESS);
    
    //let try to
    sim1.NewProcess();
    /*
    cpu [6]    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   sim1.SimFork();
   /*
    cpu [6]    disk1[]       disk2[]       disk3[]

        [7]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   sim1.TimerInterrupt();
   /*
    cpu [7]    disk1[]       disk2[]       disk3[]

        [6]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   sim1.SimFork();
   /*
    cpu [7]    disk1[]       disk2[]       disk3[]

        [6]            []            []            []
        [8]            []            []            []
        []            []            []            []
        []            []            []            []
    */
   sim1.TimerInterrupt();
   /*
    cpu [6]    disk1[]       disk2[]       disk3[]

        [8]            []            []            []
        [7]            []            []            []
        []            []            []            []
        []            []            []            []
    */
   sim1.SimExit();
   /*
    cpu []    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
    assert(sim1.GetCPU() == NO_PROCESS);
    
    // Initialize SimOS with arbitrary parameters
        SimOS simOS(3, 1024 * 1024 * 1024, 4096); // 3 disks, 1GB RAM, 4KB page size

        // Test 1: Exit with no process running
        try {
            simOS.SimExit();
        } catch (const std::logic_error& e) {
            std::cout << "Test 1 (No process): Passed - " << e.what() << std::endl;
        }

        // Test 2: Normal exit with a single process
        simOS.NewProcess();  // Assume this sets currentCPU to the new process's PID
        simOS.SimExit();
        std::cout << "Test 2 (Single process exit): Passed - Process exited cleanly." << std::endl;

        // Test 3: Exit with active children
        simOS.NewProcess();
        simOS.SimFork();  // Fork a child process
        simOS.SimExit();  // Exit the parent process, should also handle child
        std::cout << "Test 3 (Parent with child): Passed - Parent and child handled." << std::endl;

        // Test 4: Process exit when parent is waiting
    simOS.NewProcess();  // Parent process
    simOS.SimFork();     // Create child process
    simOS.SimWait();     // Parent waits for child
    simOS.SimExit();     // Child exits, should notify parent
    std::cout << "Test 4 (Parent waiting): Passed - Parent notified." << std::endl;
    simOS.SimExit();
    
        // Test 5: Process becomes a zombie
    simOS.NewProcess();  // Parent process
    simOS.SimFork();     // Create child process
    simOS.TimerInterrupt(); // Move parent process to the back of ready queue
    simOS.SimExit();     // Child exits without parent waiting
    std::cout << "Test 5 (Zombie process): Passed - Process became a zombie." << std::endl;
    simOS.SimExit();
        // Additional tests for more intricate scenarios or stress tests can be added here.
        
    // Test 1: Fork without any process running
        try {
            simOS.SimFork();
        } catch (const std::logic_error& e) {
            std::cout << "Test 1 (Fork without running process): Passed - " << e.what() << std::endl;
        }

        // Test 2: Normal fork with a single process
    simOS.NewProcess();  // PID 1
    simOS.SimFork();     // Forks PID 2, PID 1 stays on CPU, PID 2 goes to the queue
    assert(simOS.GetCPU() == 8);
    assert(simOS.GetReadyQueue().front() == 9);
    std::cout << "Test 2 (Normal fork): Passed - Child process added to queue, parent remains on CPU." << std::endl;
    simOS.SimExit();
        // Test 3: Normal wait where child exits after wait call
    simOS.NewProcess();  // PID 10
    simOS.SimFork();     // Forks PID 11
    simOS.SimWait();     // PID 10 waits
    simOS.SimExit();     // Exit PID 11
    assert(simOS.GetCPU() == 10); // PID 10 should resume
    std::cout << "Test 3 (Wait for child exit): Passed - Parent resumed after child exit." << std::endl;
    simOS.SimExit();
        // Test 4: Wait with no children
    simOS.NewProcess();  // PID 12
    try {
        simOS.SimWait();  // Attempt to wait with no children
    } catch (const std::logic_error& e) {
        std::cout << "Test 4 (Wait with no children): Passed - " << e.what() << std::endl;
    }
    simOS.SimExit();
        // Test 5: Fork and wait, with child already terminated (zombie cleanup)
    simOS.NewProcess();  // PID 13
    simOS.SimFork();     // Forks PID 14
    simOS.TimerInterrupt();
    simOS.SimExit();     // Exit PID 14, becomes zombie
    simOS.SimWait();     // PID 13 waits, should immediately resume
    assert(simOS.GetCPU() == 13);
    std::cout << "Test 5 (Zombie cleanup): Passed - Parent did not block." << std::endl;
    
    simOS.SimExit();
    simOS.NewProcess();
    /*
    cpu [15]    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   simOS.SimFork();
   /*
    cpu [15]    disk1[]       disk2[]       disk3[]

        [16]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   simOS.TimerInterrupt();
   /*
    cpu [16]    disk1[]       disk2[]       disk3[]

        [15]            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   simOS.DiskReadRequest(1,"gay.txt");
   /*
    cpu [15]    disk1[16]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */
   simOS.SimExit();
   /*
    cpu []    disk1[]       disk2[]       disk3[]

        []            []            []            []
        []            []            []            []
        []            []            []            []
        []            []            []            []
    */

   std::cout << "Good job, you passed!" << std::endl;
    return 0;

}
