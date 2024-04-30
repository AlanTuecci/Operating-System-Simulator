#include "SimOS.h"

/*
    @param  Number of hard disks in the simulated computer
    @param  Amount of RAM memory
    @param  Page size
    @post   Disks, frame, and page enumerations all start from 0
*/
SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize):
    ram_{amountOfRAM, pageSize},
    cpu_{},
    allProcesses_{},
    waitingProcesses_{}
{
    for(int i = 0; i < numberOfDisks; i++)
    {
        DiskManager disk;
        disks_.push_back(disk);
    }
    Process OS_PROCESS(0, NO_PROCESS);
    allProcesses_.push_back(OS_PROCESS);
}

/*
    @post   Creates a new process in the simulated system.
            The new process takes place in the ready-queue or immediately starts using the CPU.
            Every process in the simulated system has a PID.
            The simulation assigns PIDs to new processes starting from 1 and incrememnts it by one for each new process.
            The PIDs of terminated processes will NOT be reused.
*/
void SimOS::NewProcess()
{
    Process newProcess(allProcesses_.size(), NEW);
    allProcesses_.push_back(newProcess);
    cpu_.addProcess(newProcess);
}

/*
    @post   Forks the currently running process forks a child.
            The child is placed in the end of the ready-queue.

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::SimFork()
{
  //This method needs updating because at the time of writing, process's memory behavior is undefined
    if(cpu_.isBusy()){
        std::vector<Process>::iterator currProcess = findProcess(cpu_.getCurrentProcess());
        currProcess->setProcessType(PARENT);
        Process childProcess(allProcesses_.size(), NEW, CHILD, currProcess->getProcessID());
        currProcess->addChildProcessID(childProcess.getProcessID());
        cpu_.addProcess(childProcess);
    }
    else
        throw std::logic_error("This instruction requires a running process");
}

/*
    @post   The process that is currently using the CPU terminates.
            Any memory used by the process is released immediately.
            If the parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue).
            If a parent hasn't called wait yet, the process turns into a zombie process.
            To avoid the appearance of the orphans, the system implements the cascading termination.
            In this context, cascading termination means that if a process terminates, all its descendants terminate with it.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
    @note   The system will be implementing cascading termination
            This means that if a process terminates, all of its children will terminate with it as well
*/
void SimOS::SimExit()
{
    if(cpu_.isBusy()){
        std::vector<Process>::iterator currProcess = findProcess(cpu_.getCurrentProcess());
        switch (currProcess->getProcessType())
        {
        case PARENT:
            findChildrenProcessesAndTerminateThem();
            findParentProcessAndResumeIt(); //Parents could have parents, and if they do, and they're waiting, find them and resume them
            // Remember to free up memory
            break;
        case CHILD:
            findParentProcessAndResumeIt();
            // Remember to free up memory
            break;
        }
        currProcess->setProcessState(TERMINATED);
        cpu_.runFirstProcess();
    }
    else
        throw std::logic_error("This instruction requires a running process");
}

/*
    @post   The process wants to pause and wait for any of its child processes to terminate.
            Once the wait is over, the process goes to the end of the ready queue or the CPU.
            If the zombie-child already exists, the process proceeds right away (keeps using the CPU) and the zombie-child process disappears.
            If more than one zombie-child exists, the system uses one of them (any one of them) to immediately resumes the parent, while other zombies keep waiting for the next wait from the parent.

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::SimWait()
{
    if(cpu_.isBusy())
    {
        std::vector<Process>::iterator currProcess = findProcess(cpu_.getCurrentProcess());
        cpu_.runFirstProcess();
        currProcess->setProcessState(WAITING);
        waitingProcesses_.push_back(*currProcess);
        //Need to define functionality that relates to zombie-children
        //For the part where the process waits for the children to terminate in order to resume, try adding this functionality in the SimExit function
    }
    else
        throw std::logic_error("This instruction requires a running process");
}

/*
    @post   Interrupt arrives from the timer signaling that the time slice of the currently running process is over.

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.    
*/
void SimOS::TimerInterrupt()
{
    if(cpu_.isBusy())
    {
        cpu_.timerInterrupt();
    }
    else
        throw std::logic_error("This instruction requires a running process");
    
}

/*
    @param  The number of the disk that contains the file to be read.
    @param  The name of the file to be read.
    @post   Currently running process requests to read the specified file from the disk with a given number.
            The process issuing disk reading requests immediately stops using the CPU, even if the ready-queue is empty.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::DiskReadRequest(int diskNumber, std::string fileName)
{
}

/*
    @param  The number of the disk.
    @post   A disk with a specified number reports that a single job is completed.
            The served process should return to the ready-queue.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::DiskJobCompleted(int diskNumber)
{
    if(diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
    {
        std::cout << "Disk " << diskNumber << " is reporting that a single job is complete. Process " << disks_[diskNumber].getCurrentProcess().PID << " has been moved to the ready-queue." << std::endl;
        // Remember to add the functionality that puts the process back into the ready queue
    }
}

/*
    @param  The logical address in RAM.
    @param  The currently running process wants to access the specified logical memory address.
            System makes sure the corresponding page is loaded in the RAM.
            If the corresponding page is already in the RAM, then its "recently used" information is updated.

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::AccessMemoryAddress(unsigned long long address)
{
    if(cpu_.isBusy())
    {
        ram_.accessMemoryAtAddress(cpu_.getCurrentProcessID(), address);
    }
    else
        throw std::logic_error("This instruction requires a running process");
}

/*
    @return The PID of the process currently using the CPU.
            If the CPU is idle, it returns NO_PROCESS.
*/
int SimOS::GetCPU()
{
    if(cpu_.isBusy())
        return cpu_.getCurrentProcess().getProcessID();
    else
        return NO_PROCESS;
}

/*
    @return The std::deque containing PIDs of processes in the ready-queue.
            Here, the front element corresponds with the start of the ready-queue.
*/
std::deque<int> SimOS::GetReadyQueue()
{
    std::deque<int> readyQueuePIDs;
    std::deque<Process> readyQueue = cpu_.getReadyQueue();
    for(int i = 0; i < readyQueue.size(); i++)
        readyQueuePIDs.push_back(readyQueue[i].getProcessID());
    return readyQueuePIDs;
}

/*
    @return The std::vector containing describing all currently used frames of RAM.
            Note:   Terminated "zombie" processes don't use memory, so they don't contribute to memory usage.
                    MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
MemoryUsage SimOS::GetMemory()
{
}

/*
    @param  The number of the disk.
    @return An object with PID of the process served by the specified disk and the name of the file read for that process.
            If the disk is idle, the default FileReadRequest object (with PID 0 and empty string in fileName) is returned.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
*/
FileReadRequest SimOS::GetDisk(int diskNumber)
{
    if(diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
        return disks_[diskNumber].getCurrentProcess();

}

/*
    @return The I/O-queue of the specified disk starting from the "next to be served" process.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
*/
std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    if(diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
        return disks_[diskNumber].getDiskQueue();
}

void SimOS::findParentProcessAndResumeIt()
{   
    bool foundWaitingParent = false; //Used for Debugging
    const int parentProcessID = cpu_.getCurrentProcess().getParentProcessID();
    if(parentProcessID != 0)
    {
        for(std::vector<Process>::iterator i = waitingProcesses_.begin(); i != waitingProcesses_.end(); i++)
        {
            if(i->getProcessID() == parentProcessID)
            {
                foundWaitingParent = true; //Used for debugging
                cpu_.addProcess(*i);
                waitingProcesses_.erase(i);
                break;
            }
        }
        //Anything below here is used for debugging
        if(!foundWaitingParent)
            std::cout << "Could not find waiting parent!" << std::endl;
        else
            std::cout << "Parent with PID: *" << parentProcessID << "* found and added back to ready-queue!" << std::endl;
    }
}

void SimOS::findChildrenProcessesAndTerminateThem()
{
    std::vector<Process>::iterator currProcess = findProcess(cpu_.getCurrentProcess());
    bool foundChildProcess = false;
    for(int i = 0; i < currProcess->getChildProcesses().size()-1; i++)
    {
        foundChildProcess = true;
        int childProcess = currProcess->getChildProcesses()[i];
        allProcesses_[childProcess].setProcessState(TERMINATED);
    }
    //Anything below is used for debugging
    if(foundChildProcess)
        std::cout << "Found children processes and terminated them!" << std::endl;
}

std::vector<Process>::iterator SimOS::findProcess(const Process& process)
{
    std::vector<Process>::iterator iteratorToProcess = allProcesses_.begin();
    for(iteratorToProcess; iteratorToProcess != allProcesses_.end(); iteratorToProcess++)
    {
        if(*iteratorToProcess == process)
            break;
    }
    if(iteratorToProcess == allProcesses_.begin())
        throw std::logic_error("The process does not exist in the Process Record!");
    else
        return iteratorToProcess;
}