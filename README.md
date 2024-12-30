# OS Simulator
## A Simple Operating System Simulator

### This project is designed to simulate the internal workings of an operating system including:
- Process Management
  - You can create, fork, exit, or call wait on processses. Note: Process termination is done with cascading termination.
- Process Scheduling
  - Round-robin scheduling guarantees fair CPU resource allocation amongst processes. Processes are given a time limit for CPU usage, and if their execution time exceeds this allotted time, it is moved to the back of the ready-queue, allowing the next process to run.
- Memory Management
  - The use of paging with Least Recently Used Page Replacement (LRUPR) allows for efficient memory resource utilization. LRUPR ensures that should available memory be exhausted, the least recently used frame is removed from memory which frees up resources.
- Disk I/O Management
  - File read/disk job requests are completed via a first-come, first-served model.
 
### Try it yourself
- main.cpp is a simple program that allows you to try out some of the OS simulator functions.
- Here, you can input your own number of hard disks, processes, RAM size, and page size to better understand what the operating system is doing.
- To compile and run this program, paste the following command into your terminal at the repository directory root: `g++ main.cpp -o main && main`

### Tests
#### This repository has 2 test files: "incomplete_test.cpp" and "check.cpp"
- incomplete_test.cpp
  - This is a preliminary test suite that covers only the most basic of test cases.
  - Successful execution of these tests does not guarantee that the OS simulator functions as expected.
  - To compile and run these tests, paste the following command into your terminal at the repository directory root: `g++ incomplete_test.cpp -o incomplete_test && incomplete_test`
- check.cpp
  - This is a superset of the tests in incomplete_test.cpp that covers a significantly wider range of tests.
  - Successful execution of these tests likely means that the OS simulator works as expected.
  - To compile and run these tests, paste the following command into your terminal at the repository directory root: `g++ check.cpp -o check && check`

### System Requirements
To run any of the tests or the main program, your system must have G++ with >= C++11 support installed. 
