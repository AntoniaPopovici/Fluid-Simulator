# 2D Water Simulation

This project is a 2D fluid dynamics simulation based on the principles outlined in the paper "Real-Time Fluid Dynamics for Games" by Jos Stam. The simulation uses OpenGL and GLUT to visualize fluid behavior in a grid, allowing for interactive manipulation via mouse input.

## Features
- Real-time simulation of fluid dynamics
- Interactive input for adding density and velocity
- Visualization of fluid density using grayscale colors

## Requirements
- OpenGL
- GLUT

## Installation
1. Clone the repository:
   ```sh
   git clone [<repository-url>](https://github.com/AntoniaPopovici/Fluid-Simulator.git)
   ```
2. Compile the code:
   ```sh
   g++ -o fluid_simulation fluid_simulation.cpp -lGL -lGLU -lglut
   ```
3. Run the executable:
   ```sh
   ./fluid_simulation
   ```

## Usage
- Click and drag the mouse within the window to add density and velocity to the fluid.
- The fluid will react in real-time, displaying the changes in density.


## Screenshots

<img width="596" alt="img2" src="https://github.com/AntoniaPopovici/Fluid-Simulator/assets/100137598/f2082088-3ff8-4533-af93-74458a7bd77e">
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
