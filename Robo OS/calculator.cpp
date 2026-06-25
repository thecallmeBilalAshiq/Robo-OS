#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <signal.h>
using namespace std;
 struct interrupt { // for IPC between processes and parent process for iterruption, minimization and closing of a process
int minimize=-1, close=-1, in_out_put=-1;
int process_choice= -1;
};
void close_terminal( ){
	std::system("pkill -f gnome-terminal ");
}
int main() {
/*nt shmid1 = shmget((key_t)200, sizeof(interrupt), 0666); // for interuupt, termination
interrupt * object = (interrupt*)shmat(shmid1,NULL,0);
//sleep(2);
interrupt msg;*/
int value = -1;
int process_choice  =5 , terminate_signal = 1, yet_to_read = 1; 
    char terminate = 'n';
    char op;
    float num1, num2;
	while(terminate== 'n'){
		cout<<"enter y to terminate the process"<<endl;
    cout << "Enter operator (+, -, *, /): ";
    cin >> op;
    cout << "Enter two numbers: ";
    cin >> num1 >> num2;
	//////////////////////////////
     interrupt msg1;    
    switch(op) {
        case '+':
            cout << num1 << " + " << num2 << " = " << num1 + num2;
            break;

        case '-':
            cout << num1 << " - " << num2 << " = " << num1 - num2;
            break;

        case '*':
            cout << num1 << " * " << num2 << " = " << num1 * num2;
            break;

        case '/':
            if (num2 == 0) {
                cout << "Error: division by zero.";
            } else {
                cout << num1 << " / " << num2 << " = " << num1 / num2;
            }
            break;

        default:
            cout << "Error: invalid operator.";
            break;
    }
	
	std::ifstream in("shutdown.txt");
			     in >>value;
                    in.close();
					if(value>=0) {  
					// incrementing the value before terminating
					value++;
					std::ofstream o("shutdown.txt");
                     o <<value<<endl;
                     o.close();
					close_terminal();}
						
	cout<<"want to terminate the program?"<<endl;
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
