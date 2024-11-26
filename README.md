# *Reader-Writer Synchronization*
This project demonstrates a Reader-Writer Synchronization implementation in C using multithreading and synchronization primitives like mutexes and semaphores. The program ensures safe access to a shared resource by multiple readers and writers, adhering to synchronization constraints.

## Features
Reader Priority: Implements synchronization to prioritize readers or maintain fairness between readers and writers.

Thread-Safe Access: Ensures safe and consistent access to shared data.

Multithreading: Uses POSIX threads (pthread) for concurrent execution.

Customizable Parameters: Number of readers, writers, and access times can be easily configured.

## Technologies Used
C Programming Language

POSIX Threads (pthread)

Semaphores and Mutexes for synchronization
