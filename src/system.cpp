#include "system.h"

#include <unistd.h>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"  // DONE include header for LinuxParser namespace
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
#define N_SIZE 10

Processor& System::Cpu() { return cpu_; }

void System::addProcesses(Process process) { processes_.emplace_back(process); }

bool comparator(const System::float_int_pair& l,
                const System::float_int_pair& r) {
  // 첫번째 값을 기준으로 내림차순 비교하는 comparator
  return l.first > r.first;
}

std::vector<int> System::top_CpuUtilizations_Pid() {
  std::vector<int> pids_ = LinuxParser::Pids();

  int pid_;
  std::vector<System::float_int_pair> tempCpuUtilizations;
  std::vector<int> top_N_SIZE_CpuUtilizations_Pid_;

  for (unsigned int index = 0; index < pids_.size(); index++) {
    pid_ = pids_[index];
    tempCpuUtilizations.emplace_back(
        System::float_int_pair(LinuxParser::CpuUtilization(pid_), pid_));
  }
  stable_sort(tempCpuUtilizations.begin(), tempCpuUtilizations.end(),
              comparator);
  for (unsigned int index = 0; index < N_SIZE; index++) {
    top_N_SIZE_CpuUtilizations_Pid_.emplace_back(
        tempCpuUtilizations[index].second);
  }
  return top_N_SIZE_CpuUtilizations_Pid_;
}

vector<Process>& System::Processes() {
  std::vector<int> pids_ = System::top_CpuUtilizations_Pid();
  Process process0;
  Process process1;
  Process process2;
  Process process3;
  Process process4;
  Process process5;
  Process process6;
  Process process7;
  Process process8;
  Process process9;

  // sort top N_SIZE processor utilization
  process0.setPid(pids_[0]);
  process1.setPid(pids_[1]);
  process2.setPid(pids_[2]);
  process3.setPid(pids_[3]);
  process4.setPid(pids_[4]);
  process5.setPid(pids_[5]);
  process6.setPid(pids_[6]);
  process7.setPid(pids_[7]);
  process8.setPid(pids_[8]);
  process9.setPid(pids_[9]);

  System::addProcesses(process0);
  System::addProcesses(process1);
  System::addProcesses(process2);
  System::addProcesses(process3);
  System::addProcesses(process4);
  System::addProcesses(process5);
  System::addProcesses(process6);
  System::addProcesses(process7);
  System::addProcesses(process8);
  System::addProcesses(process9);

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
