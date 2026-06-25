#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>

std::mutex mtx;
sem_t semaphore;

void workerFunction(int id) {
    std::cout << "Worker " << id << " started." << std::endl;

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Worker " << id << " is waiting." << std::endl;

    // Wait on the semaphore
    sem_wait(&semaphore);

    // Acquire the lock
    std::lock_guard<std::mutex> lock(mtx);

    // Do some critical section work
    std::cout << "Worker " << id << " is in the critical section." << std::endl;

    // Simulate some more work
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Release the lock
    // Unlocking the mutex automatically releases the semaphore
    std::cout << "Worker " << id << " finished." << std::endl;
}

int main() {
    const int numWorkers = 5;
    std::thread workers[numWorkers];

    // Initialize the semaphore with a count of 1
    sem_init(&semaphore, 0, 1);

    for (int i = 0; i < numWorkers; ++i) {
        workers[i] = std::thread(workerFunction, i + 1);
    }

    for (int i = 0; i < numWorkers; ++i) {
        workers[i].join();
    }

    // Destroy the semaphore
    sem_destroy(&semaphore);

    return 0;
}

