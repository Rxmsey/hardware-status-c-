#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unistd.h>
#include <chrono>
#include <sys/sysinfo.h>

using namespace std;

// Function to get the current CPU usage
int getCPUUsage()
{
    // Read the "/proc/stat" file
    ifstream statFile("/proc/stat");

    // Read the first line, which contains the overall CPU usage
    string line;
    getline(statFile, line);

    // Split the line by space
    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    // Calculate the total CPU time
    long long totalTime = 0;
    for (int i = 1; i < tokens.size(); i++)
        totalTime += stoi(tokens[i]);

    // Close the file and sleep for one second
    statFile.close();
    sleep(1);

    // Read the "/proc/stat" file again
    statFile.open("/proc/stat");
    getline(statFile, line);
    statFile.close();

    // Split the line by space
    ss.str(line);
    ss.clear();
    tokens.clear();
    while (getline(ss, token, ' '))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    // Calculate the total CPU time again
    long long totalTime2 = 0;
    for (int i = 1; i < tokens.size(); i++)
        totalTime2 += stoi(tokens[i]);

    // Calculate the CPU usage
    int usage = (totalTime2 - totalTime) * 100 / (totalTime2 - totalTime);
    return usage;
}

// Function to get the total and available memory
pair<int, int> getMemoryInfo()
{
    // Use the sysinfo function to get the memory information
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    // Calculate the total and available memory
    long long totalMemory = memInfo.totalram;
    totalMemory *= memInfo.mem_unit;
    long long availableMemory = memInfo.freeram;
    availableMemory *= memInfo.mem_unit;

    // Convert the memory to MB and return it as a pair
    return { (int)(totalMemory / (1024 * 1024)), (int)(availableMemory / (1024 * 1024)) };
}

// Function to get the temperature of the CPU
int getCPUTemp()
{
    // Read the "/sys/class/thermal/thermal_zone0/temp" file
    ifstream tempFile("/sys/class/thermal/thermal_zone0/temp");

    // Read the temperature in millidegrees Celsius
    int temp;
    tempFile >> temp;

    // Close the file and convert the temperature to degrees Celsius
    tempFile.close();
    return temp / 1000;
}

int main()
{
    // Get the CPU usage
    int cpuUsage = getCPUUsage();
    cout << "CPU usage: " << cpuUsage << "%" << endl;

    // Get the total and available memory
    pair<int, int> memoryInfo = getMemoryInfo();
    cout << "Total memory: " << memoryInfo.first << "MB" << endl;
    cout << "Available memory: " << memoryInfo.second << "MB" << endl;

    // Get the CPU temperature
    int cpuTemp = getCPUTemp();
    cout << "CPU temperature: " << cpuTemp << "C" << endl;

    return 0;
}
