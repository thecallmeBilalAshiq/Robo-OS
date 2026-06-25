#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <cstring>
#include<fstream>
using namespace std;
 void close_terminal( ){
	std::system("pkill -f gnome-terminal ");
}
int main() {
	int  value  =-1;
int process_choice  = 6, terminate_signal = 1, yet_to_read = 1; 
    char terminate = 'n';
    srand(time(NULL));

    std::cout << "Welcome to the Mini Game!\n";
    std::cout << "=========================\n";

    int target = rand() % 100 + 1;
    int guess;
    int attempts = 0;
    cout<<"enter y to terminate the process"<<endl;
    while ( terminate != 'y') {
        std::cout << "Guess the number between 1 and 100: ";
        std::cin >> guess;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        attempts++;

        if (guess < target) {
            std::cout << "Too low. Try again.\n";
        } else if (guess > target) {
            std::cout << "Too high. Try again.\n";
        } else {
            std::cout << "Congratulations! You guessed the number in " << attempts << " attempts.\n";
            break;
        }
		 cout<<"enter y to terminate the process"<<endl;
		 cin>>terminate;
    }
	std::ofstream outfile("IPC.txt");
    char a = ',';
    // Write two integer values to the file
    outfile <<yet_to_read<<a<<terminate_signal<<a<<process_choice<<endl;
    // Close the file
    outfile.close();
	//std::system("pkill -f gnome-terminal ");
    return 0;
}

