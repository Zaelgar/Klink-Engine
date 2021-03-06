# __Klink Engine Overview__
Klink Engine is a custom 2D and 3D game engine written in C++ and DirectX 11. It's used mainly as a way for me to write new projects and learn more low-level code in an environment that I've built myself. 

*Note to any perusers: Klink Engine and the projects within are under heavy development. This engine was built during my time at LaSalle College and may be the victim of un-informed (possibly bad) code in certain sections. As I learn and grow as a programmer I will be reworking the code base to minimize those sections.*
## Math Library, JMath
- Linear algebra
- Uniform random functions
- Splines and curve functions
- Simple geometry intersection functions

![JMath Library Spline Functions Visualized](https://raw.githubusercontent.com/Zaelgar/Zaelgar/main/Splines.png)
 
## Graphics Features
- Mesh/Model importer tool built with assimp
- Animation and mesh rendering
- Procedural geometry tools
- Internal shader manager
- Shadow mapping
- Post processing

![Model Imported From Mixamo.com](https://github.com/Zaelgar/Zaelgar/blob/main/model.png)

## Artificial Intelligence
- Policy-based pathfinding system
    - Depth first search
    - Breadth-first search
    - Dijkstra search
    - A* search
- Steering behaviour
    - Seek
    - Arrive
    - Obstacle avoidance
    - Flocking
- Perception

![Simple Pathfinding Demo](https://github.com/Zaelgar/Zaelgar/blob/main/pathfinding2.gif)

## Machine Learning
- Linear Regression
- Logistic Regression
- KMeans
- Genetic algorithms

![Genetic Algorithm For Travelling Salesman](https://raw.githubusercontent.com/Zaelgar/Zaelgar/main/Genetic%20algorithm.png)

# Projects Built With Klink
## Hydraulic Erosion Simulation
One of my favourite things about game development is the amazing things people have written to create content. The world of procedural generation is so large and I wanted to get my hands dirty with it. I love procedural terrain, but a lot of noise-based implementations don't look realistic. I researched some ways to mitigate that and I learned about hydraulic erosion. I am currently working on making this a standalone project on GitHub.

### Before Erosion
![Raw Heightmap](https://raw.githubusercontent.com/Zaelgar/Zaelgar/main/BeforeErosion.png)
### After Erosion
![Heightmap After Erosion](https://raw.githubusercontent.com/Zaelgar/Zaelgar/main/AfterErosion.png)
