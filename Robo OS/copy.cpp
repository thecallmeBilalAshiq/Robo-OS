#include <iostream>
#include <fstream>
using namespace std;

int main() {
    std::string srcFile, destFile;
    std::ifstream srcPtr;
    std::ofstream destPtr;
    int ch;
  int process_choice  = 8, terminate_signal = 1, yet_to_read = 1; 
    char terminate = 'n';
	while(terminate != 'y'){
    std::cout << "Enter the source file name: ";
    std::cin >> srcFile;

    std::cout << "Enter the destination file name: ";
    std::cin >> destFile;

    srcPtr.open(srcFile, std::ios::binary);
    if (!srcPtr.is_open()) {
        std::cerr << "Error: cannot open source file " << srcFile << std::endl;
        
    }

    destPtr.open(destFile, std::ios::binary);
    if (!destPtr.is_open()) {
        std::cerr << "Error: cannot create destination file " << destFile << std::endl;
        
    }

    while ((ch = srcPtr.get()) != EOF) {
        destPtr.put(ch);
    }

    srcPtr.close();
    destPtr.close();

    std::cout << "File " << srcFile << " copied to " << destFile << " successfully.\n";
	
	 cout<<"want to terminate the program if not press n ?"<<endl;
	cin>>terminate;
	}
terminate = 'n';
	std::ofstream outfile("IPC.txt");
    char a = ',';
    // Write two integer values to the file
    outfile <<yet_to_read<<a<<terminate_signal<<a<<process_choice<<endl;
    // Close the file
    outfile.close();

    return 0;
}
