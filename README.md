# 2D Water Simulation

This project is a 2D fluid dynamics simulation based on the principles outlined in the paper "Real-Time Fluid Dynamics for Games" by Jos Stam. The simulation uses OpenGL and GLUT to visualize fluid behavior in a grid, allowing for interactive manipulation via mouse input.

## Features
- Real-time simulation of fluid dynamics
- Interactive input for adding density and velocity
- Right-click context menu to reset the fluid simulation or exit the application
- Visualization of fluid density using grayscale colors

## Requirements
- OpenGL
- GLUT

### Building and Running

1. **Clone the repository**:
    ```sh
    git clone https://github.com/yourusername/2d-fluid-simulation.git
    cd 2d-fluid-simulation
    ```

2. **Build the project**:
    - On Linux:
        ```sh
        g++ -o fluid_simulation main.cpp -lGL -lGLU -lglut
        ```
    - On Windows:
        - Install [MinGW](http://www.mingw.org/) and ensure `g++` is available in your PATH.
        - Compile using:
            ```sh
            g++ -o fluid_simulation.exe main.cpp -lglut32 -lopengl32 -lglu32
            ```
    - On macOS:
        ```sh
        clang++ -o fluid_simulation main.cpp -framework OpenGL -framework GLUT
        ```

3. **Run the executable**:
    ```sh
    ./fluid_simulation
    ```

## Usage

- **Mouse Interaction**:
    - **Click and Drag**: Add density and velocity to the simulation at the mouse position.
    
- **Right-click Menu**:
    - **Reset Fluid**: Clears the current fluid state.
    - **Exit**: Closes the application.

## Code Structure

- **`main.cpp`**: Contains the implementation of the fluid simulation, including functions for fluid dynamics, rendering, and user interaction.

## How It Works

The simulation uses a grid to represent the fluid. It calculates the velocity and density of the fluid at each cell using the Navier-Stokes equations. The simulation steps include:
1. **Add Source**: Adds the source densities and velocities to the grid.
2. **Diffuse**: Spreads the velocities and densities across the grid.
3. **Project**: Ensures the velocity field is mass conserving.
4. **Advect**: Moves the densities and velocities through the grid based on the velocity field.

## Customization

You can tweak various parameters to change the behavior of the simulation:
- `GRID_SIZE`: Size of the grid.
- `CELL_SIZE`: Size of each cell in the grid.
- `TIME_STEP`: Simulation time step.
- `DIFFUSION`: Diffusion rate of the fluid.
- `VISCOSITY`: Viscosity of the fluid.


## Screenshots

<img width="596" alt="img2" src="https://github.com/AntoniaPopovici/Fluid-Simulator/assets/100137598/f2082088-3ff8-4533-af93-74458a7bd77e">
<img width="596" alt="img5" src="https://github.com/AntoniaPopovici/Fluid-Simulator/assets/100137598/c3c8b1a8-6bd4-44a3-9bff-059e1cbad9d8">
<img width="593" alt="img4" src="https://github.com/AntoniaPopovici/Fluid-Simulator/assets/100137598/dc7f0b3c-29b8-474f-ac98-1a4dd047061a">
<img width="593" alt="img3" src="https://github.com/AntoniaPopovici/Fluid-Simulator/assets/100137598/e2a712f6-9632-4d19-8f71-fda330a47b42">


## Recording
Due to current limitations, attempting to record the simulation may cause the program to crash in Visual Studio. Please avoid recording the simulation directly from Visual Studio.

## Inspiration
This simulation is inspired by the paper "Real-Time Fluid Dynamics for Games" by Jos Stam. You can read the paper [here](<link-to-paper>).

## Acknowledgments
- **Author**: Jos Stam
- **Title**: Real-Time Fluid Dynamics for Games
- **Link**: [Link to the paper](http://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf)
