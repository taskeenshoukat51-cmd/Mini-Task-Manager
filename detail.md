# Mini Task Manager (Process Manager)

## Overview
A console-based Mini Task Manager built in C++ using 
Windows API. This project simulates a real operating 
system task manager that can list, suspend, resume, 
and terminate processes by PID with real-time CPU 
and memory usage monitoring with timestamped logging.

---

## Features
- List all currently running processes with PID
- Kill/Terminate any process by PID
- Suspend any running process by PID
- Resume any suspended process by PID
- View CPU time usage of any process
- View Memory usage of any process
- Real-time timestamp on every log action
- Progress indicator for operations

---

### Menu Options
========================================
## MINI TASK MANAGER (Process Manager)
System Time: HH:MM:SS

- List Running Processes
- Kill a Process
- Suspend a Process
- Resume a Process
- Show CPU & Memory Usage
- Exit

---

## Windows API Functions Used
| API Function | Purpose |
|---|---|
| CreateToolhelp32Snapshot | Take snapshot of running processes |
| Process32First / Process32Next | Iterate through all processes |
| Thread32First / Thread32Next | Iterate through all threads |
| OpenProcess | Open a process by PID |
| TerminateProcess | Kill a process by PID |
| SuspendThread | Suspend all threads of a process |
| ResumeThread | Resume all threads of a process |
| GetProcessTimes | Get CPU time of a process |
| VirtualQueryEx | Get memory usage of a process |
| GetLocalTime | Get current system time |

---

## Functions
| Function | Description |
|---|---|
| listProcesses() | Shows all running processes with PID |
| killProcess() | Terminates process by PID |
| suspendProcess() | Suspends all threads of a process |
| resumeProcess() | Resumes all threads of a process |
| showProcessUsage() | Shows CPU time and memory usage |
| getProcessMemory() | Calculates memory used by process |
| getTimeStamp() | Returns current system time |
| logMessage() | Prints timestamped log message |
| showProgress() | Shows processing animation |

---

## Logging System
Every action is logged with timestamp:
[LOG 14:35:22] Process list requested
[LOG 14:35:45] Process 1234 suspended
[LOG 14:36:10] Process 1234 resumed
[LOG 14:36:30] Process 1234 terminated

---

## Resource Monitoring
| Resource | How Measured |
|---|---|
| CPU Time | Kernel time + User time in milliseconds |
| Memory | Committed memory regions in KB |

---

## Technologies Used
- **Language:** C++
- **OS API:** Windows API (win32)
- **Libraries:** windows.h, tlhelp32.h,
  iostream, iomanip, string
- **IDE:** Visual Studio Code
- **Platform:** Windows Only

---

## How to Run
1. Open project in Visual Studio Code
2. Compile the file:
---

## Resource Monitoring
| Resource | How Measured |
|---|---|
| CPU Time | Kernel time + User time in milliseconds |
| Memory | Committed memory regions in KB |

---

## Technologies Used
- **Language:** C++
- **OS API:** Windows API (win32)
- **Libraries:** windows.h, tlhelp32.h,
  iostream, iomanip, string
- **IDE:** Visual Studio Code
- **Platform:** Windows Only

---

## How to Run
1. Open project in Visual Studio Code
2. Compile the file:

g++ task_manager.cpp -o task_manager -lpsapi

3. Run as Administrator (required for process control):

./task_manager

4. Use menu options to manage processes!

---

## Important Note
> ⚠️ Run this program as **Administrator**
> Some processes require admin privileges 
> to suspend, resume or terminate.

---

## OS Concepts Used
- Process Management
- Thread Management
- CPU Scheduling
- Memory Management
- System Calls via Windows API

---

## Limitations
- Works on **Windows only**
- Some system processes cannot be terminated
- Requires Administrator privileges for full access

---

## Future Improvements
- Add GUI interface
- Add real-time CPU percentage monitoring
- Add process priority control
- Add network usage monitoring
- Port to Linux using /proc filesystem

---

## Contact
Wajeeha Attiq  
GitHub: github.com/wajeehaattiq  
Email: wajeehaattiq7@gmail.com 
