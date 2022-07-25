#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#define num_cpu_times 10

class Processor {
 public:
  float Utilization();
  long totalDiff = 0;  // For sharing with each process to calc CPU Utilzation
  
 private:
  long activeJiffies = 0;
  long idleJiffies = 0;
  long prevActiveJiffies = 0;
  long prevIdleJiffies = 0;
  long totalCur = 0;
  long totalPrev = 0;
  long idleDiff = 0;
  float cpu_util = 0.0;
  std::vector<long> cpu_j = std::vector<long>(10,0);

  // Methods
  void calcActiveJiffies();
  void calcIdleJiffies();
  
};

#endif