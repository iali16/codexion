*This project has been created as part of the 42 curriculum by drakotov.*

# Codexion - Coder Dongle Simulation

## 📖 Description

Codexion is a multi-threaded simulation of software developers (coders) competing for limited resources (dongles) to complete compilation tasks. The simulation models real-world scenarios where developers need to acquire resources, compile code, debug, and refactor within time constraints.

The project demonstrates:
- **Multi-threading** with POSIX threads (pthread)
- **Resource management** with a priority queue
- **Scheduling algorithms** (FIFO and EDF)
- **Thread synchronization** with mutexes and condition variables
- **C89 compliance** with strict coding standards

## 🏗️ Architecture

### Overview

```
┌───────────────────────────────────────────────────┐
│                  CODEXION SIM                     │
├───────────────────────────────────────────────────┤
│                                                   │
│  ┌──────────┐    ┌──────────┐    ┌──────────┐     │
│  │  CODERS  │    │  DONGLE  │    │ MONITOR  │     │
│  │   (N)    │◄──►│  QUEUE   │    │  THREAD  │     │
│  │  THREADS │    │  (HEAP)  │    │          │     │
│  └──────────┘    └──────────┘    └──────────┘     │
│       │               │               │           │
│       ▼               ▼               ▼           │
│  ┌─────────────────────────────────────────────┐  │
│  │           SHARED STATE (t_sim)              │  │
│  │     - Configuration       - Threads         │  │
│  │     - State               - lock            │  │
│  └─────────────────────────────────────────────┘  │
└───────────────────────────────────────────────────┘
```

### Components

| Component | Description |
|-----------|-------------|
| **Coders** | Developer threads that acquire dongles, compile, debug, and refactor |
| **Dongles** | Limited resources (N) that coders compete for |
| **Monitor** | Watches for burnout conditions and stops simulation |
| **Scheduler** | Manages dongle allocation (FIFO or EDF) |
| **Logger** | Thread-safe event logging with timestamps |

### Scheduling Algorithms

- **FIFO (First In, First Out)**: Coders acquire dongles in order of request
- **EDF (Earliest Deadline First)**: Coders with earlier deadlines get priority

## 🚀 Getting Started

### Prerequisites

- **Compiler**: GCC or Clang with C89 support
- **Libraries**: pthread (POSIX threads)
- **OS**: Linux/Unix (tested on Ubuntu, macOS)

### Installation

```bash
# Clone the repository
git clone <repository-url>
cd codexion

# Build the project
make

# Clean build files
make clean

# Remove executable and objects
make fclean

# Rebuild everything
make re
```

### Usage

```bash
./codexion <n> <burnout> <compile> <debug> <refactor> <required> <cooldown> <sched>
```

### Parameters

| Parameter | Description | Type | Example |
|-----------|-------------|------|---------|
| `n` | Number of coders/dongles | int > 0 | 10 |
| `burnout` | Maximum time before burnout (ms) | long | 200 |
| `compile` | Compilation time (ms) | long | 100 |
| `debug` | Debugging time (ms) | long | 50 |
| `refactor` | Refactoring time (ms) | long | 30 |
| `required` | Required compilations per coder | long | 3 |
| `cooldown` | Dongle cooldown time (ms) | long | 10 |
| `sched` | Scheduling algorithm | fifo/edf | fifo |

### Example

```bash
# Run with FIFO scheduling
./codexion 10 200 100 50 30 3 10 fifo

# Run with EDF scheduling
./codexion 10 200 100 50 30 3 10 edf
```

## 📊 Output Format

The simulation outputs events with timestamps in the following format:

```
<timestamp> <coder_num> <event>
```

### Events

| Event | Description |
|-------|-------------|
| `has taken a dongle` | Coder acquired a dongle |
| `is compiling` | Coder started compiling |
| `is debugging` | Coder started debugging |
| `is refactoring` | Coder started refactoring |
| `burned out` | Coder exceeded burnout time |

### Example Output

```
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
0 3 has taken a dongle
0 3 has taken a dongle
0 3 is compiling
0 5 has taken a dongle
0 5 has taken a dongle
0 5 is compiling
100 5 is debugging
100 3 is debugging
100 1 is debugging
150 1 is refactoring
150 5 is refactoring
150 3 is refactoring
200 1 burned out
```

## 🧪 Testing

### Memory Leak Check

```bash
valgrind --leak-check=full --show-leak-kinds=all ./codexion 10 200 100 50 30 3 10 fifo
```

### Thread Safety Check (Helgrind)

```bash
valgrind --tool=helgrind --history-level=full ./codexion 10 200 100 50 30 3 10 fifo
```

### Compare FIFO and EDF

```bash
# Run both schedulers and compare
./codexion 10 200 100 50 30 3 10 fifo > fifo_output.txt
./codexion 10 200 100 50 30 3 10 edf > edf_output.txt
diff fifo_output.txt edf_output.txt
```

### Different Test Scenarios

```bash
# Test with more coders and shorter burnout
./codexion 20 100 50 25 15 3 5 fifo

# Test with longer compile time
./codexion 10 200 200 50 30 3 10 edf

# Test with high required compilations
./codexion 10 200 100 50 30 5 10 fifo

# Test with minimal parameters
./codexion 5 100 50 25 15 2 5 edf
```

## 📁 Project Structure

```
codexion/
├── Makefile                 # Build configuration
├── README.md                # Documentation
├── inc/                     # Header files directory
│   └── codexion.h           # Main header file
└── src/                     # Source files directory
    ├── main.c               # Entry point
    ├── output.c             # Output utilities
    ├── time.c               # Time helpers
    ├── log.c                # Logging functions
    ├── parser.c             # Argument parsing
    ├── sim_utils.c          # Simulation utilities
    ├── scheduler.c          # Heap/priority queue operations
    ├── dongle.c             # Dongle operations
    ├── coder.c              # Coder routines
    ├── monitor.c            # Monitor thread
    └── init.c               # Initialization & cleanup
```

### File Descriptions

#### Core Files

| File | Functions | Description |
|------|-----------|-------------|
| `main.c` | `main()` | Program entry point, initializes simulation |
| `init.c` | `init_sim()`, `cleanup_sim()` | Simulation setup and cleanup |
| `parser.c` | `parse_args()`, `parse_nonnegative()` | Command-line argument parsing |
| `monitor.c` | `monitor_routine()` | Monitors burnout conditions |

#### Utility Files

| File | Functions | Description |
|------|-----------|-------------|
| `output.c` | `put_string()`, `put_number()` | Direct output using write() |
| `time.c` | `now_ms()`, `ms_to_timespec()` | Time measurement and conversion |
| `log.c` | `log_event()` | Thread-safe event logging |
| `sim_utils.c` | `sim_is_running()`, `sleep_ms()` | Simulation state helpers |

#### Resource Management Files

| File | Functions | Description |
|------|-----------|-------------|
| `dongle.c` | `dongle_init()`, `dongle_destroy()`, `dongle_acquire()`, `dongle_release()`, `stop_dongles()` | Dongle resource operations |
| `scheduler.c` | `waiter_less()`, `heap_push()`, `heap_remove()`, `heap_sift_up()`, `heap_sift_down()`, `heap_fix()`, `heap_swap()` | Priority queue for scheduling |
| `coder.c` | `coder_routine()`, `coder_acquire_two()`, `coder_release_two()`, `make_waiter()` | Coder thread logic |

## 🛠️ Development

### Coding Standards

The project follows strict **C89** standard with **Norminette** compliance:

| Rule | Description |
|------|-------------|
| **Function Length** | Max 25 lines (not counting braces) |
| **Variables** | Max 5 per function |
| **Parameters** | Max 4 per function |
| **Indentation** | Tabs only (no spaces) |
| **Comments** | `/* */` style only (no `//`) |
| **No Ternaries** | Use `if/else` instead of `? :` |
| **No For Loops** | Use `while` loops instead |
| **No Do-While** | Use `while` loops instead |
| **Headers** | 42 header required at top of each file |
| **Declarations** | Must be at top of block (C89 requirement) |

### Compilation Flags

```makefile
CFLAGS = -Wall -Wextra -Werror -pthread -std=c89 -pedantic -Iinc
```

| Flag | Purpose |
|------|---------|
| `-Wall -Wextra` | Enable all warnings |
| `-Werror` | Treat warnings as errors |
| `-pthread` | Link pthread library |
| `-std=c89 -pedantic` | Enforce C89 standard |
| `-Iinc` | Include header directory |

### Build Process

```bash
# Compile each .c file
$(CC) $(CFLAGS) -c src/main.c -o main.o
$(CC) $(CFLAGS) -c src/output.c -o output.o
# ... etc

# Link objects into executable
$(CC) $(CFLAGS) -o codexion main.o output.o ...
```

### Makefile Targets

| Target | Description |
|--------|-------------|
| `all` | Build the executable (default) |
| `clean` | Remove object files |
| `fclean` | Remove object files and executable |
| `re` | Rebuild everything (fclean + all) |

## 🔧 Troubleshooting

### Common Issues

| Issue | Solution |
|-------|----------|
| `command not found: make` | Install build-essential: `sudo apt install build-essential` |
| `pthread not found` | Install pthread: `sudo apt install libpthread-stubs0-dev` |
| `codexion.h: No such file or directory` | Ensure `-Iinc` flag is in CFLAGS |
| Compilation errors with C89 | No `//` comments, declarations at top of blocks |
| Valgrind errors | Check for data races with `--tool=helgrind` |
| Norminette errors | Run `norminette` and fix reported issues |

### Debug Build

```bash
make CFLAGS="-Wall -Wextra -pthread -std=c89 -pedantic -g -Iinc"
```

### Testing Different Schedulers

```bash
# Compare FIFO and EDF outputs
./codexion 10 200 100 50 30 3 10 fifo > fifo_output.txt
./codexion 10 200 100 50 30 3 10 edf > edf_output.txt
diff fifo_output.txt edf_output.txt
```

## 📊 Performance

### Time Complexity

| Operation | Complexity |
|-----------|------------|
| Dongle acquisition | O(log N) |
| Dongle release | O(log N) |
| Heap push | O(log N) |
| Heap remove | O(log N) |
| Coder cycle | O(N) for completion check |

### Space Complexity

| Component | Memory Usage |
|-----------|--------------|
| Dongles | O(N) |
| Coders | O(N) |
| Heap | O(N) |
| Total | O(N) |

### Performance Considerations

- **Memory Usage**: O(N) for dongles and coders
- **Thread Safety**: Mutex locks for all shared data
- **Scalability**: Handles up to INT_MAX coders (practical limit depends on system)
- **Bottleneck**: Heap operations are the main performance factor
- **Optimization**: EDF scheduling requires additional comparisons

## 🗂️ Resources

### Official documentation

- POSIX Threads overview:
  https://man7.org/linux/man-pages/man7/pthreads.7.html
- pthread_create:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html
- pthread_join:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
- pthread_mutex_init:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
- pthread_cond_wait:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html
- pthread_cond_timedwait:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html
- pthread_cond_broadcast:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_broadcast.html
- gettimeofday:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/gettimeofday.html
- usleep:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/usleep.html
- write:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/write.html
- malloc:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/malloc.html
- free:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/free.html

### Concurrency theory

- Dining philosophers problem:
  https://en.wikipedia.org/wiki/Dining_philosophers_problem
- Coffman conditions:
  https://en.wikipedia.org/wiki/Coffman_conditions
- Deadlock prevention:
  https://en.wikipedia.org/wiki/Deadlock_prevention_algorithms
- Earliest Deadline First scheduling:
  https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling
- FIFO scheduling:
  https://en.wikipedia.org/wiki/FIFO_(computing_and_electronics)
- Binary heap:
  https://en.wikipedia.org/wiki/Binary_heap
- Priority queue:
  https://en.wikipedia.org/wiki/Priority_queue

### Debugging tools

- Valgrind:
  https://valgrind.org/docs/manual/
- Valgrind Memcheck:
  https://valgrind.org/docs/manual/mc-manual.html
- Valgrind Helgrind:
  https://valgrind.org/docs/manual/hg-manual.html
- ThreadSanitizer:
  https://clang.llvm.org/docs/ThreadSanitizer.html

### AI usage

AI was used to assist with:

- brainstorming the overall architecture,
- understanding how to model dongles, coders, and waiting queues,
- reviewing possible deadlock and data-race scenarios,
- designing the FIFO/EDF heap comparator,
- preparing test cases,
- structuring the README,
- preparing for peer evaluation and live coding.

All AI-assisted code and explanations were reviewed, tested, and discussed with peers before submission. The final implementation was verified to ensure compliance with the subject constraints.
