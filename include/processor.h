#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#define num_cpu_times 10

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  //Processor() : actual_cpu_j(10,0), prev_cpu_j(10,0) {};

 private:
  long activeJiffies = 0;
  long idleJiffies = 0;
  long prevActiveJiffies = 0;
  long prevIdleJiffies = 0;
  long totalCur = 0;
  long totalPrev = 0;
  long totalDiff = 0;
  long idleDiff = 0;
  std::vector<long> cpu_j = std::vector<long>(10,0);
    std::vector<long> prev_cpu_j = std::vector<long>(10,0);

  // Methods
  void calcActiveJiffies();
  void calcIdleJiffies();
};

#endif