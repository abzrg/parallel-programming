# POSIX Threads

## Order

- `hello.c`: simple program to execute a simple routine on multiple threads
- `race_cond{0,1}.c`: what is race condition and how to prevent it from happening using mutexes
- `roll_dice.c`: return values from threads
- `printing_primes.c`: printing 10 primes using 10 threads
- `array_sum.c`: compute sum of elements in an array using multiple threads


## Threads vs Processes

A comparison between **pthreads (POSIX threads)** and **Unix processes**:

| Aspect                | Pthreads (POSIX Threads)                   | Unix Processes                       |
|-----------------------|-------------------------------------------|--------------------------------------|
| **Definition**        | Lightweight execution units within a single process. | Independent instances of programs.   |
| **Memory Sharing**    | Share the same address space (memory).     | Have separate address spaces.        |
| **Creation Time**     | Faster to create.                          | Slower to create due to more overhead.|
| **Context Switching** | Faster due to less overhead (no change in memory space). | Slower due to the need for memory space switching. |
| **Communication**     | Easier and faster (shared memory).         | More complex (Inter-Process Communication - IPC mechanisms like pipes, sockets, etc.). |
| **Resources**         | Share most resources (files, memory, etc.). | Have their own independent resources.|
| **Crash Impact**      | If one thread crashes, the whole process might crash. | If one process crashes, others remain unaffected. |
| **Concurrency**       | Used for achieving concurrency within a single process. | Achieve parallelism across different processes.  |
| **System Calls**      | Uses thread-specific system calls (e.g., `pthread_create`). | Uses process-specific system calls (e.g., `fork`). |
| **Synchronization**   | Needs explicit synchronization (mutexes, semaphores) due to shared memory. | Less synchronization required between processes; often use IPC for communication. |
| **Security**          | Less secure due to shared address space.   | More secure due to isolated memory spaces.       |
| **Performance**       | Generally, higher performance for short tasks due to shared resources. | Can be slower for communication and context switching.|
| **Termination**       | Threads can be individually terminated; one thread can terminate all threads if it terminates the process. | Processes terminate independently unless a parent or related process terminates them. |

Key Differences:

1. **Memory and Resource Sharing**: Threads share memory and resources within the same process, making inter-thread communication faster but also riskier if synchronization is not handled properly. Processes, on the other hand, have isolated memory spaces, which makes them safer but also more complex to manage for communication.

2. **Performance and Overhead**: Threads are lightweight, with lower overhead, which makes them ideal for tasks that require frequent communication and quick context switching. Processes, being more heavyweight, are more suited to tasks that need independent execution with less frequent interaction.

3. **Use Cases**: Threads are often used in applications where tasks need to run concurrently within the same application (e.g., web servers, GUI applications). Processes are more suitable for running separate applications or tasks that require strong isolation (e.g., running different services on a server).


## Race condition

### What is a Race Condition?

A **race condition** occurs when two or more threads or processes access shared data concurrently, and the final outcome depends on the order or timing of their execution. This can lead to unpredictable or incorrect behavior, especially when at least one of the threads is modifying the shared data.

#### Example of a Race Condition

Let's consider a simple example:

- Suppose you have a global variable `counter` initialized to `0`.
- Two threads (`Thread A` and `Thread B`) both increment this `counter` by `1`:

```c
// Shared global variable
int counter = 0;

void* increment(void* arg) {
    counter++; // Increment the counter
    return NULL;
}
```

- When both threads execute `counter++`, the expected result is for the `counter` to be incremented twice, resulting in `counter = 2`.

However, due to the nature of concurrent execution, the actual outcome may vary because of the following steps involved in the increment operation:

1. **Read** the value of `counter`.
2. **Increment** the value by `1`.
3. **Write** the new value back to `counter`.

If both threads read the value of `counter` at the same time before either has written back the incremented value, they both see `counter` as `0`:

- **Thread A** reads `counter = 0`.
- **Thread B** reads `counter = 0`.
- **Thread A** increments and writes back `counter = 1`.
- **Thread B** increments and writes back `counter = 1`.

As a result, `counter` is `1` instead of the expected `2`, which is a race condition.

### Mutex: A Solution to Race Conditions

A **mutex** (short for "mutual exclusion") is a synchronization mechanism used to prevent race conditions by ensuring that only one thread can access a critical section of code at a time.

#### How a Mutex Works

A mutex provides two main operations:

1. **Lock (acquire)**: A thread locks a mutex before entering the critical section, ensuring exclusive access to the shared resource.
2. **Unlock (release)**: The thread unlocks the mutex after leaving the critical section, allowing other threads to access the shared resource.

If a thread attempts to lock a mutex that is already locked by another thread, it will block (wait) until the mutex is unlocked.

#### Using a Mutex to Solve Race Conditions

Here is how you can use a mutex to prevent the race condition in the above example:

```c
#include <pthread.h>
#include <stdio.h>

int counter = 0;             // Shared global variable
pthread_mutex_t lock;        // Mutex to protect the counter

void* increment(void* arg) {
    pthread_mutex_lock(&lock);    // Lock the mutex before entering the critical section
    counter++;                    // Critical section: safely increment the counter
    pthread_mutex_unlock(&lock);  // Unlock the mutex after leaving the critical section
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&lock, NULL);  // Initialize the mutex

    // Create two threads
    pthread_create(&thread1, NULL, increment, NULL);
    pthread_create(&thread2, NULL, increment, NULL);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock);  // Destroy the mutex

    printf("Final counter value: %d\n", counter);  // Should be 2

    return 0;
}
```

#### Explanation

- **`pthread_mutex_t lock;`**: Declares a mutex named `lock` to control access to the shared variable `counter`.
- **`pthread_mutex_init(&lock, NULL);`**: Initializes the mutex before it is used.
- **`pthread_mutex_lock(&lock);`**: Locks the mutex before entering the critical section where `counter` is incremented. If another thread has already locked the mutex, this thread will wait until the mutex is unlocked.
- **`pthread_mutex_unlock(&lock);`**: Unlocks the mutex after the critical section is completed, allowing other threads to access the critical section.
- **`pthread_mutex_destroy(&lock);`**: Cleans up and releases resources associated with the mutex after all threads are done.

### Why Mutexes Work

Mutexes work because they enforce *mutual exclusion*, meaning:

- Only one thread can enter the critical section (where the shared resource is accessed or modified) at any time.
- All other threads trying to enter the critical section will wait until the mutex is unlocked.

By serializing access to the critical section, a mutex prevents race conditions and ensures that the shared data is accessed safely and predictably.

### Summary

- A **race condition** occurs when multiple threads or processes access shared data concurrently, and the outcome depends on the timing of their execution.
- A **mutex** is a synchronization tool that provides mutually exclusive access to a critical section, preventing race conditions by ensuring that only one thread can execute the critical section at a time.

### A closer look

To understand how a **race condition** happens during a concurrent increment operation, let's break down the disassembled machine code instructions that are executed when multiple threads perform an increment operation on a shared variable. We'll see how the different steps of the increment can interleave between threads, leading to incorrect results.

### Disassembly of the Increment Operation

Let's consider a simple C function that increments a shared variable `counter`:

```c
void increment() {
    counter++;
}
```

#### Assembly Instructions for `counter++`

The `counter++` operation involves the following steps at the machine code level:

1. **Load** the value of `counter` into a register (read from memory).
2. **Increment** the value in the register.
3. **Store** the incremented value back to `counter` (write to memory).

In x86 assembly language, this might look like:

```assembly
mov eax, [counter]  ; Step 1: Load counter into the register EAX
inc eax             ; Step 2: Increment the value in EAX
mov [counter], eax  ; Step 3: Store the incremented value back to counter
```

#### Understanding the Race Condition

Let's assume we have two threads, **Thread A** and **Thread B**, both executing this `increment()` function concurrently without any synchronization (like a mutex). 

Here's what happens step-by-step:

1. **Thread A** starts executing:
   - `mov eax, [counter]` — Loads `counter` (say, `0`) into `EAX`.
   
2. **Thread B** starts executing before Thread A finishes:
   - `mov eax, [counter]` — Also loads `counter` (still `0`) into `EAX`.

3. **Thread A** continues:
   - `inc eax` — Increments `EAX` to `1`.

4. **Thread B** continues:
   - `inc eax` — Increments its own `EAX` (also `0`) to `1`.

5. **Thread A** resumes:
   - `mov [counter], eax` — Stores its `EAX` value (`1`) back to `counter`.

6. **Thread B** resumes:
   - `mov [counter], eax` — Stores its `EAX` value (`1`) back to `counter`.

At the end of this sequence, `counter` is `1`, even though both threads have incremented it. The final value should have been `2`, but due to the interleaving of instructions, both threads ended up writing the same value (`1`) to `counter`.

### Visualizing the Race Condition

Let’s visualize the interleaving of these instructions to show how the race condition occurs:

| Time | **Thread A**         | **Thread B**         | **Shared Variable `counter`** |
|------|----------------------|----------------------|-------------------------------|
| t1   | `mov eax, [counter]` |                      | `counter = 0`                 |
| t2   |                      | `mov eax, [counter]` | `counter = 0`                 |
| t3   | `inc eax`            |                      | `counter = 0`                 |
| t4   |                      | `inc eax`            | `counter = 0`                 |
| t5   | `mov [counter], eax` |                      | `counter = 1`                 |
| t6   |                      | `mov [counter], eax` | `counter = 1`                 |

Here, both threads read the value of `counter` as `0` (at `t1` and `t2`), incremented their local registers (`eax`), and then both wrote back the same result (`1`) to `counter`.

### Preventing the Race Condition with a Mutex

Now, let's see how a **mutex** solves this problem by ensuring mutual exclusion, i.e., only one thread can access the critical section (increment operation) at a time.

#### Modified Function with Mutex Lock

Here's the increment function with mutex protection:

```c
pthread_mutex_t lock;

void increment() {
    pthread_mutex_lock(&lock);   // Lock the mutex before entering the critical section
    counter++;                   // Critical section: safely increment the counter
    pthread_mutex_unlock(&lock); // Unlock the mutex after leaving the critical section
}
```

#### Assembly with Mutex Lock

The mutex lock adds additional instructions that control access to the critical section. Here's what happens step-by-step:

1. **Thread A** locks the mutex.
2. **Thread A** performs the `mov`, `inc`, and `mov` instructions safely.
3. **Thread A** unlocks the mutex.
4. **Thread B** attempts to lock the mutex while Thread A holds it and waits.
5. After Thread A releases the mutex, **Thread B** acquires it and safely executes the increment operation.

### Visualizing Mutex Lock Protection

| Time | **Thread A**                        | **Thread B**               | **Shared Variable `counter`** |
|------|-------------------------------------|-----------------------------|-------------------------------|
| t1   | `pthread_mutex_lock(&lock)`         |                             | `counter = 0`                 |
| t2   | `mov eax, [counter]`                |                             | `counter = 0`                 |
| t3   | `inc eax`                           |                             | `counter = 0`                 |
| t4   | `mov [counter], eax`                |                             | `counter = 1`                 |
| t5   | `pthread_mutex_unlock(&lock)`       |                             | `counter = 1`                 |
| t6   |                                     | `pthread_mutex_lock(&lock)` | `counter = 1`                 |
| t7   |                                     | `mov eax, [counter]`        | `counter = 1`                 |
| t8   |                                     | `inc eax`                   | `counter = 1`                 |
| t9   |                                     | `mov [counter], eax`        | `counter = 2`                 |
| t10  |                                     | `pthread_mutex_unlock(&lock)`| `counter = 2`                 |

#### Explanation

- **At `t1`**, Thread A locks the mutex, ensuring exclusive access to the `counter`.
- **At `t6`**, Thread B attempts to lock the mutex but is blocked until Thread A unlocks it at `t5`.
- Each thread safely completes the increment operation in isolation, resulting in `counter = 2`.

### Summary

- A **race condition** occurs when multiple threads perform operations on shared data without proper synchronization, leading to unpredictable results.
- The **increment operation** involves multiple steps (load, increment, store) that can interleave between threads, causing a race condition.
- A **mutex** ensures that only one thread can execute the critical section at a time, preventing race conditions by enforcing mutual exclusion.
