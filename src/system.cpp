#include "system.h"

#include <unistd.h>

#include <cstddef>
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

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

void System::addProcesses(Process process) { processes_.push_back(process); }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  std::vector<int> pids_ = LinuxParser::Pids();
  // int pids_size = pids_.size();
  // int index;
  Process process;
  Process process2;

  // for (index = 0; index > pids_size; index++) {
  // process.setPid(pids_[index]);
  // display 에있는 n 이 10으로 초기화 되어있음.. 즉 pid 는 총 10개가
  // 나온다는거고 여따가 cpu 부하 높은 10개만 따로 넣으면 되는건가?.,....??
  // 동영상 강의 다시 한번 보자
  process.setPid(5);
  process2.setPid(10);
  System::addProcesses(process);
  System::addProcesses(process2);
  // }

  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }