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
+---------------------------------------------------------------------+
|                            CODEXION SIM                             |
+---------------------------------------------------------------------+
|                                                                     |
|  +-------------+  +-------------+  +-------------+  +------------+  |
|  |  CODERS (n) |  |  DONGLE (n) |  |   MONITOR   |  |   LOGGER   |  |
|  | n threads   |  | mutex/cond  |  |  1 thread   |  | log_lock   |  |
|  |   compile   |  | heap        |  |   burnout   |  | 1 write()  |  |
|  |   debug     |  | FIFO | EDF  |  |   watcher   |  | per line   |  |
|  |   refactor  |  |             |  |             |  |            |  |
|  +------+------+  +------+------+  +------+------+  +------+-----+  |
|         |                |                |                |        |
|         v                v                v                v        |
|  +---------------------------------------------------------------+  |
|  |             SHARED STATE (t_sim) · guarded by sim->lock       |  |
|  |   config · state · threads · dongles · coders · counters      |  |
|  +---------------------------------------------------------------+  |
+---------------------------------------------------------------------+
```

### Components

| Component | Description |
|-----------|-------------|
| **Coders** | Developer threads that acquire dongles, compile, debug, and refactor |
| **Dongles** | Limited resources (n) that coders compete for |
| **Monitor** | Watches for burnout conditions and stops simulation |
| **Scheduler** | Manages dongle allocation (FIFO or EDF) |
| **Logger** | Thread-safe event logging with timestamps |

### Scheduling Algorithms

- **FIFO (First In, First Out)**: Coders acquire dongles in order of request
- **EDF (Earliest Deadline First)**: Coders with earlier deadlines get priority

## 🚀 Instructions

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
.
├── README.md                # Documentation
└── coders/                  # Project directory
    ├── Makefile             # Build configuration
    ├── inc/                 # Header files directory
    │   └── codexion.h       # Main header file
    └── src/                 # Source files directory
        ├── main.c           # Entry point
        ├── parser.c         # Argument parsing
        ├── output.c         # Output utilities
        ├── time.c           # Time helpers
        ├── log.c            # Logging functions
        ├── sim_utils.c      # Simulation utilities
        ├── sim_state.c      # Simulation state helpers
        ├── init.c           # Initialization & cleanup
        ├── dongle.c         # Dongle operations
        ├── dongle_wait.c    # Dongle waiting queue
        ├── heap.c           # Heap/priority queue operations
        ├── scheduler.c      # Scheduling helpers
        ├── coder.c          # Coder routines
        ├── coder_acquire.c  # Dongle acquisition logic
        ├── coder_cycle.c    # Compile/debug/refactor cycle
        ├── monitor.c        # Monitor thread
        ├── monitor_check.c  # Burnout and completion checks
        └── threads.c        # Thread creation and joining
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

The project follows strict **C89** standard with **Norminette** compliance.

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

## 📊 Performance

### Time Complexity

| Operation | Complexity |
|-----------|------------|
| Dongle acquisition | O(log n) |
| Dongle release | O(log n) |
| Heap push | O(log n) |
| Heap remove | O(log n) |
| Coder cycle | O(n) for completion check |

### Space Complexity

| Component | Memory Usage |
|-----------|--------------|
| Dongles | O(n) |
| Coders | O(n) |
| Heap | O(n) |
| Total | O(n) |

### Performance Considerations

- **Memory Usage**: O(n) for dongles and coders
- **Thread Safety**: Mutex locks for all shared data
- **Scalability**: Handles up to INT_MAX coders (practical limit depends on system)
- **Bottleneck**: Heap operations are the main performance factor
- **Optimization**: EDF scheduling requires additional comparisons


## 🚫 Blocking cases handled

This section describes the main concurrency issues handled by the implementation.

### ↘️ Deadlock prevention and Coffman’s conditions

Deadlock is prevented by breaking the circular-wait condition.

Each dongle has a fixed numeric identifier. When a coder needs two dongles, the coder always requests them in the same global order:

1. the lower-numbered dongle is requested first,
2. the higher-numbered dongle is requested second,
3. both dongles are released after compiling.

Because all coders follow the same acquisition order, a circular dependency between coders and dongles cannot form.

This can be explained using Coffman’s conditions:

***Mutual exclusion:***
Each dongle is protected by its own mutex. Only one coder can own a dongle at a time.

***Hold and wait:***
A coder may hold one dongle while waiting for the second one. However, this cannot produce a circular wait because dongles are always acquired in the same global order.

***No preemption:***
Dongles are not taken by force. A coder releases dongles voluntarily after compiling.

***Circular wait:***
Circular wait is eliminated by the fixed dongle acquisition order. Since no coder can hold a higher-numbered dongle while waiting for a lower-numbered one, a deadlock cycle cannot occur.

---

### ↘️ Starvation prevention

Each dongle maintains a waiting queue implemented as a binary heap.

When multiple coders request the same dongle, the dongle grants access according to the selected scheduler.

***FIFO:***
With FIFO, the heap orders waiters by arrival sequence. The request that arrived first is served first.

***EDF:***
With EDF, the heap orders waiters by burnout deadline:

```text
deadline = last_compile_start + time_to_burnout
```

The coder with the earliest deadline is served first.

If two EDF deadlines are equal, the implementation uses a deterministic tie-breaker: the higher coder number is preferred.

This guarantees that dongle arbitration is not left to arbitrary thread scheduling.

---

### ↘️ Dongle cooldown handling

After a dongle is released, it cannot be reused immediately.

Each dongle stores a timestamp representing when it becomes available again:

```text
available_at = current_time + dongle_cooldown
```

A waiter can take the dongle only when:

```text
current_time >= available_at
```

This ensures that the cooldown constraint is respected even under contention.

The cooldown timestamp is protected by the dongle mutex, so no thread can read or modify it while another thread is updating the dongle state.

---

### ↘️ Burnout detection

A monitor thread checks burnout deadlines.

Each coder has a burnout deadline:

```text
last_compile_start + time_to_burnout
```

If a coder has not started compiling before that deadline, the monitor detects the burnout and stops the simulation.

The monitor checks the shared state frequently enough to respect the required precision:

> The burnout message must be displayed within 10 ms of the actual burnout time.

The monitor does not rely on coders communicating with each other. It observes protected shared state and stops the simulation when a burnout condition is detected.

---

### ↘️ Log serialization

All state-change messages are printed through a single logging function.

This function uses a dedicated log mutex so that two threads can never write at the same time. This prevents interleaved or partially mixed log lines.

The logger produces messages in the required format:

```text
timestamp_in_ms X has taken a dongle
timestamp_in_ms X is compiling
timestamp_in_ms X is debugging
timestamp_in_ms X is refactoring
timestamp_in_ms X burned out
```

Log serialization guarantees that each displayed message remains atomic with respect to other threads.

---

## 🚦 Thread synchronization mechanisms

This project uses POSIX threading primitives and a custom waiting/event mechanism built from protected shared state.

The main synchronization tools are:

- `pthread_mutex_t`
- `pthread_cond_t`
- a custom event/waiting implementation based on protected state, condition variables, and heap-based waiting queues

No global mutable variables are used. All shared state is stored inside structures passed explicitly to threads.

---

### `pthread_mutex_t`

Mutexes protect every piece of shared mutable state.

#### Dongle mutex

Each dongle has its own mutex.

The dongle mutex protects:
- the current dongle owner,
- the cooldown timestamp,
- the waiting queue,
- the stopped state of the dongle.

Any operation that reads or modifies the dongle state is performed while holding that dongle’s mutex.

Example pattern:

```c
pthread_mutex_lock(&dongle->lock);

if (dongle_is_available(dongle))
    grant_dongle_to_waiter(dongle, waiter);

pthread_mutex_unlock(&dongle->lock);
```

This prevents two coders from taking the same dongle at the same time.

---

#### Simulation state mutex

A simulation mutex protects shared simulation state, including:

- whether the simulation is still running,
- coder states,
- compile counters,
- last compile start times,
- burnout information.

Coders update their state under this mutex. The monitor reads coder state under the same mutex.

Example:

```c
pthread_mutex_lock(&sim->lock);

coder->last_compile_start = current_time;
coder->state = STATE_COMPILING;

pthread_mutex_unlock(&sim->lock);
```

This prevents a race between a coder updating its state and the monitor checking burnout deadlines.

---

#### Log mutex

A separate mutex protects logging.

Every log message is written while holding the log mutex. This ensures that messages are not interleaved.

Example:

```c
pthread_mutex_lock(&sim->log_lock);

write_log_message(...);

pthread_mutex_unlock(&sim->log_lock);
```

This guarantees that each log line is written atomically with respect to other threads.

---

### `pthread_cond_t`

Each dongle also has a condition variable.

The condition variable is used when a coder cannot immediately take a dongle. The coder waits on the dongle’s condition variable while still holding the dongle mutex.

This is important because `pthread_cond_wait()` and `pthread_cond_timedwait()` atomically release the mutex and put the thread to sleep. When the thread wakes up, it reacquires the same mutex before continuing.

Example pattern:

```c
pthread_mutex_lock(&dongle->lock);

while (!dongle_can_be_granted(dongle, waiter))
    pthread_cond_timedwait(&dongle->cond, &dongle->lock, &timeout);

if (dongle_can_be_granted(dongle, waiter))
    grant_dongle_to_waiter(dongle, waiter);

pthread_mutex_unlock(&dongle->lock);
```

This prevents the following race condition:

1. Thread A checks whether the dongle is available.
2. Thread B changes the dongle state.
3. Thread A waits using outdated information.

Because the check and the wait happen while holding the mutex, the state cannot change between the check and the wait.

---

### Custom event implementation

The project does not use an external event library. Instead, events are implemented using a combination of:

- protected state,
- mutexes,
- condition variables,
- heap-based waiting queues.

The main events are:

- dongle released,
- dongle cooldown expired,
- simulation stopped,
- burnout detected,
- required number of compiles reached.

These events are not represented by unsynchronized shared flags. They are represented by state changes inside mutex-protected structures.

For example:

- when a dongle is released, its state is updated under its mutex and waiting threads are notified,
- when the monitor detects burnout, the simulation state is marked as stopped under the simulation mutex,
- when the simulation stops, dongles are marked as stopped under their own mutexes,
- coder threads observe these protected state changes and exit cleanly.

This design avoids busy waiting and provides safe thread communication.

---

### Coordination of shared resources

#### Dongles

Dongles are the main shared resource.

Each dongle has:

- a mutex,
- a condition variable,
- an owner,
- a cooldown timestamp,
- a priority queue of waiting coders.

All access to these fields is synchronized.

This ensures:

- no duplicate dongle usage,
- correct cooldown behavior,
- fair arbitration between waiting coders,
- safe wake-up when the dongle becomes available.

---

#### Logging

Logging is a shared output resource.

If two threads printed at the same time, their messages could become mixed. To prevent this, all logs go through one serialized logging function protected by a mutex.

This ensures that the output always remains well-formed and readable.

---

#### Monitor state

The monitor thread must observe coder state without introducing race conditions.

Coders publish their state by updating protected fields:

- current state,
- last compile start time,
- compile count.

The monitor reads those fields using the same simulation mutex.

This gives the monitor a consistent view of the simulation without requiring direct communication between coders.

---

### Examples of race-condition prevention

#### Example 1: protecting dongle ownership

Without synchronization, two coders could believe they own the same dongle.

With synchronization:

```c
pthread_mutex_lock(&dongle->lock);

if (dongle->owner == -1 && current_time >= dongle->available_at)
{
    dongle->owner = coder_id;
}

pthread_mutex_unlock(&dongle->lock);
```

Only one thread can execute this critical section at a time, so the owner field cannot be corrupted.

---

#### Example 2: protecting burnout state

Without synchronization, the monitor could read a coder’s state while a coder is updating it.

With synchronization:

```c
pthread_mutex_lock(&sim->lock);

coder->state = STATE_COMPILING;
coder->last_compile_start = now_ms();

pthread_mutex_unlock(&sim->lock);
```

The monitor also uses:

```c
pthread_mutex_lock(&sim->lock);

check_all_coder_deadlines(sim);

pthread_mutex_unlock(&sim->lock);
```

This prevents inconsistent observations.

---

#### Example 3: protecting logs

Without synchronization, two threads could write simultaneously and produce mixed output.

With a log mutex, each line is written completely before another thread can write.

This guarantees that messages such as:

```text
12 3 is compiling
```

are never mixed with other messages.

---

### Thread-safe communication between coders and the monitor

Coders do not communicate directly with each other. They also do not send direct messages to the monitor.

Instead, communication happens through protected shared state.

#### Coder to monitor communication

When a coder starts compiling, it updates its state:

```c
pthread_mutex_lock(&sim->lock);

coder->state = STATE_COMPILING;
coder->last_compile_start = current_time;

pthread_mutex_unlock(&sim->lock);
```

The monitor later reads this state under the same mutex.

This allows the monitor to know whether the coder has started compiling before the burnout deadline.

---

#### Monitor to coder communication

When the monitor detects burnout, it stops the simulation:

```c
pthread_mutex_lock(&sim->lock);

sim->running = 0;

pthread_mutex_unlock(&sim->lock);
```

Then the dongles are marked as stopped under their own mutexes.

Coder threads observe the stopped state through synchronized checks and exit cleanly.

This provides safe communication without requiring coders to share information directly.

## 🗂️ Resources

### Official documentation

- POSIX Threads overview:
  https://man7.org/linux/man-pages/man7/pthreads.7.html
- pthread_create, _join, _mutex_init, cond_wait, _cond_timedwait, _cond_broadcast, _gettimeofday, _usleep:
  https://pubs.opengroup.org/onlinepubs/9699919799/functions/

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

### AI usage

AI was used to assist with:

- understanding how to model dongles, coders, and waiting queues
- reviewing possible deadlock and data-race scenarios
- preparing test cases
- structuring the README

All AI-assisted code and explanations were reviewed, tested, and discussed with peers before submission. The final implementation was verified to ensure compliance with the subject constraints.
