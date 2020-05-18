#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>

class Processor {
 public:
  float Utilization();  
  void AssignPrevValues(std::vector<std::string> cpuValues);

  // Declare any necessary private members
 private:
  int prevuser{0};
  int prevnice{0};
  int prevsystem{0};
  int previdle{0};
  int previowait{0};
  int previrq{0};
  int prevsoftirq{0};
  int prevsteal{0};
};

#endif