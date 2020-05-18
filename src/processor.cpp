#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    std::vector<std::string> cpuValues = LinuxParser::CpuUtilization();

    float user    = std::stof(cpuValues[LinuxParser::kUser_]);
    float nice    = std::stof(cpuValues[LinuxParser::kNice_]);
    float system  = std::stof(cpuValues[LinuxParser::kSystem_]);
    float idle    = std::stof(cpuValues[LinuxParser::kIdle_]);
    float iowait  = std::stof(cpuValues[LinuxParser::kIOwait_]);
    float irq     = std::stof(cpuValues[LinuxParser::kIRQ_]);
    float softirq = std::stof(cpuValues[LinuxParser::kSoftIRQ_]);
    float steal   = std::stof(cpuValues[LinuxParser::kSteal_]);

    float PrevIdle = previdle + previowait;
    float Idle = idle + iowait;

    float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    float NonIdle = user + nice + system + irq + softirq + steal;

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;

    // differentiate: actual value minus the previous one
    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    float CPU_Percentage = ((totald - idled)/totald);

    AssignPrevValues(cpuValues);

    return CPU_Percentage;
 }

void Processor::AssignPrevValues(std::vector<std::string> prevCpuValues){
    prevuser    = std::stoi(prevCpuValues[LinuxParser::kUser_]);
    prevnice    = std::stoi(prevCpuValues[LinuxParser::kNice_]);
    prevsystem  = std::stoi(prevCpuValues[LinuxParser::kSystem_]);
    previdle    = std::stoi(prevCpuValues[LinuxParser::kIdle_]);
    previowait  = std::stoi(prevCpuValues[LinuxParser::kIOwait_]);
    previrq     = std::stoi(prevCpuValues[LinuxParser::kIRQ_]);
    prevsoftirq = std::stoi(prevCpuValues[LinuxParser::kSoftIRQ_]);
    prevsteal   = std::stoi(prevCpuValues[LinuxParser::kSteal_]);
}