#include "linux_parser.h"
#include <iostream>

int main() {
    std::string osname = LinuxParser::OperatingSystem();
    std::string kernalname = LinuxParser::Kernel();
    std::vector<int> pidlist = LinuxParser::Pids();
    float memutil = LinuxParser::MemoryUtilization();
    long uptime = LinuxParser::UpTime();
    int totalproc = LinuxParser::TotalProcesses();
    int runproc = LinuxParser::RunningProcesses();

    // Find Index for VS Code based on PID (make more robust later, if needed)
    int idx = 0;
    auto it = find(pidlist.begin(),pidlist.end(),12049);
    if (it != pidlist.end()){
        idx = it - pidlist.begin();
    }

    std::string uid = LinuxParser::Uid(pidlist.at(idx));
    std::string user = LinuxParser::User(pidlist.at(idx));
    std::string ram = LinuxParser::Ram(pidlist.at(idx));
    std::string cmd = LinuxParser::Command(pidlist.at(idx));
    long utime_pid = LinuxParser::UpTime(pidlist.at(idx));
    long atime_pid = LinuxParser::ActiveJiffies(pidlist.at(idx));

    std::cout << "***** LINUX PARSER TEST *****" << std::endl;
    std::cout << "OS Name: " << osname << std::endl;
    std::cout << "Kernal: " << kernalname << std::endl;
    std::cout << "Memory Utilization: " << memutil << std::endl;
    std::cout << "System Uptime: " << uptime << std::endl;
    std::cout << "Total Processes: " << totalproc << std::endl;
    std::cout << "Running Processes: " << runproc << std::endl;
    std::cout << "Uid for First pid: " << uid << std::endl;
    std::cout << "User for First pid: " << user << std::endl;
    std::cout << "Ram for First pid: " << ram << std::endl;
    std::cout << "Command for First pid: " << cmd << std::endl;
    std::cout << "Uptime for First pid: " << utime_pid << std::endl;
    std::cout << "Active Jiffies for First pid: " << atime_pid << std::endl;

    // std::cout << "*** PID List ***" << std::endl;
    // for (int mypid : pidlist){
    //     std::cout << "PID: " << mypid << std::endl;
    // }

}