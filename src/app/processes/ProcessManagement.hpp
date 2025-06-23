#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include <queue>
#include <atomic>
#include <mutex>
#include <semaphore.h>
#include <memory>
#include "Task.hpp"

using namespace std;

class ProcessManagement{
    private:
    // ==== [Version 1: Sequential] ==== //
    /*
        // STL Queue DS to store the Task in FIFO manner
        queue<unique_ptr<Task>> taskQueue;
    */
    // ==== ======================= ==== // 


    // ==== [Version 2,3: Multiprocessing / Multithreading] ==== //
    /**/
        // Counts available tasks (consumer waits on this)
        sem_t* itemsSemaphore;
        // Counts free slots in queue (producer waits on this)
        sem_t* emptySlotsSemaphore;

        // This structure is shared between processes/threads to coordinate task management using a bounded circular queue.
        struct SharedMemory{
            // Impl of the bounded circular queue
            atomic<int> size;
            char tasks[1000][256];
            int front;
            int rear;

            void printSharedMemory() {
                cout << size << '\n';
                cout << front << '\n';
                cout << rear << '\n';
            }
        };

        // Pointer to shared memory region
        SharedMemory* sharedMem;

        int shmFd;
        const char* SHM_NAME = "/my_queue";

        std::mutex queueLock;
    /**/
    // ==== ============================================== ==== //
        
    public:
        ProcessManagement();
        ~ProcessManagement();

        bool submitToQueue(unique_ptr<Task> task);
        void executeTask();
};

#endif