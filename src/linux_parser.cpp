#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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
    filestream.close();
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
    stream.close();
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

float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  int memTotal = 0;
  int memFree = 0;
  int memAvail = 0;
  int buff = 0;
  float mem_util = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      line.erase(std::remove_if(line.begin(),line.end(), ::isspace),line.end()); // Remove whitespace
      //line.erase(std::remove_if(line.begin(),line.end(),[&units](const char &c) {return units.find(c) != std::string::npos;}),line.end());
      removeChars(line,unit_str); // Remove unit characters
      std::replace(line.begin(), line.end(), ':', ' '); // Format for istringstream
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        // Check for key/value pairs we care about
        if (key == "MemTotal") {
          memTotal = stoi(value);
          break;
        }
        else if (key == "MemFree") {
          memFree = stoi(value);
          break;
        }
        else if (key == "MemAvailable") {
          memAvail = stoi(value);
          break;
        }
        else if (key == "Buffers") {
          buff = stoi(value);
          break;
        }
      }
      // Check if we have all the necessary inputs to get utilization
      if (memTotal > 0 && memFree > 0) {
        mem_util = ((memTotal-memFree)/(float)memTotal)*100.0;
        break;
      }
    }
    filestream.close();
  }
  return mem_util;
}

long LinuxParser::UpTime() { 
  string line;
  string uptime_str;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    uptime_str = line.substr(0,line.find(" ")); // Get only the system uptime, which is prior to the first space
    filestream.close();
  }
  return (long)stoi(uptime_str); 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line, value;
  std::vector<std::string> line_vec;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      line_vec.push_back(value);
    }
    filestream.close();
  }
  return 0; 
  }

long LinuxParser::ActiveJiffies(int pid) { 
  std::vector<std::string> line_vec = parsePidStat(pid);
  long utime = (long)stoi(line_vec.at(13));
  long stime = (long)stoi(line_vec.at(14));
  long cutime = (long)stoi(line_vec.at(15));
  long cstime = (long)stoi(line_vec.at(16));
  return utime + stime + cutime + cstime; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
    filestream.close();
  }
  return 0; 
}

int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
    filestream.close();
  }
  return 0; 
}

string LinuxParser::Command(int pid) { 
  string line;
  string pid_str = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pid_str + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    filestream.close();
  }
  return line; 
}

string LinuxParser::Ram(int pid) { 
  string line;
  string key, value;
  string pid_str = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pid_str + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' '); // remove colon
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          removeChars(value, unit_str); // Get just number
          int temp = stoi(value); // Convert to number in kB
          temp = temp/1000.0; // convert to MB
          return std::to_string(temp) + " MB";
        }
      }
    }
    filestream.close();
  }
  return "Unknown"; 
}

string LinuxParser::Uid(int pid) { 
  string line;
  string key, value;
  string pid_str = std::to_string(pid);
  std::ifstream filestream(kProcDirectory + pid_str + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' '); // format for string stream
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid") {
          return value;
        }
      }
    }
    filestream.close();
  }
  return string(); 
}

string LinuxParser::User(int pid) { 
  string my_uid = Uid(pid);
  string line, key, value;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      removeChars(line,":x");
      std::replace(line.begin(), line.end(), ':', ' '); // format for string stream
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (value == my_uid) {
          return key;
        }
      }
    }
    filestream.close();
  }
  return string(); 
}

long LinuxParser::UpTime(int pid) { 
  std::vector<std::string> line_vec = parsePidStat(pid);
  int stime = stoi(line_vec.at(21));  // Per Linux documentaiton, uptime is this element in the file
  long uptime = (long)(stime/(float)clk_tcks);
  return uptime;
}

// ***** Helper Functions *****

// Based on Reference: https://stackoverflow.com/questions/5891610/how-to-remove-certain-characters-from-a-string-in-c
// and reference: https://stackoverflow.com/questions/32435003/how-to-remove-all-substrings-from-a-string
void LinuxParser::removeChars(std::string &str, const std::string &toRemove){
  std::regex pattern(toRemove);
  str = std::regex_replace(str,pattern,"");

  // for (uint i = 0; i < toRemove.length(); ++i){
  //   str.erase(std::remove(str.begin(),str.end(),toRemove[i]),str.end());
  // }
}

// Making this a helper function since it is used in more than one place
std::vector<std::string> LinuxParser::parsePidStat(int& pid){
  std::string line, value;
  std::string pid_str = std::to_string(pid);
  std::vector<std::string> line_vec;
  std::ifstream filestream(kProcDirectory + pid_str + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      line_vec.push_back(value);
    }
    filestream.close();
  }
  return line_vec;
}