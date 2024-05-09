#include "CPUManager.h"

/*
    @post   Ready queue is initialized with 0 elements
            CPU ID initialized to 0
            Current Process is set to some dummy process with PID 0 and state 'NO_PROCESS'
*/
CPUManager::CPUManager(): 
    readyQueue_{0}, cpu_{0},
    currentProcess_{0, NO_PROCESS}
{
}

/*
    @param  A const Lvalue reference to a CPUManager object
    @post   CPUManager object constructed to be an exact copy of the parameter object
*/
CPUManager::CPUManager(const CPUManager& rhs):
    readyQueue_{std::move(rhs.readyQueue_)}, 
    currentProcess_{std::move(rhs.currentProcess_)}, 
    cpu_{rhs.cpu_}
{
}   

/*
    @param  A const Lvalue reference to a CPUManager object
    @post   CPUManager object is set to be an exact copy of the parameter object
    @return A reference to the CPUManager object
*/
CPUManager& CPUManager::operator=(const CPUManager& rhs)
{
    readyQueue_ = std::move(rhs.readyQueue_);
    currentProcess_ = std::move(rhs.currentProcess_);
    cpu_ = rhs.cpu_;

    return *this;
}  

/*
    @param  An Rvalue reference to a CPUManager object
    @post   CPUManager object constructed to be an exact copy of the parameter object
*/
CPUManager::CPUManager(CPUManager&& rhs):
    readyQueue_{std::move(rhs.readyQueue_)}, 
    currentProcess_{std::move(rhs.currentProcess_)},
    cpu_{rhs.cpu_}
{
}          

/*
    @param  An Rvalue reference to a CPUManager object
    @post   CPUManager object is set to be an exact copy of the parameter object
    @return A reference to the CPUManager object
*/
CPUManager& CPUManager::operator=(CPUManager&& rhs)
{
    readyQueue_ = std::move(rhs.readyQueue_);
    currentProcess_ = std::move(rhs.currentProcess_);
    cpu_ = rhs.cpu_;

    return *this;
}       

/*
    @param  A reference to a process object
    @post   If there is no currently running process, the process begins running instantly with state 'Running'
            Otherwise, the process is added to the ready queue with state 'Ready'
*/
void CPUManager::addProcess(Process &process)
{
    if (currentProcess_.getProcessState() != NO_PROCESS && currentProcess_.getProcessID() != 0)
    {
        process.setProcessState(READY);
        readyQueue_.push_back(process);
    }
    else
    {
        process.setProcessState(RUNNING);
        currentProcess_ = process;
    }
}

/*
    @post   If the ready queue is not empty, the CPU begins running the first process in the ready queue
            Note: This function immediately overwrites the data of the currently running process, so to avoid losing it, save it before calling this function
*/
void CPUManager::runFirstProcess()
{
    if (!readyQueue_.empty())
    {
        currentProcess_ = readyQueue_.front();
        readyQueue_.pop_front();
        currentProcess_.setProcessState(RUNNING);
    }
}

/*
    @post   The currently running process is immediately killed and all data is cleared.
            The currently running process member of the class will be set to a dummy process that has PID 0 and state 'NO_PROCESS'
*/
void CPUManager::killRunningProcess()
{
    currentProcess_.setProcessID(0);
    currentProcess_.setProcessState(NO_PROCESS);
}

/*
    @post   The currently running process is moved to the back of the ready queue with state 'Ready'
            The first process in the ready queue begins executing with state 'Running'
            If the ready queue is empty, the process that was just paused by the timer interrupt resumes running as there are no processes waiting to be executed
*/
void CPUManager::timerInterrupt()
{
    Process currentProcess = currentProcess_;
    killRunningProcess();
    runFirstProcess();
    currentProcess.setProcessState(READY);
    addProcess(currentProcess);
}

/*
    @return A flag that indicates whether or not the CPU is currently running a process
*/
bool CPUManager::isBusy() const
{
    return currentProcess_.getProcessState() != NO_PROCESS;
}

/*
    @return The process that is currently being executed 
            Throws an exception if there is no process that is currently running
*/
Process CPUManager::getCurrentProcess() const
{
    if(currentProcess_.getProcessState() != NO_PROCESS)
        return currentProcess_;
    else
        throw std::logic_error("There is no currently running process.");
}

/*
    @return The ID of the process that is currently being executed 
            Throws an exception if there is no process that is currently running
*/
int CPUManager::getCurrentProcessID() const
{
    if(currentProcess_.getProcessState() != NO_PROCESS)
        return currentProcess_.getProcessID();
    else
        throw std::logic_error("There is no currently running process.");
}



/*
    @return The ready queue
*/
std::deque<Process> CPUManager::getReadyQueue() const
{
    return readyQueue_;
}