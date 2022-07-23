#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <cctype>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;
  bool operator==(Process const& a) const;

  // Setters
  void updateUptime();
  void updateCpuUtil(long totalDiff_j = 0);
  void updateRamUtil();
  
  // Contructor/destructor
  Process(int);
  ~Process(){};


 private:
    int pid_{0};
    long int utime_{0};
    long int stime_{0};
    float cputime_{0};
    float prev_cputime{0};
    float cpu_util_{0.0};
    std::string user_;
    std::string cmd_;
    std::string ram_;
};

#endif