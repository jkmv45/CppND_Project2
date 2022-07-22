#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  bool operator==(Process const& a) const;
  //bool operator==(int const& i) const;

  // Setters
  void updateUptime();
  void updateCpuUtil();
  void updateRamUtil();
  
  // Contructor/destructor
  //Process(int pid, long int ut, float cpu_util, std::string name, std::string cmd, std::string ram_util) : 
  //  pid_(pid), utime_(ut), cpu_util_(cpu_util), user_(name), cmd_(cmd), ram_(ram_util){};
  Process(int);
  ~Process(){};


 private:
    int pid_{0};
    long int utime_{0};
    float cpu_util_{0.0};
    std::string user_;
    std::string cmd_;
    std::string ram_;
};

#endif