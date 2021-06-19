#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"  // DONE include header for LinuxParser namespace

using std::string;
using std::to_string;
using std::vector;

int Process::getPid() { return pid_; }
void Process::setPid(int pid) { pid_ = pid; }

double Process::CpuUtilization() { return LinuxParser::CpuUtilization(pid_); }

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() {
  int pid = Process::getPid();
  return LinuxParser::UpTime(pid);
}

// not used
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}