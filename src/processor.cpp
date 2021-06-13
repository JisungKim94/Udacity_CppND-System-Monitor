#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

float Processor::Utilization() {
  return (float)((float)(LinuxParser::IdleJiffies() -
                         LinuxParser::ActiveJiffies()) /
                 (float)LinuxParser::IdleJiffies());
}