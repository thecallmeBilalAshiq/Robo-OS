#include <iostream>
#include <fstream>
#include <string>
#include<unistd.h>
using namespace std;

int main() {
    int process_choice  = 3, terminate_signal = 1, yet_to_read = 1; 
	
              
        std::ofstream outfile("IPC.txt");
        char a = ',';
        // Write two integer values to the file
        outfile << yet_to_read << a << terminate_signal << a << process_choice << endl;
        // Close the file
        outfile.close();
		     const char* args[] = {"gnome-terminal", "--", "bash", "-c", "gst-play-1.0 sunset.mp4; exec bash", NULL};
                    execvp(args[0], const_cast<char**>(args));
    

    return 0;
}
