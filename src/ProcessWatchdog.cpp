#include <windows.h>
#include <sstream>
#include <psapi.h>

#include "ProcessWatchdog.h"

namespace {

const uint32_t StillAlive = 259;

}

ProcessWatchdog::ProcessWatchdog()
    : processId()
{
}

ProcessWatchdog::ProcessWatchdog(uint32_t processId)
    : processId(processId)
{
    updateProcessInfo();
}

ProcessWatchdog::ProcessWatchdog(const std::wstring& applicationName)
    : processId(),
      applicationName(applicationName)
{
    updateCommandLine();
}

std::wstring
ProcessWatchdog::getApplicationName() const
{
    return this->applicationName;
}

void
ProcessWatchdog::updateProcessInfo()
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processId);
    if (processHandle != nullptr) {
        std::vector<wchar_t> buffer(1024);
        size_t size = GetModuleFileNameEx(processHandle, nullptr, &buffer[0], static_cast<DWORD>(buffer.size()));
        if (size > buffer.size()) {
            buffer.resize(size + 1);
            if (GetModuleFileNameEx(processHandle, nullptr, &buffer[0], static_cast<DWORD>(size)) == size) {
                this->applicationName = std::wstring(&buffer[0], size);
            }
        } else {
            this->applicationName = std::wstring(&buffer[0], size);
        }

        CloseHandle(processHandle);

        updateCommandLine();
    }
}

std::wstring
ProcessWatchdog::getCommandLine() const
{
    return this->commandLine;
}

void
ProcessWatchdog::setCommandLineArguments(const std::vector<std::wstring>& arguments)
{
    this->arguments = arguments;

    updateCommandLine();
}

void
ProcessWatchdog::updateCommandLine()
{
    std::wostringstream output;
    output << this->applicationName << " ";
    for (const auto& argument : this->arguments) {
        output << argument << " ";
    }

    this->commandLine = output.str();
}

bool
ProcessWatchdog::check()
{
    if (processId != 0) {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, this->processId);
        if (processHandle != nullptr) {
            DWORD exitCode = StillAlive;
            if (GetExitCodeProcess(processHandle, &exitCode) && exitCode == StillAlive) {
                return true;
            }
        }
    }

    return false;
}

bool
ProcessWatchdog::restart()
{
    std::vector<wchar_t> buffer(std::begin(this->commandLine), std::end(this->commandLine));
    buffer.push_back(0);

    STARTUPINFO startupInfo = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION processInformation = {0};
    bool creationResult = CreateProcess(nullptr,
            &buffer[0],
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &startupInfo,
            &processInformation);
    if (creationResult) {
        CloseHandle(processInformation.hProcess);

        this->processId = processInformation.dwProcessId;
        updateProcessInfo();
    }

    return creationResult;
}
