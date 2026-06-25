#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>

int main() {
    while (true) {
        // Get the current time
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);

        // Convert the time to a string
        char* timeStr = std::ctime(&time);

        // Clear the console and move the cursor to the top left corner
        std::cout << "\033[2J\033[1;1H";

        // Print the current time
        std::cout << "Current time: " << timeStr;

        // Wait for one second before updating the time again
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}



