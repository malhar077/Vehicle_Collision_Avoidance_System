#  Vehicle Collision Avoidance System (Linux IPC & System Calls)

##  Overview

This project simulates a **Vehicle Collision Avoidance System** using Linux system programming concepts.
Multiple vehicle processes communicate through **shared memory**, while a central controller continuously monitors vehicle positions and detects potential collisions.

The system mimics real-world behavior:

* Warning when vehicles come too close
* Automatic driver action (speed reduction)
* Delay using signals (reaction time simulation)
* Collision detection if danger persists
* Logging of all events for analysis

This project demonstrates **process control, multithreading, signals, and IPC mechanisms** in a clean and modular way.

---

## Directory Structure

```
Vehical_Collision_Avoidance_System/
│
├── src/
│   ├── main_controller.c     # Main logic (threads + signals + collision detection)
│   ├── create_vehicle.c     # Creates vehicle processes (fork + exec)
│   ├── vehicle_process.c    # Vehicle movement simulation
│
├── ipc/
│   ├── shared_memory.c      # Shared memory communication
│   ├── message_queue.c      # Message queue alerts
│   ├── fifo_comm.c          # FIFO (named pipe communication)
│   ├── pipe_comm.c          # Pipe communication
│   ├── semaphore.c          # Synchronization (mutex-like behavior)
│
├── include/
│   ├── common.h             # Structs and constants
│   ├── ipc.h                # IPC function declarations
│
├── log/
│   └── vehicle_log.txt      # Stores system events (history)
│
├── Makefile                 # Build automation
└── README.md
```

---

## Build & Run

### Build Project

```
make
```

### Run Full System

```
make run
```

### Test Controller Only

```
make test
```

### Stop All Processes

```
make stop
```

### Clean Build Files

```
make clean
```

---

## System Workflow

```
Vehicle Processes
      ↓
Update position in Shared Memory
      ↓
Main Controller (Threads)
      ↓
WARNING (distance ≤ 25)
      ↓
Speed Reduction
      ↓
Delay using SIGALRM (driver reaction time)
      ↓
COLLISION (if distance ≤ 10)
      ↓
Log + FIFO + Message Queue + STOP
```

---

## Signal Reference

| Signal  | Purpose                                      |
| ------- | -------------------------------------------- |
| SIGALRM | Introduces delay before confirming collision |
| SIGTERM | Stops system after collision                 |
| SIGINT  | Manual termination (Ctrl + C)                |

---

## IPC Architecture

```
[ Vehicle Processes ]
         ↓
   Shared Memory
         ↓
   Main Controller
         ↓
 ┌───────────────┬───────────────┬───────────────┐
 | FIFO (Live)   | Message Queue | Log File      |
 | Alerts        | Communication | History       |
 └───────────────┴───────────────┴───────────────┘
```

---

## System Call Reference

| Category | System Call | Purpose                  |
| -------- | ----------- | ------------------------ |
| Process  | fork()      | Create vehicle processes |
| Process  | exec()      | Run vehicle program      |
| Process  | waitpid()   | Synchronize processes    |
| Signal   | signal()    | Register signal handlers |
| Signal   | kill()      | Terminate system         |
| File I/O | open()      | Open log file            |
| File I/O | write()     | Write logs               |
| File I/O | close()     | Close file               |
| IPC      | shmget()    | Create shared memory     |
| IPC      | shmat()     | Attach shared memory     |
| IPC      | msgget()    | Create message queue     |
| IPC      | msgsnd()    | Send message             |
| IPC      | mkfifo()    | Create FIFO              |
| IPC      | sem_init()  | Initialize semaphore     |

---

## Features

✔ Multi-process architecture (fork + exec)
✔ Multi-threaded controller
✔ Shared memory communication
✔ FIFO + Message Queue integration
✔ Semaphore-based synchronization
✔ Signal-based timing (real-time simulation)
✔ Event logging with timestamps
✔ Makefile automation

---

##  Log System

All events are stored in:

```
log/vehicle_log.txt
```

Example:

```
[10:21:05] WARNING: Vehicles getting close
[10:21:05] ACTION: Speed reduced
[10:21:09] COLLISION: Vehicles 1 & 2
[10:21:10] SYSTEM STOPPED
```

---


## Sample Output 

--- Vehicle Data ---
Vehicle 1 → position=0 speed=50
Vehicle 2 → position=50 speed=50
Vehicle 3 → position=100 speed=30

--- Vehicle Data ---
Vehicle 1 → position=10 speed=50
Vehicle 2 → position=40 speed=50
Vehicle 3 → position=102 speed=30

⚠️ WARNING: Vehicles 1 & 2 close
🚗 Driver Action: Reducing speed!

--- Vehicle Data ---
Vehicle 1 → position=19 speed=40
Vehicle 2 → position=31 speed=40
Vehicle 3 → position=104 speed=30

--- Vehicle Data ---
Vehicle 1 → position=27 speed=40
Vehicle 2 → position=23 speed=40
Vehicle 3 → position=106 speed=30

🚨 COLLISION DETECTED (1 & 2)
🚗 System stopped due to collision!
