#include "processor.h"

// Reference for math: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    std::vector<std::string> cpu_times = LinuxParser::CpuUtilization();
    // First check to make sure we got the right number of elements out of the file.  If not, don't update anything.
    if (cpu_times.size() == num_cpu_times){
        for (int idx = 0; idx < num_cpu_times; idx++){
            cpu_j.at(idx) = (long)stoi(cpu_times.at(idx)); // update current state
        }
    }
    // Compute Active and Idle Jiffies current and previous
    prevActiveJiffies = activeJiffies;
    prevIdleJiffies = idleJiffies;
    calcActiveJiffies();
    calcIdleJiffies();
    // Calculate Total Jiffies current and previous
    totalCur = activeJiffies + idleJiffies;
    totalPrev = prevActiveJiffies + prevIdleJiffies;
    // Caluclate Differential between current and previous values
    totalDiff = totalCur - totalPrev;
    idleDiff = idleJiffies - prevIdleJiffies;    // Calculate CPU Utilzation
    if (totalDiff != 0){
        cpu_util = (totalDiff - idleDiff)/(float)totalDiff;
    }
    return cpu_util;
}

void Processor::calcActiveJiffies(){
    activeJiffies = cpu_j.at(LinuxParser::CPUStates::kUser_) + cpu_j.at(LinuxParser::CPUStates::kNice_) + 
                    cpu_j.at(LinuxParser::CPUStates::kSystem_) + cpu_j.at(LinuxParser::CPUStates::kIRQ_) +
                    cpu_j.at(LinuxParser::CPUStates::kSoftIRQ_) + cpu_j.at(LinuxParser::CPUStates::kSteal_);
}

void Processor::calcIdleJiffies(){
    idleJiffies =  cpu_j.at(LinuxParser::CPUStates::kIdle_) + cpu_j.at(LinuxParser::CPUStates::kIOwait_);
}