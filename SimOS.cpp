// Alan Tuecci
#include "SimOS.h"

/*
    @param  Number of hard disks in the simulated computer
    @param  Amount of RAM memory
    @param  Page size
    @post   Disks, frame, and page enumerations all start from 0
*/
SimOS::SimOS(int numberOfDisks, unsigned long long amountOfRAM, unsigned int pageSize) : 
    ram_{amountOfRAM, pageSize},
    cpu_{},
    allProcesses_{},
    waitingProcesses_{}
{
    for (int i = 0; i < numberOfDisks; i++)
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
    newProcess.setProcessState(cpu_.addProcess(newProcess.getProcessID()));
    allProcesses_.push_back(newProcess);
}

/*
    @post   Forks the currently running process forks a child.
            The child is placed in the end of the ready-queue.

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
void SimOS::SimFork()
{
    if (cpu_.isBusy())
    {
        int currProcessID = cpu_.getCurrentProcessID();    
        allProcesses_[currProcessID].setProcessType(PARENT);    //Mark the current process as parent
        Process childProcess(allProcesses_.size(), NEW, CHILD, currProcessID);  //Construct the child
        allProcesses_[currProcessID].addChildProcessID(childProcess.getProcessID());    //Make sure the parent knows it has a child
        allProcesses_.push_back(childProcess);
        allProcesses_[childProcess.getProcessID()].setProcessState(cpu_.addProcess(childProcess.getProcessID()));   //Send the child to the CPU and update the process's state
    }
    else
        throw std::logic_error("This instruction requires a running process");
}

/*
    @post   The process that is currently using the CPU terminates.
            Any memory used by the process is released immediately.
            If the parent is already waiting, the process terminates immediately and the parent becomes runnable (goes to the ready-queue).
            If a parent hasn't called wait yet, the process turns into a zombie process.
            To avoid the appearance of the orphans, the system implements cascading termination.
            In this context, cascading termination means that if a process terminates, all its descendants terminate with it.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
    @note   The system will be implementing cascading termination
            This means that if a process terminates, all of its children will terminate with it as well
*/
void SimOS::SimExit()
{
    if (cpu_.isBusy())
    {
        int currentProcess = cpu_.getCurrentProcessID();
        ram_.findAndClearMemoryUsedByAProcess(currentProcess);

        // Logic for cascading termination [By the nature of SimFork(), any process with children is going to be a Parent]
        if (allProcesses_[currentProcess].getProcessType() == PARENT) 
            findChildrenProcessesAndTerminateThem(currentProcess);

        allProcesses_[currentProcess].setProcessType(ZOMBIE);

        //If there's a waiting parent and it was successfully resumed, then the current process is now terminated
        int parentProcessID = allProcesses_[currentProcess].getParentProcessID();
        if (allProcesses_[parentProcessID].getProcessState() == WAITING)
        {
            if(findParentProcessAndResumeIt(parentProcessID))
                allProcesses_[currentProcess].setProcessState(TERMINATED);
        }

        findAndRemoveZombiesOrTerminatedProcesses();    //Remove any zombies or terminated processes from the ready queue
        std::pair<int, int> newCurrentProcess = cpu_.runFirstProcess();
        allProcesses_[newCurrentProcess.first].setProcessState(newCurrentProcess.second);
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
            It is unclear what needs to be done if there are no children and the process calls wait. 
            It is assumed that the process shouldn't be calling wait unless it has children.
*/
void SimOS::SimWait()
{
    if (cpu_.isBusy())
    {
        std::vector<Process>::iterator waitProcess = findProcessByID(cpu_.getCurrentProcessID());
        if (!waitProcess->getChildProcesses().empty())  //If the process has children
        {
            bool zombieFlag = findZombieProcessAndTerminateIt();
            if(!zombieFlag && !cpu_.getReadyQueue().empty())
            {
                std::pair<int, int> newCurrentProcess = cpu_.runFirstProcess(); //Run the next process
                allProcesses_[newCurrentProcess.first].setProcessState(newCurrentProcess.second);   //Update the new current process's information in the record
                waitProcess->setProcessState(WAITING);  //Set the process state to waiting
                waitingProcesses_.push_back(*waitProcess);  //Add the process to the waiting processes vector
            }
            //If a zombie was terminated, then the process continues using the CPU like nothing happened
        }
        //If there are no children, this function shouldn't be called in the first place
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
    if (cpu_.isBusy())
    {
        int interruptedProcess = cpu_.getCurrentProcessID();
        std::pair<int, int> newCurrentProcess = cpu_.timerInterrupt();
        allProcesses_[newCurrentProcess.first].setProcessState(newCurrentProcess.second);   //Update the new current process's information in the record
        if(newCurrentProcess.first != interruptedProcess)   
            allProcesses_[interruptedProcess].setProcessState(READY);   //If the interrupted process was sent to the ready queue, update it's state in the record to reflect this
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
    if (diskNumber >= disks_.size() || diskNumber < 0)
        throw std::out_of_range("The disk with the requested number does not exist");
    else
    {
        FileReadRequest newRequest{cpu_.getCurrentProcessID(), fileName};
        disks_[diskNumber].addToQueue(newRequest);
        std::pair<int, int> newCurrentProcess = cpu_.runFirstProcess();
        allProcesses_[newCurrentProcess.first].setProcessState(newCurrentProcess.second);
    }
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
    if (diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
    {
        int finishedProcess = disks_[diskNumber].getCurrentFileReadRequest().PID;
        allProcesses_[finishedProcess].setProcessState(cpu_.addProcess(finishedProcess));
        disks_[diskNumber].serveNextProcess();
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
    if (cpu_.isBusy())
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
    if (cpu_.isBusy())
        return cpu_.getCurrentProcessID();
    else
        return NO_PROCESS;
}

/*
    @return The std::deque containing PIDs of processes in the ready-queue.
            Here, the front element corresponds with the start of the ready-queue.
*/
std::deque<int> SimOS::GetReadyQueue()
{
    return cpu_.getReadyQueue();
}

/*
    @return The std::vector containing describing all currently used frames of RAM.
            Note:   Terminated "zombie" processes don't use memory, so they don't contribute to memory usage.
                    MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high).

    @note   If instruction is called that requires a running process, but the CPU is idle, throw std::logic_error exception.
*/
MemoryUsage SimOS::GetMemory()
{
    return ram_.getMemoryUsage();
    //Specifications said that "MemoryItems appear in the MemoryUsage vector in the order they appear in memory (from low addresses to high)."
    //Does this mean that we need to sort the MemoryUsage, and if yes, do we sort by PageNumber or FrameNumber?
    //Since this was not specified, I'll implement it so that the rightmost element of the memoryusage vector contains the most recently added page.
}

/*
    @param  The number of the disk.
    @return An object with PID of the process served by the specified disk and the name of the file read for that process.
            If the disk is idle, the default FileReadRequest object (with PID 0 and empty string in fileName) is returned.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
*/
FileReadRequest SimOS::GetDisk(int diskNumber)
{
    if (diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
        return disks_[diskNumber].getCurrentFileReadRequest();
}

//--------------------------------------------Helper Functions--------------------------------------------

/*
    @return The I/O-queue of the specified disk starting from the "next to be served" process.

    @note   If a disk with the requested number doesn’t exist throw std::out_of_range exception.
*/
std::deque<FileReadRequest> SimOS::GetDiskQueue(int diskNumber)
{
    if (diskNumber >= disks_.size())
        throw std::out_of_range("The disk with the requested number does not exist");
    else
    {
        return disks_[diskNumber].getDiskQueue();
    }
}

/*
    @post   Finds the waiting parent process of the currently running process, and changes its state from Waiting to Ready.
            If parent is waiting, add it to the ready queue and delete it from the collection of waiting processes.
    @return True if the waiting parent was found and resumed, false otherwise.
*/
bool SimOS::findParentProcessAndResumeIt(const int& parentProcessID)
{
    if (parentProcessID != 0)
    {
        for (std::vector<Process>::iterator i = waitingProcesses_.begin(); i != waitingProcesses_.end(); i++)
        {
            if (i->getProcessID() == parentProcessID)
            {
                allProcesses_[i->getProcessID()].setProcessState(cpu_.addProcess(i->getProcessID()));
                waitingProcesses_.erase(i);
                return true;
            }
        }
    }
    return false;
}

/*
    @param  A const lvalue iterator to a process in the collection of all processes.
    @post   Finds, terminates, and clears any memory used by the children of the given process.
*/
void SimOS::findChildrenProcessesAndTerminateThem(const int &currProcessID)
{
    for (int i = 0; i < allProcesses_[currProcessID].getChildProcesses().size(); i++)
    {
        int childProcess = allProcesses_[currProcessID].getChildProcesses()[i];
        allProcesses_[childProcess].setProcessState(TERMINATED);
        ram_.findAndClearMemoryUsedByAProcess(childProcess);
        if(allProcesses_[childProcess].getProcessType() == PARENT)
            findChildrenProcessesAndTerminateThem(childProcess);
    }
}

/*
    @post   Finds, terminates, and clears any memory used by the children of the given process.
    @return True if found and terminated a zombie, false otherwise
*/
bool SimOS::findZombieProcessAndTerminateIt()
{
    //Iterating through the children processes vector
    std::vector<int> childProcesses = allProcesses_[cpu_.getCurrentProcessID()].getChildProcesses();
    for(auto x : childProcesses)
    {
        if(allProcesses_[x].getProcessType() == ZOMBIE){
            ram_.findAndClearMemoryUsedByAProcess(x);
            allProcesses_[x].setProcessState(TERMINATED);
            return true;
        }
    }
    return false;
}

/*
    @param  A const lvalue reference to a process.
    @return An iterator to a matching process in the collection of all processes.

    @note   Throws an exception if the iterator is pointing to Process 0 (The NO_PROCESS placeholder).
*/
std::vector<Process>::iterator SimOS::findProcess(const Process &process)
{
    std::vector<Process>::iterator iteratorToProcess = allProcesses_.begin();
    for (iteratorToProcess; iteratorToProcess != allProcesses_.end(); iteratorToProcess++)
    {
        if (*iteratorToProcess == process)
            break;
    }
        
    return iteratorToProcess;
}

/*
    @param  A const lvalue reference to the proces ID.
    @return An iterator in the allProcesses_ vector pointing to the location of the process.
*/
std::vector<Process>::iterator SimOS::findProcessByID(const int &processID)
{
    std::vector<Process>::iterator iteratorToProcess = allProcesses_.begin();
    for (iteratorToProcess; iteratorToProcess != allProcesses_.end(); iteratorToProcess++)
    {
        if (iteratorToProcess->getProcessID() == processID)
            break;
    }
        
    return iteratorToProcess;
}

/*
    @post   Removes any terminated or zombie processes from the ready queue.
*/
void SimOS::findAndRemoveZombiesOrTerminatedProcesses()
{
    std::deque<int> origReadyQueue = cpu_.getReadyQueue();
    std::deque<int> SanitizedReadyQueue;
    for(int i = 0; i < origReadyQueue.size(); i++)
    {
        int pid = origReadyQueue.front();
        if(allProcesses_[pid].getProcessState() == READY && allProcesses_[pid].getProcessType() != ZOMBIE)
        {
            SanitizedReadyQueue.push_back(pid);
        }
        origReadyQueue.pop_front();
    }
    cpu_.setReadyQueue(SanitizedReadyQueue);
}