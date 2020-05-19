#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
 */
class Process {
 public:
  Process(int pid);
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  // Declare any necessary private members
 private:
    int pid_{0};
    std::string command_{};
    std::string user_{};
    float cpu_utilization_{};
    std::string ram_{};
    long int up_time_{};
};


#endif