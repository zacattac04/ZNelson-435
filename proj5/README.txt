Zachary Nelson
Project 5
CMSC 435

This program will read the input froma .in file, and create a simple boids simulation, which will be outputted in a .out file.

I based most of my code on the pseudocode found at
https://vergenet.net/~conrad/boids/pseudocode.html
which was one of the external links at the Wikipedia article for boids.
The main thing to note when running is that boids in the Godot viewer will occasionally disappear temporarily.
This is because I did not implement a rigid bounding box, but rather a force that pushes inward when a boid goes out of bounds.
I'm leaving it as is, as I felt it made for a more smooth animation.

HOW TO COMPILE:
"make" or "make boids" will work
This may leave behind a .o file. "make clean" will remove all files of this type.

HOW TO RUN:
./boids "inputFile.in" "outputFile.out"