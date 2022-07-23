#include "process.h"

using std::to_string;

// Constructor
Process::Process(int pid){
    pid_ = pid;
    user_ = LinuxParser::User(pid);
    stime_ = LinuxParser::STime(pid); // Pid start time
    utime_ = LinuxParser::UpTime() - stime_;
    cmd_ = LinuxParser::Command(pid);
    ram_ = LinuxParser::Ram(pid);
    cpu_util_ = 0.0;
}

// Getters
int Process::Pid() { return pid_; }
std::string Process::User() { return user_; }
std::string Process::Command() { return cmd_; }
float Process::CpuUtilization() { return cpu_util_; }
std::string Process::Ram() { 
    updateRamUtil();
    return ram_; 
}
long int Process::UpTime() { 
    updateUptime();
    return utime_; 
}

// Setters
void Process::updateUptime(){ utime_ = LinuxParser::UpTime() - stime_; }
void Process::updateRamUtil(){ ram_ = LinuxParser::Ram(pid_); }

// Reference: https://stackoverflow.com/questions/1420426/how-to-calculate-the-cpu-usage-of-a-process-by-pid-in-linux-from-c/1424556#1424556
void Process::updateCpuUtil(long totalDiff_j){
    // Store last cpu active time, then update
    prev_cputime = cputime_;
    cputime_ = LinuxParser::ActiveCpuTime(pid_);
    // Calculate change in active time
    float cputimeDiff = std::fabs(cputime_ - prev_cputime);
    // Convert total CPU jiffies to seconds
    float totalDiffsec = (float) totalDiff_j/(float)LinuxParser::clk_tcks;
    if (totalDiffsec != 0.0){
        cpu_util_ = cputimeDiff/totalDiffsec;
    }
}

// Overloads
bool Process::operator<(Process const& a) const { return (cpu_util_ < a.cpu_util_); }
bool Process::operator>(Process const& a) const { return (cpu_util_ > a.cpu_util_); }
bool Process::operator==(Process const& a) const {return pid_ == a.pid_; } // to compare Process objects