#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  // LinuxParser::CpuUtilization();
  return (float)((float)(LinuxParser::IdleJiffies() -
                         LinuxParser::ActiveJiffies()) /
                 (float)LinuxParser::IdleJiffies());
}