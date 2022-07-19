#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Getters
int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() { return this->cpu_util_; }

string Process::Command() { return this->cmd_; }

string Process::Ram() { return this->ram_; }

string Process::User() { return this->user_; }

long int Process::UpTime() { return this->utime_; }

// Setters
void Process::setCpuUtil(float cpu_util){
    this->cpu_util_ = cpu_util;
}

void Process::setRamUtil(long int ram_util){
    this->ram_ = ram_util;
}

// Overloads
bool Process::operator<(Process const& a) const { return this->cpu_util_ < a.cpu_util_; }