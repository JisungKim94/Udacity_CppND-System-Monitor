#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

#define Hertz sysconf(_SC_CLK_TCK)

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string
      line;  // A variable string line is the line itself, not the line number.
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(
        filestream,
        line)) {  // if line ended std::getline(filestream, line) return 0
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=',
                   ' ');  //  linestream output(>>) is each spacing! Thus key &
                          //  value is devided by =
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >>
             value) {  //  linestream output(>>) is each spacing! Thus key &
                       //  value is devided by =
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);  // /proc/version have just 1 line thus, dont
                                 // have to while expression
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO & BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  // c++17 이상에서 사용할 수 있는 filesystem lib -> 안쓸랭
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        if (filename != "") {
          int pid = stoi(filename);
          pids.emplace_back(pid);
        }
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key;
  string line;
  string KB;
  float value = 0.0;  // value type is float!
  float available_value;
  float total_value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(
        filestream,
        line)) {  // if line ended std::getline(filestream, line) return 0
      std::istringstream linestream(line);
      while (linestream >> key >>
             value) {  //  linestream output(>>) is each spacing! Thus key &
                       //  value is devided by =
        if (key == "MemTotal:") {
          total_value = value;
        } else if (key == "MemAvailable:") {
          available_value = value;
          return (1.0 - (available_value / total_value));
        }
      }
    }
  }
  return 0.0;
}

// DONE: Read and return the system uptime
long int LinuxParser::UpTime() {
  long int value;
  long int tempvalue;
  string line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);  // /proc/version have just 1 line thus,
                                     // dont have to while expression
    std::istringstream linestream(line);
    linestream >> tempvalue;
    value = tempvalue;
  }

  return value;
}

long LinuxParser::NonIdle_forCalcCpuUtilization() {
  std::vector<std::string> aggregate_cpu_ = LinuxParser::CpuUtilization();
  return stol(aggregate_cpu_[LinuxParser::kUser_]) +
         stol(aggregate_cpu_[LinuxParser::kNice_]) +
         stol(aggregate_cpu_[LinuxParser::kSystem_]) +
         stol(aggregate_cpu_[LinuxParser::kIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSoftIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSteal_]);
}

long LinuxParser::Idle_forCalcCpuUtilization() {
  std::vector<std::string> aggregate_cpu_ = LinuxParser::CpuUtilization();
  return stol(aggregate_cpu_[LinuxParser::kIdle_]) +
         stol(aggregate_cpu_[LinuxParser::kIOwait_]);
}

vector<string> LinuxParser::CpuUtilization() {
  std::vector<std::string> aggregate_cpu_;
  string cpu_;
  string user_;
  string nice_;
  string system_;
  string idle_;
  string iowait_;
  string irq_;
  string softirq_;
  string steal_;
  string guest_;
  string guest_nice_;

  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu_ >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >>
        irq_ >> softirq_ >> steal_ >> guest_ >> guest_nice_;

    aggregate_cpu_.emplace_back(user_);
    aggregate_cpu_.emplace_back(nice_);
    aggregate_cpu_.emplace_back(system_);
    aggregate_cpu_.emplace_back(idle_);
    aggregate_cpu_.emplace_back(iowait_);
    aggregate_cpu_.emplace_back(irq_);
    aggregate_cpu_.emplace_back(softirq_);
    aggregate_cpu_.emplace_back(steal_);
    aggregate_cpu_.emplace_back(guest_);
    aggregate_cpu_.emplace_back(guest_nice_);
  }
  return aggregate_cpu_;
}

float LinuxParser::CpuUtilization(int pid) {
  string pass_;
  string utime_;
  string stime_;
  string cutime_;
  string cstime_;
  string starttime_;

  float total_time_ = 0.0;
  float seconds_ = 0.0;
  float uptime_ = (float)UpTime();
  float CpuUtilization = 0.0;

  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> pass_ >> pass_ >> pass_ >> pass_ >> pass_ >> pass_ >> pass_ >>
        pass_ >> pass_ >> pass_ >> pass_ >> pass_ >> pass_ >> utime_ >>
        stime_ >> cutime_ >> cstime_ >> pass_ >> pass_ >> pass_ >> pass_ >>
        starttime_;
  }
  if ((utime_ != "") && (stime_ != "") && (cutime_ != "") && (cstime_ != "") &&
      (starttime_ != "")) {
    total_time_ = stof(utime_) + stof(stime_) + stof(cutime_) + stof(cstime_);
    seconds_ = uptime_ - (stof(starttime_) / Hertz);
    CpuUtilization = 100 * ((total_time_ / Hertz) / seconds_);
  }

  return CpuUtilization;
}

int LinuxParser::TotalProcesses() {
  vector<int> pids_ = LinuxParser::Pids();

  return (int)pids_.size();
}

int LinuxParser::RunningProcesses() {
  std::vector<int> processes_ = LinuxParser::Pids();
  string state;
  string pass;
  string line;
  int RunningProcesses_ = 0;
  for (unsigned int i = 0; i < processes_.size(); i++) {
    std::ifstream filestream(kProcDirectory + to_string(processes_[i]) +
                             kStatFilename);
    if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> pass >> pass >> state;
    } else {
      /* Nothing to do */
    }

    if (state == "R") {
      RunningProcesses_++;
    } else {
      /* Nothing to do */
    }
  }

  return RunningProcesses_;
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  long int longint_value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      // using VmData not VmSize;
      if (key == "VmData:") {
        if (value != "") {
          longint_value = (long int)(stof(value) / 1024);
          value = to_string(longint_value);
          return value;
        }
      }
    }
  }
  return value;
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return value;
}

// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string Uid = LinuxParser::Uid(pid);
  string line;
  string x;
  string value;
  string usrname;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> usrname >> x >> value) {
        if (value == Uid) {
          return usrname;
        }
      }
    }
  }
  return usrname;
}

// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string x;
  string value;
  string pass;
  string usrname;
  int cnt = 0;
  float uptime = (float)UpTime();
  long uptimepid;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while (linestream >> pass) {
      if (cnt == 21) {
        value = pass;
        break;
      }
      cnt = cnt + 1;
    }
  }
  uptimepid = (uptime - stol(value) / Hertz);
  return uptimepid;
}
