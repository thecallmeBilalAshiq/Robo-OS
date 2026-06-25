#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
using namespace std;
class Stopwatch {
private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;

public:
    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        endTime = std::chrono::high_resolution_clock::now();
    }

    double getDuration() {
        std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime);
        return duration.count();
    }
};

int main() {

    int process_choice = 13, terminate_signal = 1, yet_to_read = 1; 
    char terminate = 'n';
    while (terminate != 'y') {
    Stopwatch stopwatch;

    std::cout << "Press ENTER to start the stopwatch." << std::endl;
    std::cin.ignore();

    stopwatch.start();

    std::cout << "Stopwatch started. Press ENTER to stop." << std::endl;
    std::cin.ignore();

    stopwatch.stop();

    double duration = stopwatch.getDuration();
    std::cout << "Elapsed time: " << duration << " seconds." << std::endl;

    cout << "Want to terminate the program? Press y if yes, or any other key to continue." << endl;
        cin >> terminate;
    }
    
    
    std::ofstream outfile("IPC.txt");
    char a = ',';
    // Write two integer values to the file
    outfile << yet_to_read << a << terminate_signal << a << process_choice << endl;
    // Close the file
    outfile.close();
    return 0;
}

