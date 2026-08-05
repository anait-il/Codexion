
*This project has been created as part of the 42 curriculum by anait-il.*

# Codexion

> **Master the race for resources before the deadline masters you.**

Codexion is a multithreaded concurrency simulation written in C using the POSIX Threads (pthreads) library. It models a group of coders sharing a limited number of USB dongles while competing to compile before burning out.

The project focuses on thread synchronization, resource arbitration, scheduling algorithms, and precise timing constraints.

---

# Table of Contents

- Description
- Features
- Project Rules
- Instructions
- Usage
- Blocking Cases Handled
- Thread Synchronization Mechanisms
- Project Architecture
- Scheduler Implementation
- Resources
- AI Usage
- License

---

# Description

The goal of Codexion is to simulate multiple independent coder threads competing for shared USB dongles.

Each coder continuously performs the following cycle:

1. Compile
2. Debug
3. Refactor

To start compiling, a coder must acquire **two USB dongles**.

The simulation ends when:

- a coder burns out because they failed to compile before their deadline, or
- every coder has compiled the required number of times.

The project is designed to teach practical synchronization techniques including:

- POSIX threads
- Mutexes
- Condition variables
- Deadlock prevention
- Fair scheduling
- Race-condition avoidance
- Monitor threads
- Thread-safe logging
- Timing-sensitive concurrent programming

---

# Features

- POSIX Threads (`pthread`)
- Mutex protected shared resources
- Condition variables
- Monitor thread
- FIFO scheduling
- EDF (Earliest Deadline First) scheduling
- Heap-based priority queue
- Dongle cooldown support
- Accurate burnout detection
- Thread-safe logging
- Memory leak free
- Compatible with Helgrind and Valgrind

---

# Project Rules

Each coder is represented by its own thread.

Each dongle is a shared resource protected by synchronization primitives.

The monitor continuously checks whether:

- a coder burned out
- all coders completed the required number of compilations

The scheduler determines which waiting coder receives a released dongle.

Supported scheduling policies:

- FIFO (First In First Out)
- EDF (Earliest Deadline First)

---

# Instructions

## Compilation

```bash
make
```

Builds the executable:

```text
./codexion
```

Clean object files:

```bash
make clean
```

Remove objects and executable:

```bash
make fclean
```

Rebuild:

```bash
make re
```

---

## Execution

```bash
./codexion \
number_of_coders \
time_to_burnout \
time_to_compile \
time_to_debug \
time_to_refactor \
number_of_compiles_required \
dongle_cooldown \
scheduler
```

Example:

```bash
./codexion 5 800 200 200 200 5 50 fifo
```

or

```bash
./codexion 2 60 10 10 10 1 10 edf
```

---

# Usage

Example output:

```
0 2 has taken a dongle
0 2 has taken a dongle
0 2 is compiling
10 2 is debugging
20 2 is refactoring
20 1 has taken a dongle
20 1 has taken a dongle
20 1 is compiling

```

---

# Blocking Cases Handled

This project handles several classical concurrency problems.

## Deadlock Prevention

The four Coffman conditions are:

1. Mutual exclusion
2. Hold and wait
3. No preemption
4. Circular wait

Mutual exclusion is unavoidable because only one coder may own a dongle at a time.

Instead of allowing coders to grab dongles independently, dongle acquisition is arbitrated through a scheduler that grants access only when both required dongles are available. This removes circular waiting between neighboring coders and prevents deadlock.

---

## Starvation Prevention

Starvation is prevented through scheduling.

### FIFO

Coders receive dongles in the exact order they requested them.

Older requests cannot be indefinitely postponed.

### EDF

Requests are ordered by the earliest burnout deadline:

```
deadline = last_compile_start + time_to_burnout
```

The coder closest to burning out always receives the highest priority.

Provided the simulation parameters are feasible, EDF guarantees that no coder starves.

---

## Dongle Cooldown

After a coder releases a dongle, it immediately enters a cooldown period.

During this time the dongle cannot be assigned to another coder.

This simulates limited hardware reuse and prevents unrealistic immediate reacquisition.

---

## Burnout Detection

A dedicated monitor thread continuously checks every coder.

If:

```
time_elapsed_from_the_begin_of_the_simulation > last_compile_start + time_to_burnout
```

the simulation immediately stops and the burnout is logged.

The monitor ensures burnout messages are printed within 10ms.

---

## Log Serialization

Multiple threads may attempt to print simultaneously.

A dedicated logging mutex protects stdout so that:

- messages never interleave
- each log line remains complete
- timestamps stay consistent

---

# Thread Synchronization Mechanisms

## pthread_mutex_t

Mutexes protect every shared resource.

Examples include:

- dongle ownership
- scheduler queue
- logging
- monitor state
- shared simulation flags

Without mutexes, multiple threads could modify shared data simultaneously, leading to race conditions.

---

## pthread_cond_t

Condition variables are used whenever threads must sleep until an event occurs.

Examples:

- waiting for the simulation to begin
- waiting for dongles to become available
- waking waiting coders after resources are released

Unlike busy waiting, condition variables allow threads to sleep without consuming CPU time.

---

## Custom Event System

The implementation also uses custom synchronization events built on top of mutexes and condition variables.

These events coordinate:

- simulation startup
- scheduler notifications
- monitor wake-ups
- resource availability

This event-based approach minimizes unnecessary polling while ensuring fast response times.

---

## Race Condition Prevention

Every shared variable is protected by its corresponding mutex before being:

- read
- modified
- reset

Examples include:

- running flag
- last compile timestamp
- compile counters
- dongle ownership
- scheduler queue

This guarantees that all threads observe a consistent program state.

---

## Thread-safe Communication

Communication between coder threads and the monitor is achieved through:

- mutexes
- condition variables
- shared state flags

The monitor updates global simulation state while coder threads react safely after receiving condition-variable notifications.

---

# Project Architecture

```
             Monitor Thread
                    │
                    │
          checks burnout state
                    │
                    ▼
      ┌──────────────────────────┐
      │     Shared Simulation    │
      └──────────────────────────┘
            ▲              ▲
            │              │
            │              │
     Scheduler         Logger Mutex
            │
            ▼
      Heap Priority Queue
            │
            ▼
      Waiting Coders
            │
            ▼
       Shared Dongles
```

---

# Scheduler Implementation

Two scheduling algorithms are implemented.

## FIFO

Priority is determined by request arrival time.

```
priority = arrival_order
```

This provides fairness through chronological ordering.

---

## EDF

Priority is determined by:

```
deadline =
last_compile_start +
time_to_burnout
```

The earliest deadline receives the highest priority.

Both schedulers are implemented using a custom binary heap priority queue for efficient insertion and extraction.

---

# Resources

## POSIX Threads

https://man7.org/linux/man-pages/man7/pthreads.7.html

https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2

## Mutexes

https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3.html

## Condition Variables

https://man7.org/linux/man-pages/man3/pthread_cond_wait.3.html

## The Linux Programming Interface

Michael Kerrisk

## Operating Systems: Three Easy Pieces

https://pages.cs.wisc.edu/~remzi/OSTEP/

## Dining Philosophers Problem

https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2

---

# AI Usage

Artificial Intelligence was used as a learning assistant throughout the development of this project.

Specifically, AI was used for:

- understanding pthread synchronization primitives
- reviewing scheduling algorithms
- discussing deadlock and starvation prevention strategies

All generated explanations and suggestions were manually reviewed, tested, and adapted before being incorporated into the project.
