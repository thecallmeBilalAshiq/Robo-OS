#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
using namespace std;

// Game board
char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

// Turn variable
condition_variable turn;
int current_player = 1;

// Mutex lock for board access
mutex board_lock;

// Semaphore for game end flag
int game_over = 0;
condition_variable_any game_over_cv;
mutex game_over_lock;

void print_board() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) {
                cout << " | ";
            }
        }
        cout << endl;
        if (i < 2) {
            cout << "---------" << endl;
        }
    }
}

bool check_winner(char player) {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
            return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; j++) {
        if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
            return true;
        }
    }

    // Check diagonals
    if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
        return true;
    }
    if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
        return true;
    }

    return false;
}

void play(int player) {
    while (true) {
        unique_lock<mutex> lock(board_lock);
        while (current_player != player) {
            turn.wait(lock);
        }

        // Check game over
        lock_guard<mutex> game_over_guard(game_over_lock);
        if (game_over) {
            return;
        }

        // Player move
        int row, col;
        cout << "Player " << player << ", enter your move (row col): ";
        cin >> row >> col;

        if (board[row][col] != ' ') {
            cout << "Invalid move! Try again." << endl;
            continue;
        }

        board[row][col] = (player == 1) ? 'X' : 'O';
        print_board();

        if (check_winner(board[row][col])) {
            cout << "Player " << player << " wins!" << endl;

            lock_guard<mutex> game_over_guard(game_over_lock);
            game_over = 1;
            game_over_cv.notify_all();
            return;
        }

        // Switch player
        current_player = 3 - current_player;
        turn.notify_all();
    }
}

int main() {
    thread player1(play, 1);
    thread player2(play, 2);

    player1.join();
    player2.join();

    {
        unique_lock<mutex> lock(game_over_lock);
        game_over_cv.wait(lock, []{return game_over == 1;});
    }

    return 0;
}

