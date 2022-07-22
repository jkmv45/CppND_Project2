#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::to_string;
using std::vector;

// Constructor
Process::Process(int pid){
    pid_ = pid;
    user_ = LinuxParser::User(pid);
    utime_ = LinuxParser::UpTime() - (LinuxParser::UpTime(pid)/LinuxParser::clk_tcks); // System uptime - Pid start time
    cmd_ = LinuxParser::Command(pid);
    ram_ = LinuxParser::Ram(pid);
    cpu_util_ = LinuxParser::ActiveJiffies(pid)/(float)utime_;
}

// Getters
int Process::Pid() { return pid_; }
std::string Process::User() { return user_; }
std::string Process::Command() { return cmd_; }
float Process::CpuUtilization() { 
    updateCpuUtil();
    return cpu_util_; 
}
std::string Process::Ram() { 
    updateRamUtil();
    return ram_; 
}
long int Process::UpTime() { 
    updateUptime();
    return utime_; 
}

// Setters
void Process::updateUptime(){
    utime_ = LinuxParser::UpTime() - LinuxParser::UpTime(pid_)/LinuxParser::clk_tcks;
}

void Process::updateCpuUtil(){
    updateUptime(); // Ensure uptime is updated first
    cpu_util_ = (LinuxParser::ActiveJiffies(pid_)/(float)LinuxParser::clk_tcks)/(float)utime_;
}

void Process::updateRamUtil(){
    ram_ = LinuxParser::Ram(pid_);
}

// Overloads
bool Process::operator<(Process const& a) const { return cpu_util_ < a.cpu_util_; }
bool Process::operator==(Process const& a) const {return pid_ == a.pid_; } // to compare Process objects
//bool Process::operator==(int const& i) const { return pid_ == i; } // to compare Process to pid value