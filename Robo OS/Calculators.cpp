#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

// Semaphore for number inputs
int num_sem = 0;
condition_variable num_cv;
mutex num_mutex;

// Semaphore for operation inputs
int op_sem = 0;
condition_variable op_cv;
mutex op_mutex;

// Result variable
int result = 0;

// Mutex lock for result access
mutex result_mutex;

// Calculator thread
void calculator_thread() {
    while (true) {
        // Wait for number input
        unique_lock<mutex> num_lock(num_mutex);
        num_cv.wait(num_lock, []{ return num_sem > 0; });
        num_sem--;

        // Get the number
        int number;
        cin >> number;

        // Release operation semaphore
        {
            lock_guard<mutex> op_lock(op_mutex);
            op_sem++;
        }
        op_cv.notify_one();

        // Update the result
        lock_guard<mutex> result_lock(result_mutex);
        result += number;
    }
}

int main() {
    // Start the calculator thread
    thread calc_thread(calculator_thread);

    while (true) {
        // Wait for operation input
        unique_lock<mutex> op_lock(op_mutex);
        op_cv.wait(op_lock, []{ return op_sem > 0; });
        op_sem--;

        // Get the operation
        char operation;
        cin >> operation;

        // Release number semaphore
        {
            lock_guard<mutex> num_lock(num_mutex);
            num_sem++;
        }
        num_cv.notify_one();

        // Perform the operation
        int number;
        {
            lock_guard<mutex> result_lock(result_mutex);
            number = result;
        }

        // Check for the exit condition
        if (operation == 'q' || operation == 'Q') {
            break;
        }

        // Perform the operation
        switch (operation) {
            case '+':
                result = number + result;
                break;
            case '-':
                result = number - result;
                break;
            case '*':
                result = number * result;
                break;
            case '/':
                if (result != 0) {
                    result = number / result;
                } else {
                    cout << "Error: Division by zero!" << endl;
                }
                break;
            default:
                cout << "Error: Invalid operation!" << endl;
                break;
        }
    }

    // Wait for the calculator thread to finish
    calc_thread.join();

    // Print the final result
    cout << "Result: " << result << endl;

    return 0;
}

