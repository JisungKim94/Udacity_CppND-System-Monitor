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
        int pid = stoi(filename);
        pids.push_back(pid);
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
          return available_value / total_value;
        }
      }
    }
  }
  return value;
}

// DONE: Read and return the system uptime
long int LinuxParser::UpTime() {
  long int value;
  string line;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);  // /proc/version have just 1 line thus,
                                     // dont have to while expression
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }

  return value;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::vector<std::string> aggregate_cpu_ = LinuxParser::CpuUtilization();
  return stol(aggregate_cpu_[LinuxParser::kIdle_]) +
         stol(aggregate_cpu_[LinuxParser::kIOwait_]) +
         stol(aggregate_cpu_[LinuxParser::kUser_]) +
         stol(aggregate_cpu_[LinuxParser::kNice_]) +
         stol(aggregate_cpu_[LinuxParser::kSystem_]) +
         stol(aggregate_cpu_[LinuxParser::kIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSoftIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSteal_]) +
         stol(aggregate_cpu_[LinuxParser::kGuest_]) +
         stol(aggregate_cpu_[LinuxParser::kGuestNice_]);
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::vector<std::string> aggregate_cpu_ = LinuxParser::CpuUtilization();
  return stol(aggregate_cpu_[LinuxParser::kUser_]) +
         stol(aggregate_cpu_[LinuxParser::kNice_]) +
         stol(aggregate_cpu_[LinuxParser::kSystem_]) +
         stol(aggregate_cpu_[LinuxParser::kIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSoftIRQ_]) +
         stol(aggregate_cpu_[LinuxParser::kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::vector<std::string> aggregate_cpu_ = LinuxParser::CpuUtilization();
  return stol(aggregate_cpu_[LinuxParser::kIdle_]) +
         stol(aggregate_cpu_[LinuxParser::kIOwait_]);
}

// TODO: Read and return CPU utilization
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

    aggregate_cpu_.push_back(user_);
    aggregate_cpu_.push_back(nice_);
    aggregate_cpu_.push_back(system_);
    aggregate_cpu_.push_back(idle_);
    aggregate_cpu_.push_back(iowait_);
    aggregate_cpu_.push_back(irq_);
    aggregate_cpu_.push_back(softirq_);
    aggregate_cpu_.push_back(steal_);
    aggregate_cpu_.push_back(guest_);
    aggregate_cpu_.push_back(guest_nice_);
  }
  return aggregate_cpu_;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  vector<int> pids_ = LinuxParser::Pids();

  return (int)pids_.size();
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
