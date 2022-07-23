#include "system.h"

using std::set;
using std::size_t;

// Constructor
System::System(){
    kernal_ = LinuxParser::Kernel();
    os_ = LinuxParser::OperatingSystem();
    cpu_ = Processor();
    std::vector<int> pidlist = LinuxParser::Pids(); // get the current active pids
    for (int mypid : pidlist){
        processes_.push_back(Process(mypid)); // First call, populate list
    }
}

// Getters
Processor& System::Cpu() { return cpu_; }
std::string System::Kernel() { return kernal_; }
std::string System::OperatingSystem() { return os_; }
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }
long int System::UpTime() { return LinuxParser::UpTime(); }

// Process Manager
std::vector<Process>& System::Processes() { 
    std::vector<int> pidlist = LinuxParser::Pids(); // get the current active pids
    std::vector<Process> old_list = processes_; // store local copy of current list to track new and dead processes
    // Add New Processes
    for (int mypid : pidlist){
        // First make sure we have a list of processes for comparison
        if (!old_list.empty()){
            // Search the old list for the current pid
            auto it = std::find_if(old_list.begin(), old_list.end(),[&mypid](Process& obj) { return obj.Pid() == mypid; });
            if (it == old_list.end() && it->Pid() != mypid){
                processes_.push_back(Process(mypid)); // Pid not found, so add to list
            }
            else {
                old_list.erase(it); // Pid was found, so remove it from old list
            }
        }
        else {
            processes_.push_back(Process(mypid)); // In case we have an empty list, fill it up here.
        }
    }
    // Remove Dead Processes
    for (uint idx = 0; idx < old_list.size(); idx++){
        // At this point, any elements in old_list must not be active, so lets remove them
        Process deadproc = old_list.at(idx);
        processes_.erase(std::find_if(processes_.begin(),processes_.end(), [&deadproc](Process& obj) {return obj == deadproc; }));
    }
    // Update the CPU utilization so sort works properly
    for (uint idy = 0; idy < processes_.size(); idy++){
        Process* myproc = &(processes_.at(idy));
        myproc->updateCpuUtil(cpu_.totalDiff);
    }
    // Sort Processes in Descending Order
    std::sort(processes_.begin(),processes_.end(),std::greater<Process>());
    return processes_; 
}