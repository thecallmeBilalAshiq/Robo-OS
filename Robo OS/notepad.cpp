#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    int process_choice  = 7, terminate_signal = 1, yet_to_read = 1; 
    char terminate = 'n';

    string filename, line;
    cout << "Enter the filename: ";
    getline(cin, filename);

    ofstream file(filename);

    do {
        cout << "Enter text to write to file: ";
        getline(cin, line);
        file << line << endl;

        cout << "Want to continue writing to the file? (y/n)" << endl;
        cin >> terminate;
        cin.ignore(); // Ignore the newline character left by cin
    } while (terminate != 'n');

    file.close();
    cout << "File saved: " << filename << endl;

    if (terminate == 'n') {
        std::ofstream outfile("IPC.txt");
        char a = ',';
        // Write two integer values to the file
        outfile << yet_to_read << a << terminate_signal << a << process_choice << endl;
        // Close the file
        outfile.close();
    }

    return 0;
}

