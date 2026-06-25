#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void workerFunction(int id) {
    std::cout << "Worker " << id << " started." << std::endl;

    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Worker " << id << " is waiting." << std::endl;

    // Acquire the lock
    std::unique_lock<std::mutex> lock(mtx);

    // Wait until ready signal is received
    cv.wait(lock, [] { return ready; });

    // Do some critical section work
    std::cout << "Worker " << id << " is in the critical section." << std::endl;

    // Release the lock
    lock.unlock();

    // Simulate some more work
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "Worker " << id << " finished." << std::endl;
}

int main() {
    const int numWorkers = 5;
    std::thread workers[numWorkers];

    for (int i = 0; i < numWorkers; ++i) {
        workers[i] = std::thread(workerFunction, i + 1);
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Acquire the lock
    std::unique_lock<std::mutex> lock(mtx);

    // Set ready signal to true
    ready = true;

    // Notify all waiting threads
    cv.notify_all();

    // Release the lock
    lock.unlock();

    for (int i = 0; i < numWorkers; ++i) {
        workers[i].join();
    }

    return 0;
}

