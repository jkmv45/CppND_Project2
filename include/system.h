#ifndef SYSTEM_H
#define SYSTEM_H

#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System {
 public:
  Processor& Cpu();                 
  std::vector<Process>& Processes();
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem(); 

  // Constructor/destructor
  System();
  ~System(){};

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string kernal_;
  std::string os_;
};

#endif