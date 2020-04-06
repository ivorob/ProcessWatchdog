#include <iostream>
#include <thread>
#include <string>
#include <windows.h>

#include "ProcessWatchdog.h"

int
main(int argc, char *argv[])
{
    ProcessWatchdog processWatchdog;
    if (argc < 2) {
        processWatchdog = ProcessWatchdog(TEXT("notepad.exe"));
    } else {
        uint32_t processId = strtoul(argv[1], nullptr, 0);
        if (processId != 0) {
            processWatchdog = ProcessWatchdog(processId);
        }
    }
    
    processWatchdog.setCommandLineArguments({TEXT("C:\\temp\\1.txt")});
    std::wcout << "Watch for " << processWatchdog.getApplicationName() << std::endl;
    std::wcout << "Restart command line: " << processWatchdog.getCommandLine() << std::endl;

    while (1) {
        if (!processWatchdog.check()) {
            processWatchdog.restart();

            std::wcout << "Watch for " << processWatchdog.getApplicationName() << std::endl;
            std::wcout << "Restart command line: " << processWatchdog.getCommandLine() << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return EXIT_SUCCESS;
}
