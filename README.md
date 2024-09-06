demo:


https://github.com/user-attachments/assets/227f63be-f905-40fa-988f-6e0f1d61a9bf

# OpenGL Raycasting Project

A simple raycasting engine implemented using OpenGL.

## Prerequisites

- C compiler (GCC recommended)
- OpenGL libraries
- GLUT (OpenGL Utility Toolkit)

## Installation

### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential freeglut3-dev libglew-dev
```

### macOS:
```bash
brew install freeglut
```

### Windows:
1. Install MinGW-w64 for GCC compiler
2. Download and install freeglut

## Cloning and Compiling

```bash
git clone https://github.com/kussshhhh/raycaster.git
cd raycaster
gcc -o raycaster main.c -lGL -lGLU -lglut -lm
```

## Running the Project

```bash
./raycaster
```

## Controls

- W: Move forward
- S: Move backward
- A: Rotate left
- D: Rotate right

## Troubleshooting

- Ensure all required libraries are installed
- Some systems may require `-lOpenGL` instead of `-lGL`
- Update your graphics drivers if issues persist

For more help, open an issue on the GitHub repository.
