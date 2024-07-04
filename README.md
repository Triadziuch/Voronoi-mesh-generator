# Voronoi diagram generation with C++ and SFML
This repository is an efficient, customizable and highly interactive implementation of [Vonoroi diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) generation algorithm C++/SFML. The application allows users to create Voronoi diagrams, configure their appearance, and interact with them in real-time by controlling one of the diagram's points. This interactivity facilitates a deeper understanding of the algorithm's functionality and behavior. Additionally, the application offers various customization options to adjust the visual aspects of the diagram, making it a versatile tool for both learning and visualization purposes.

![Voronoi mesh generation showcase](https://github.com/Triadziuch/Voronoi-mesh-generator/assets/75269577/d9316c0c-54c7-49cd-bfbf-cd2d18829c2c)

## Features
The application enables users to generate Voronoi diagrams dynamically, offering several interactive and customization features. Users can modify the color and boundaries of the cells in the diagram, providing a personalized visualization. Real-time interaction is a key feature, allowing users to move a point within the diagram and immediately observe the changes in the Voronoi cells, which helps in comprehending how the algorithm processes and updates the diagram.

## Controls
* UP - Add a point at a random position
* DOWN - Remove the most recently added point.
* Mouse wheel up / down - Add or remove points at random locations
* LMB - Add point at the mouse position
* RMB - Remove point at the mouse position
* I - Interactive mode. Allows user to control one of the points in the Voronoi diagram
* G - Toggle the visibility of the GUI
* B - Toggle the visibility of cell borders
* P - Toggle the visibility of points
* J - Toggle cell coloring
* R - Randomize the positions of points

# Dependencies
Before running this application, ensure that you have installed the Visual C++ Redistributable for Visual Studio 2015-2022. You can download it from the official Microsoft website [here](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).

# License
Copyright © 2024 by Triadziuch

This project is licensed under the MIT License - see the LICENSE file for details.


# Third-party Components
This project utilizes the SFML library, which is licensed under the zlib/png license
