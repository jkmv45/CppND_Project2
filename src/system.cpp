#include "system.h"

using std::set;
using std::size_t;

System::System(){
    kernal_ = LinuxParser::Kernel();
    os_ = LinuxParser::OperatingSystem();
    cpu_ = Processor();
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
std::vector<Process>& System::Processes() { 
    std::vector<int> pidlist = LinuxParser::Pids(); // get the current active pids
    std::vector<Process> old_list = processes_; // store local copy of current list to track new and dead processes
    // Add New Processes
    for (int mypid : pidlist){
        auto it = std::find_if(old_list.begin(), old_list.end(),[&mypid](Process& obj) { return obj.Pid() == mypid; });
        if (it == old_list.end()){
            processes_.push_back(Process(mypid)); // Pid not found, so add to list
        }
        else {
            old_list.erase(it); // Pid was found, so remove it from list copy
        }
    }
    // Remove Dead Processes
    for (int idx; idx < old_list.size(); idx++){
        Process deadproc = old_list.at(idx);
        processes_.erase(std::find_if(processes_.begin(),processes_.end(), [&deadproc](Process& obj) {return obj == deadproc; }));
    }

    // Sort Processes
    std::sort(processes_.begin(),processes_.end());
    std::reverse(processes_.begin(),processes_.end());
    return processes_; }

std::string System::Kernel() { return kernal_; }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return os_; }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }