*This project has been created as part of the 42 curriculum by drakotov.*



## Description

Codexion is a concurrency simulation in C using POSIX threads.

Multiple coders sit around a shared workspace and must compile quantum code.
To compile, each coder needs two adjacent USB dongles. After compiling, the
coder releases the dongles, then debugs, then refactors, then tries to compile
again.

The simulation stops when either:

- all coders have compiled the required number of times, or
- a coder burns out because they did not start compiling in time.

The project implements:

- POSIX threads,
- mutexes,
- condition variables,
- FIFO and EDF scheduling,
- dongle cooldown,
- burnout detection,
- serialized logging.

## Instructions

Build the project:

```bash
make
