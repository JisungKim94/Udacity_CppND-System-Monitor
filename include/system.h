#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

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

  void addProcesses(Process process);
  std::vector<int> top_CpuUtilizations_Pid();
  typedef std::pair<float, int> float_int_pair;

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif