#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int getPid();
  std::string User();
  std::string Command();
  double CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  void setPid(int pid);

 private:
  int pid_;
};

#endif