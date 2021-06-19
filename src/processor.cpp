#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

double Processor::Utilization() {
  long preIdle = 0;
  long preNonIdle = 0;
  long preTotal = 0;
  long Idle = 0;
  long NonIdle = 0;
  long Total = 0;
  double cpu_percentage = 0.0;

  Idle = LinuxParser::Idle_forCalcCpuUtilization();
  NonIdle = LinuxParser::NonIdle_forCalcCpuUtilization();
  Total = Idle + NonIdle;

  cpu_percentage = (double)((double)((Total - preTotal) - (Idle - preIdle)) /
                            ((double)Total - preTotal));

  preIdle = Idle;
  preNonIdle = NonIdle;
  preTotal = preIdle + preNonIdle;
  return cpu_percentage;
}