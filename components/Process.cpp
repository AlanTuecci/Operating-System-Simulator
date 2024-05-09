#include "Process.h"

/*
    @post   Process ID set to 0
            Process State set to 'NEW'
            Process Type set to 'REGULAR'
            Child Processes vector initialized to an empty state
            Parent Process ID set to 0
*/
Process::Process():
    processID_{0},
    processState_{NEW},
    processType_{REGULAR},
    childProcesses_{},
    parentProcessID_{0}
{
}

/*
    @param  A const Lvalue reference to an integer representing the process ID
    @post   Process ID set to the value of the process ID parameter
            Process State set to 'NEW'
            Process Type set to 'REGULAR'
            Child Processes vector initialized to an empty state
            Parent Process ID set to 0
*/
Process::Process(const int& processID):
    processID_{processID},
    processState_{NEW},
    processType_{REGULAR},
    childProcesses_{},
    parentProcessID_{0}
{
}

/*
    @param  A const Lvalue reference to an integer representing the process ID
    @param  A const Lvalue reference to an integer representing the process State
            State can be 'NO_PROCESS', 'NEW', 'READY', 'RUNNING', 'WAITING', or 'TERMINATED'
    @post   Process ID set to the value of the process ID parameter
            Process State set to the value of the process State parameter
            Process Type set to 'REGULAR'
            Child Processes vector initialized to an empty state
            Parent Process ID set to 0
*/
Process::Process(const int& processID, const int& processState):
    processID_{processID},
    processState_{processState},
    processType_{REGULAR},
    childProcesses_{},
    parentProcessID_{0}
{
}

/*
    @param  A const Lvalue reference to an integer representing the process ID
    @param  A const Lvalue reference to an integer representing the process State
            State can be 'NO_PROCESS', 'NEW', 'READY', 'RUNNING', 'WAITING', or 'TERMINATED'
    @param  A const Lvalue reference to a Type object representing the process Type 
            Type can be 'REGULAR', 'PARENT', 'CHILD' or 'ZOMBIE'
    @post   Process ID set to the value of the process ID parameter
            Process State set to the value of the process State parameter
            Process Type set to the value of the process Type parameter
            Child Processes vector initialized to an empty state
            Parent Process ID set to 0
*/
Process::Process(const int& processID, const int& processState, const Type& processType):
    processID_{processID},
    processState_{processState},
    processType_{processType},
    childProcesses_{},
    parentProcessID_{0}
{
}

/*
    @param  A const Lvalue reference to an integer representing the process ID
    @param  A const Lvalue reference to an integer representing the process State
            State can be 'NO_PROCESS', 'NEW', 'READY', 'RUNNING', 'WAITING', or 'TERMINATED'
    @param  A const Lvalue reference to an Type object representing the process Type 
            Type can be 'REGULAR', 'PARENT', 'CHILD' or 'ZOMBIE'
    @param  A const Lvalue reference to an integer representing the parent process ID
    @post   Process ID set to the value of the process ID parameter
            Process State set to the value of the process State parameter
            Process Type set to the value of the process Type parameter
            Child Processes vector initialized to an empty state
            Parent Process ID set to 0
*/
Process::Process(const int& processID, const int& processState, const Type& processType, const int& parentProcessID):
    processID_{processID},
    processState_{processState},
    processType_{processType},
    childProcesses_{},
    parentProcessID_{parentProcessID}
{
}

/*
    @param  A const Lvalue reference to a Process object
    @post   Process object is initialized to be an exact copy of the parameter object
*/
Process::Process(const Process& rhs):
    processID_{rhs.processID_},
    processState_{rhs.processState_},
    processType_{rhs.processType_},
    childProcesses_{std::move(rhs.childProcesses_)},
    parentProcessID_{rhs.parentProcessID_}
{
}

/*
    @param  A const Lvalue reference to a Process object
    @post   Process object is set to be an exact copy of the parameter object
    @return A reference to the Process object
*/
Process& Process::operator=(const Process& rhs)
{
    processID_ = rhs.processID_;
    processState_ = rhs.processState_;
    processType_ = rhs.processType_;
    childProcesses_ = std::move(rhs.childProcesses_);
    parentProcessID_ = rhs.parentProcessID_;

    return *this;
}

/*
    @param  An Rvalue reference to a Process object
    @post   Process object is initialized to be an exact copy of the parameter object
*/
Process::Process(Process&& rhs):
    processID_{rhs.processID_},
    processState_{rhs.processState_},
    processType_{rhs.processType_},
    childProcesses_{std::move(rhs.childProcesses_)},
    parentProcessID_{rhs.parentProcessID_}
{
}

/*
    @param  An Rvalue reference to a Process object
    @post   Process object is set to be an exact copy of the parameter object
    @return A reference to the Process object
*/
Process& Process::operator=(Process&& rhs)
{
    processID_ = rhs.processID_;
    processState_ = rhs.processState_;
    processType_ = rhs.processType_;
    childProcesses_ = std::move(childProcesses_);
    parentProcessID_ = rhs.parentProcessID_;

    return *this;
}

/*
    @param  A const Lvalue reference to the process ID
    @post   The process ID member is set to the value of the parameter
*/
void Process::setProcessID(const int& processID)
{
    processID_ = processID;
}

/*
    @param  A const Lvalue reference to the process State
    @post   The process state member is set to the value of the parameter
*/
void Process::setProcessState(const int& processState)
{
    processState_ = processState;
}

/*
    @param  A const Lvalue reference to the process Type
    @post   The process Type member is set to the value of the parameter
*/
void Process::setProcessType(const Type& processType)
{
    processType_ = processType;
}

/*
    @param  A const Lvalue refernce to the process ID of the child process
    @post   The child process's ID is added to the child process record
*/
void Process::addChildProcessID(const int& childProcessID)
{
    childProcesses_.push_back(childProcessID);
}

/*
    @param  A const Lvalue refernce to the process ID of the child process
    @post   The child process's ID is removed from the child process record
*/
void Process::removeChildProcessFromRecord(const int& childProcessID)
{
    for(std::vector<int>::iterator i = childProcesses_.begin(); i != childProcesses_.end(); i++)
    {
        if(*i = childProcessID)
        {
            childProcesses_.erase(i);
            break;
        }
    }
}

/*
    @return The process ID
*/
int Process::getProcessID() const
{
    return processID_;
}

/*
    @return The process State
*/
int Process::getProcessState() const
{
    return processState_;
}

/*
    @return The process Type
*/
Type Process::getProcessType() const
{
    return processType_;
}

/*
    @return The child processes vector
*/
std::vector<int> Process::getChildProcesses() const
{
    return childProcesses_;
}

/*
    @return The process's parent's process ID
*/
int Process::getParentProcessID() const
{
    return parentProcessID_;
}

/*
    @param  The process to be compared to
    @return True if the Processes have the same ID, false otherwise
*/
bool Process::operator==(const Process& rhs) const
{
    return processID_ == rhs.processID_;
}