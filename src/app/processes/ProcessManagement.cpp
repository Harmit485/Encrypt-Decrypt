#include <iostream>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <thread>
#include <atomic>
#include <semaphore.h>
#include "ProcessManagement.hpp"
#include "../encryptDecrypt/Cryption.hpp"

using namespace std;

ProcessManagement::ProcessManagement(){
    // ==== [Version 2,3: Multiprocessing / Multithreading] ==== //
    /**/
        // counts how many items are available (starts at 0).
        sem_t* itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0); 
        // counts how many slots are free (starts at 1000).
        sem_t* emptySlotsSemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);

        if (itemsSemaphore == SEM_FAILED || emptySlotsSemaphore == SEM_FAILED) {
            perror("sem_open failed");
            exit(EXIT_FAILURE);
        }

        // creates a shared memory file.
        shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

        if (shmFd == -1) {
            perror("shm_open failed");
            exit(EXIT_FAILURE);
        }

        // sets its size to fit SharedMemory structure.
        ftruncate(shmFd, sizeof(SharedMemory));

        // maps the shared memory so all processes can access it.
        sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
        
        // Sets front and rear pointers to 0 & Initializes size to 0 (using atomic store).
        sharedMem->front = 0;
        sharedMem->rear = 0;
        sharedMem->size.store(0);
    /**/
    // ==== ============================================== ==== //
}

ProcessManagement::~ProcessManagement(){
    // ==== [Version 2: Multiprocessing] ==== // 
    /*
        munmap(sharedMem, sizeof(SharedMemory));
        shm_unlink(SHM_NAME);
    */
    // ==== ============================ ==== //


    // ==== [Version 3: Multithreading] ==== // 
    /**/
        munmap(sharedMem, sizeof(SharedMemory));
        shm_unlink(SHM_NAME);

        sem_close(itemsSemaphore);
        sem_close(emptySlotsSemaphore);
        sem_unlink("/items_semaphore");
        sem_unlink("/empty_slots_semaphore");
    /**/
    // ==== =========================== ==== //
}

bool ProcessManagement::submitToQueue(unique_ptr<Task> task){
    // ==== [Version 1: Sequential] ==== //
    /*
        taskQueue.push(move(task));
        executeTask();
    */
    // ==== ======================= ==== // 


    // ==== [Version 2,3: Multiprocessing / Multithreading] ==== //
    /**/
        // Waits until there’s room in the shared memory queue.
        sem_wait(emptySlotsSemaphore);
        std::unique_lock<std::mutex> lock(queueLock);

        // Push the current task into Queue

            // Checks if the queue is full.
            if (sharedMem->size.load() >= 1000) {
                return false;
            }

            // Converts task to string and adds it at rear of queue.
            strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());

            // Moves rear forward (circular queue).
            sharedMem->rear = (sharedMem->rear + 1) % 1000;

            // Increases task count.        
            sharedMem->size.fetch_add(1);

        lock.unlock();
        sem_post(itemsSemaphore);
    /**/
    // ==== ============================================== ==== //


    // ==== [Version 2: Multiprocessing] ==== // 
    /*
        // fork() creates a new process.
        // Child process runs executeTask() and exits.

        int pid = fork();

        if(pid < 0){
            return false;
        }
        else if(pid == 0){
            executeTask();
            exit(0);
        }
    */
    // ==== ============================ ==== //
    

    // ==== [Version 3: Multithreading] ==== // 
    /**/
        // Creates a new thread (thread_1) that runs the executeTask() member function on the current object (this).
        std::thread thread_1(&ProcessManagement::executeTask, this);
        thread_1.detach();
    /**/
    // ==== =========================== ==== //

    return true;
}

void ProcessManagement::executeTask(){
    // ==== [Version 1: Sequential] ==== //
    /*
        while(!taskQueue.empty()){
            unique_ptr<Task> curTask = move(taskQueue.front());
            taskQueue.pop();

            // Do Enc / Dec 
            executeCryption(curTask->toString());
        }
    */
    // ==== ======================= ==== // 


    // ==== [Version 2,3: Multiprocessing / Multithreading] ==== //
    /**/
        // Waits until there’s at least one task in shared memory.
        sem_wait(itemsSemaphore);
        // Locks the queue using std::mutex.
        std::unique_lock<std::mutex> lock(queueLock);

            // Copies the task from the front.
            char taskStr[256];
            strcpy(taskStr, sharedMem->tasks[sharedMem->front]);

            // Moves front ahead (circularly).
            sharedMem->front = (sharedMem->front + 1) % 1000;
            // Decreases the task count.
            sharedMem->size.fetch_sub(1);

        lock.unlock();
        sem_post(emptySlotsSemaphore);

        executeCryption(taskStr);
    /**/
    // ==== ============================================== ==== //

    return;
}