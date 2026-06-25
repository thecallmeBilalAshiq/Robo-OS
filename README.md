# Robo OS

Robo OS is a small C++ desktop-style operating system simulator built around a graphical launcher and a collection of utility programs. The main application presents a custom SFML interface that can open tools such as a calculator, calendar, file utilities, notepad, music/video players, a system monitor, games, and timer/watch features.

## Highlights

- Graphical launcher built with SFML
- Multiple standalone utilities written in C++
- Linux/POSIX-based process and IPC handling
- Bundled assets for the UI and media features
- Lightweight project that can be compiled tool by tool

## Project Structure

The `Robo OS/` folder contains the main launcher, the utility programs, and the assets they use.

### Main launcher

- `final.cpp` - the primary SFML desktop shell that opens and coordinates the app menu

### Utilities

- `calculator.cpp` - calculator
- `calender.cpp` - calendar utility
- `copy.cpp` - file copy tool
- `compress.cpp` - file compression tool
- `delete.cpp` - file deletion tool
- `file.cpp` - file manager / file-related tool
- `music.cpp` - music player
- `notepad.cpp` - simple text editor
- `randomnumber.cpp` - number guessing game
- `system_monitor.cpp` - process viewer and system monitoring tool
- `tictac.cpp` / `tictactoe.cpp` - Tic Tac Toe game code
- `timer.cpp` - timer utility
- `watch.cpp` - stopwatch/watch utility
- `video.cpp` - video player
- `hungman.cpp` - Hangman game
- `semwork.cpp`, `worker.cpp`, `Calculators.cpp` - supporting/demo process files

### Assets and runtime files

- `Banana.ttf` - UI font used by the launcher
- `woods.jpg` - background image for the main window
- `song.mp3`, `piano.mp3`, `sunset.mp4` - bundled media examples
- `IPC.txt` - simple IPC/status file used by several tools

## Requirements

- Linux
- `g++` with C++17 support
- SFML development libraries
- Standard POSIX/Linux headers and tools used by the process-control features

On Debian/Ubuntu, the SFML packages are typically:

```bash
sudo apt install g++ libsfml-dev
```

## Build and Run

The launcher and the utilities are separate executables. Build the one you want from inside the `Robo OS/` directory.

### Main launcher

```bash
g++ -std=c++17 final.cpp -o robo-os \
	-lsfml-graphics -lsfml-window -lsfml-system
./robo-os
```

### Example: standalone utility

```bash
g++ -std=c++17 calculator.cpp -o calculator
./calculator
```

If a tool uses SFML or additional system libraries, add the required linker flags for that file as needed.

## How It Works

Robo OS is designed as a simple multi-process simulation. The main launcher opens a window with clickable tools, while some utilities communicate through shared files or Linux IPC mechanisms. Several programs write to `IPC.txt` so the launcher can coordinate their state.

## Notes

- Some filenames intentionally keep the original project spelling, such as `calender` and `hungman`.
- The project was built for Linux desktop environments and may need adjustments on Windows or macOS.
- Media and font files must stay alongside the executables so the launcher can load them correctly.

## Suggested Next Step

If you want, I can also make this README more polished by adding badges, screenshots, and a clean table of contents, or I can help standardize the build commands for every individual `.cpp` file.
