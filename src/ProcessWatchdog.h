#pragma once

#include <string>
#include <vector>

class ProcessWatchdog {
public:
    ProcessWatchdog();
    ProcessWatchdog(uint32_t processId);
    ProcessWatchdog(const std::wstring& applicationName);

    std::wstring getApplicationName() const;

    std::wstring getCommandLine() const;
    void setCommandLineArguments(const std::vector<std::wstring>& arguments);

    bool check();
    bool restart();
private:
    void updateProcessInfo();
    void updateCommandLine();
private:
    uint32_t processId;
    std::wstring applicationName;
    std::wstring commandLine;
    std::vector<std::wstring> arguments;
};
