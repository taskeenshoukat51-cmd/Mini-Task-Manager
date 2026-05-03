#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;


/* -------------------- Utility Functions -------------------- */

// Get current time for logs
string getTimeStamp() {
    SYSTEMTIME time;
    GetLocalTime(&time);

    char buffer[50];
    sprintf(buffer, "%02d:%02d:%02d",
            time.wHour, time.wMinute, time.wSecond);

    return string(buffer);
}

// Print log message with time
void logMessage(string msg) {
    cout << "[LOG " << getTimeStamp() << "] " << msg << endl;
}

// Simple progress effect
void showProgress() {
    cout << "Processing";
    for (int i = 0; i < 3; i++) {
        Sleep(300);
        cout << ".";
    }
    cout << endl;
}

/* -------------------- Step 1: Process Listing -------------------- */

void listProcesses() {
    logMessage("Process list requested");

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    process.dwSize = sizeof(PROCESSENTRY32);

    if (snapshot == INVALID_HANDLE_VALUE) {
        cout << "Error: Unable to create snapshot.\n";
        return;
    }

    if (!Process32First(snapshot, &process)) {
        cout << "Error: Unable to read process list.\n";
        CloseHandle(snapshot);
        return;
    }

    cout << "\n================ RUNNING PROCESSES ================\n";
    cout << "PID\t\tProcess Name\n";
    cout << "--------------------------------------------------\n";

    do {
        cout << process.th32ProcessID << "\t\t"
             << process.szExeFile << endl;
    } while (Process32Next(snapshot, &process));

    CloseHandle(snapshot);
}

/* -------------------- Step 2: Kill Process -------------------- */

void killProcess() {
    DWORD pid;
    cout << "\nEnter PID to terminate: ";
    cin >> pid;

    showProgress();

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    if (hProcess == NULL) {
        cout << "Error: Access denied or invalid PID.\n";
        return;
    }

    if (TerminateProcess(hProcess, 0)) {
        cout << "Process terminated successfully.\n";
        logMessage("Process " + to_string(pid) + " terminated");
    } else {
        cout << "Failed to terminate process.\n";
    }

    CloseHandle(hProcess);
}

/* -------------------- Step 3: Suspend Process -------------------- */

void suspendProcess() {
    DWORD pid;
    cout << "\nEnter PID to suspend: ";
    cin >> pid;

    showProgress();

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 thread;
    thread.dwSize = sizeof(THREADENTRY32);

    bool found = false;

    if (Thread32First(snapshot, &thread)) {
        do {
            if (thread.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE,
                                            thread.th32ThreadID);
                if (hThread) {
                    SuspendThread(hThread);
                    CloseHandle(hThread);
                    found = true;
                }
            }
        } while (Thread32Next(snapshot, &thread));
    }

    CloseHandle(snapshot);

    if (found) {
        cout << "Process suspended successfully.\n";
        logMessage("Process " + to_string(pid) + " suspended");
    } else {
        cout << "Error: Process not found or access denied.\n";
    }
}

/* -------------------- Step 3: Resume Process -------------------- */

void resumeProcess() {
    DWORD pid;
    cout << "\nEnter PID to resume: ";
    cin >> pid;

    showProgress();

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    THREADENTRY32 thread;
    thread.dwSize = sizeof(THREADENTRY32);

    bool found = false;

    if (Thread32First(snapshot, &thread)) {
        do {
            if (thread.th32OwnerProcessID == pid) {
                HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE,
                                            thread.th32ThreadID);
                if (hThread) {
                    ResumeThread(hThread);
                    CloseHandle(hThread);
                    found = true;
                }
            }
        } while (Thread32Next(snapshot, &thread));
    }

    CloseHandle(snapshot);

    if (found) {
        cout << "Process resumed successfully.\n";
        logMessage("Process " + to_string(pid) + " resumed");
    } else {
        cout << "Error: Process not found or access denied.\n";
    }
}

/* -------------------- Step 4: CPU & Memory Usage -------------------- */

// Helper: Get approximate memory usage of process
SIZE_T getProcessMemory(DWORD pid) {
    SIZE_T memUsage = 0;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess) {
        MEMORY_BASIC_INFORMATION mbi;
        char* addr = 0;
        while (VirtualQueryEx(hProcess, addr, &mbi, sizeof(mbi)) == sizeof(mbi)) {
            if (mbi.State == MEM_COMMIT)
                memUsage += mbi.RegionSize;
            addr += mbi.RegionSize;
        }
        CloseHandle(hProcess);
    }
    return memUsage;
}

void showProcessUsage() {
    DWORD pid;
    cout << "\nEnter PID to view CPU & Memory usage: ";
    cin >> pid;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (hProcess == NULL) {
        cout << "Error: Unable to open process.\n";
        return;
    }

    // CPU Time
    FILETIME ftCreation, ftExit, ftKernel, ftUser;
    if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &ftKernel, &ftUser)) {
        ULARGE_INTEGER kTime, uTime;
        kTime.LowPart = ftKernel.dwLowDateTime;
        kTime.HighPart = ftKernel.dwHighDateTime;
        uTime.LowPart = ftUser.dwLowDateTime;
        uTime.HighPart = ftUser.dwHighDateTime;

        double cpuTime = (kTime.QuadPart + uTime.QuadPart) / 10000.0;
        cout << fixed << setprecision(2);
        cout << "CPU Time Used: " << cpuTime << " ms\n";
    } else {
        cout << "Unable to read CPU usage.\n";
    }

    // Memory Usage
    SIZE_T mem = getProcessMemory(pid);
    cout << "Memory Usage (approx.): " << mem / 1024 << " KB\n";

    logMessage("Resource usage checked for PID " + to_string(pid));
    CloseHandle(hProcess);
}

/* -------------------- MAIN MENU -------------------- */

int main() {
    int choice;

    do {
        cout << "\n========================================\n";
        cout << " MINI TASK MANAGER (Process Manager)\n";
        cout << " System Time: " << getTimeStamp() << endl;
        cout << "========================================\n";
        cout << "1. List Running Processes\n";
        cout << "2. Kill a Process\n";
        cout << "3. Suspend a Process\n";
        cout << "4. Resume a Process\n";
        cout << "5. Show CPU & Memory Usage\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            listProcesses();
            break;
        case 2:
            killProcess();
            break;
        case 3:
            suspendProcess();
            break;
        case 4:
            resumeProcess();
            break;
        case 5:
            showProcessUsage();
            break;
        case 0:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE);
cout << " MINI TASK MANAGER \n";
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    return 0;
}
