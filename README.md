# Teensy-Ray-Tracing
Ray Tracing Algorithm ran on a Teensy 4.1 Microcontroller 

## Project Description

The goal of this project is to demonstrate the capabilities of the Teensy 4.1 microcontroller by using it to run a computationally intensive ray tracing algorithm. The ray tracer renders a 3D scene consisting of spheres with different materials. The rendered image is then displayed on an Adafruit ILI9341 display.

The project includes optimizations to reduce memory usage and improve performance, such as reusing objects, reducing precision, optimizing data structures, using iterative algorithms, and limiting scene complexity.

## Project Structure

### Include Directory
Contains all the header files for the project.

### Libraries Directory
Contains all the libraries used in the project.

There are no static libaries in the project.

This directory is intended for project specific (private) libraries.
PlatformIO will compile them to static libraries and link into executable file.

The source code of each library should be placed in an own separate directory
("lib/your_library_name/[here are source files]").

### Source Directory
Contains all the source files for the project.

### Platformio.ini
Contains the configuration settings for the PlatformIO build system.

### Hardware Wiring Schematic Used
The Adafruit ILI9341 display is connected to the Teensy 4.1 using the following pinout table provided by PJRC:

picture here

Here is what my hardware setup looks like:

picture here

## How to Run

1. Connect the Teensy 4.1 to your computer.
2. Open the project in the Arduino IDE or PlatformIO.
3. Compile and upload the code to the Teensy 4.1.
4. The ray traced scene should now be displayed on the Adafruit ILI9341 display.

## Credits and Citations
Much of the code in this project is based on the book "Ray Tracing in One Weekend" by Peter Shirley, Trevor David Black, and Steve Hollasch. The book is available online at [https://raytracing.github.io/books/RayTracingInOneWeekend.html](https://raytracing.github.io/books/RayTracingInOneWeekend.html).

The book is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. The full license text is available at [https://creativecommons.org/licenses/by-nc-sa/4.0/](https://creativecommons.org/licenses/by-nc-sa/4.0/).

The following citation is provided in accordance with the license requirements:
- [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
  - **Title (series): “Ray Tracing in One Weekend Series”
  - Title (book): “Ray Tracing in One Weekend”
  - Author: Peter Shirley, Trevor David Black, Steve Hollasch
  - Version/Edition: v4.0.0-alpha.2
  - Date: 2024-04-07
  - URL (series): https://raytracing.github.io
  - URL (book): https://raytracing.github.io/books/raytracinginoneweekend.html