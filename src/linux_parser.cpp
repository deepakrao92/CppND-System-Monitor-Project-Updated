#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string key, value, line;
  float memTotal, memFree;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
  if (stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream (line);
      linestream >> key >> value;
      if (key == "MemTotal:") memTotal = std::stof(value);
      else if (key == "MemFree:") memFree = std::stof(value);
      else if (key == "MemAvailable:") break;
    } 
  }
  return ((memTotal - memFree)/memTotal);
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
    string upTime;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream >> upTime;
    }
  return std::stol(upTime);
}

/*
// Read and return the number of jiffies for the system (Not Implemented)
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID (Not Implemented)
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// Read and return the number of active jiffies for the system (Not Implemented)
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system (Not Implemented)
long LinuxParser::IdleJiffies() { return 0; }
*/

// Read and return CPU utilization of a process
float LinuxParser::CpuUtilization(int pid) {
  string line, var;
  vector<string> proc_stat;
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + 
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
      for (int i = 0; i < 22; i++){
      linestream >> var;
      proc_stat.emplace_back(var);
      }
  }

  float utime = LinuxParser::UpTime(pid)*1.0f;
  float stime = std::stof(proc_stat[14]);
  float cutime = std::stof(proc_stat[15]);
  float cstime = std::stof(proc_stat[16]);
  float starttime = std::stof(proc_stat[21]);
  float uptime = LinuxParser::UpTime()*1.0f;
  float hertz = sysconf(_SC_CLK_TCK);

  float total_time = utime + stime + cutime + cstime;

  float seconds = uptime - (starttime / hertz);
  float cpu_usage = 100.0f * ((total_time / hertz) / seconds);

  return cpu_usage;
}

// Read and return total CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_stat;
  string line, var;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> var;
    for (int i = 0; i < 10; i++){
      linestream >> var;
      cpu_stat.push_back(var);
    }
  }
  return cpu_stat;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, line;
  int totalProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (true){
      std::getline(stream, line);
      std::istringstream linestream (line);
      linestream >> key;
      if (key == "processes"){
        linestream >> totalProcesses;
        break;
      }
    }
  }
  return totalProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, line;
  int runningProcesses;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (true){
      std::getline(stream, line);
      std::istringstream linestream (line);
      linestream >> key;
      if (key == "procs_running"){
        linestream >> runningProcesses;
        break;
      }
    }
  }
  return runningProcesses;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
    string line, command;
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
        linestream >> command;
    }  
  return command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if ( key == "VmSize:")
	      return Format::kB2MB(std::stol(value));
    }
  }  
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if ( key == "Uid:")
        return value;
    }
  }  
  return string(); 
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, key, value, x ;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == LinuxParser::Uid(pid)) {
          std::replace(value.begin(), value.end(), ' ', ':');
          return key;
        }
      }
    }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long seconds;
  string line, var;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream.str(line);
    // Loop to pop off first 21 variables
    for (int i = 0; i < 22; i++){
      linestream >> var;
    }
    seconds = std::stol(var)/sysconf(_SC_CLK_TCK);
    return LinuxParser::UpTime() - seconds; // To update the uptime in process display 
  }
    
  return 0; // shouldn't happen
}
