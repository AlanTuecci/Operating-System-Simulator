// Alan Tuecci
#include "CPUManager.h"

//--------------------------------------------Constructors--------------------------------------------

/*
    @post   Ready queue is initialized with 0 elements.
            Current Process is set to some dummy process ID with PID 0.
*/
CPUManager::CPUManager(): 
    readyQueue_{},
    currentProcess_{0}
{
}

/*
    @param  A const Lvalue reference to a CPUManager object.
    @post   CPUManager object constructed to be an exact copy of the parameter object.
*/
CPUManager::CPUManager(const CPUManager& rhs):
    readyQueue_{std::move(rhs.readyQueue_)}, 
    currentProcess_{rhs.currentProcess_}
{
}   

/*
    @param  A const Lvalue reference to a CPUManager object.
    @post   CPUManager object is set to be an exact copy of the parameter object.
    @return A reference to the CPUManager object.
*/
CPUManager& CPUManager::operator=(const CPUManager& rhs)
{
    readyQueue_ = std::move(rhs.readyQueue_);
    currentProcess_ = rhs.currentProcess_;

    return *this;
}  

/*
    @param  An Rvalue reference to a CPUManager object.
    @post   CPUManager object constructed to be an exact copy of the parameter object.
*/
CPUManager::CPUManager(CPUManager&& rhs):
    readyQueue_{std::move(rhs.readyQueue_)}, 
    currentProcess_{rhs.currentProcess_}
{
}          

/*
    @param  An Rvalue reference to a CPUManager object.
    @post   CPUManager object is set to be an exact copy of the parameter object.
    @return A reference to the CPUManager object.
*/
CPUManager& CPUManager::operator=(CPUManager&& rhs)
{
    readyQueue_ = std::move(rhs.readyQueue_);
    currentProcess_ = rhs.currentProcess_;

    return *this;
}       

//--------------------------------------------Setters--------------------------------------------

/*
    @post   Replaces the existing readyQueue with the one in the parameter.
*/
void CPUManager::setReadyQueue(const std::deque<int>& readyQueue)
{
    readyQueue_ = readyQueue;
}

//--------------------------------------------Getters--------------------------------------------

/*
    @return The ID of the process that is currently being executed.
            Throws an exception if there is no process that is currently running.
*/
int CPUManager::getCurrentProcessID() const
{
    if(currentProcess_ != NO_PROCESS)
        return currentProcess_;
    else
        throw std::logic_error("There is no currently running process.");
}

/*
    @return The ready queue.
*/
std::deque<int> CPUManager::getReadyQueue() const
{
    return readyQueue_;
}

//--------------------------------------------Utilities--------------------------------------------

/*
    @param  A reference to a process ID.
    @post   If there is no currently running process, the process begins running instantly with state 'Running'.
            Otherwise, the process is added to the ready queue with state 'Ready'.
    @return READY if the process was sent to the readyqueue, RUNNING if the process immediately began using the CPU
*/
int CPUManager::addProcess(const int &process)
{
    if (currentProcess_ != 0)
    {
        readyQueue_.push_back(process);
        return READY;
    }
    else
    {
        currentProcess_ = process;
        return RUNNING;
    }
}

/*
    @post   If the ready queue is not empty, the CPU begins running the first process in the ready queue.
            Note: This function immediately overwrites the data of the currently running process, so to avoid losing it, save it before calling this function.
    @return An std::pair object with the first element being the PID of the new current process, and the second element being the current process's new state.
            std::pair<0, 0> if there are no other processes in the ready queue.
*/
std::pair<int, int> CPUManager::runFirstProcess()
{
    if (!readyQueue_.empty())
    {
        currentProcess_ = readyQueue_.front();
        readyQueue_.pop_front();
        return std::make_pair(currentProcess_, RUNNING);    //Return the currentProcess PID and its current state
    }
    else
    {
        killRunningProcess();
        return std::make_pair(0, 0);    //Return some placeholder showing no process
    }
}

/*
    @post   The currently running process is immediately killed and all data is cleared.
            The currently running process member of the class will be set to a dummy process that has PID 0 and state 'NO_PROCESS'.
*/
void CPUManager::killRunningProcess()
{
    currentProcess_ = 0;
}

/*
    @post   The currently running process is moved to the back of the ready queue with state 'Ready'.
            The first process in the ready queue begins executing with state 'Running'.
            If the ready queue is empty, the process that was just paused by the timer interrupt resumes running as there are no processes waiting to be executed.
    @return An std::pair object with the first element being the PID of the current Process and the second element being the state of the current process.
*/
std::pair<int, int> CPUManager::timerInterrupt()
{
    addProcess(currentProcess_);
    killRunningProcess();
    return runFirstProcess();
}

/*
    @return A flag that indicates whether or not the CPU is currently running a process.
*/
bool CPUManager::isBusy() const
{
    return currentProcess_ != NO_PROCESS;
}