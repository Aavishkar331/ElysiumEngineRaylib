# ElysiumEngine

A lightweight 2D physics engine and game framework built with C++20 and Raylib.

## Features

- **Physics Engine**: Supports Rigid Body dynamics, AABB collision detection, and Sphere/Box colliders.
- **Component System**: Simple GameObject-Component architecture for easy extensibility.
- **Raylib Integration**: Built-in renderer using Raylib 5.5 for high-performance 2D graphics, windowing, and input.

## Project Structure

- `app/`: Application entry point (`main.cpp`).
- `engine/`: Core engine logic.
    - `core/`: GameObject, Scene, and Component systems.
    - `physics/`: Rigid body dynamics, colliders, and collision detection.

## Prerequisites

- **C++20 Compiler**: GCC 10+, Clang 10+, or MSVC 2019+.
- **CMake**: Version 3.15 or higher.
- **Linux dependencies**: X11 and OpenGL dev packages (`libxrandr-dev libxcursor-dev libxi-dev libxinerama-dev libgl1-mesa-dev xorg-dev`).
- Raylib is fetched automatically via CMake FetchContent — no manual install needed.

## Building the Project

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/ElysiumEngine.git
   cd ElysiumEngine
   ```

2. **Configure and Build**:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

3. **Run the Application**:
   ```bash
   ./ElysiumApp
   ```

## Controls

- **WASD**: Move the player
- **Space**: Jump
- **Left Click**: Spawn a new ball at cursor
- **G**: Toggle debug grid
- **Escape**: Quit

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
