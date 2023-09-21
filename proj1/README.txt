Zachary Nelson
Project 1
CMSC 435



This program will read the information from an NFF file, and use ray tracing to create a PPM file.

I did not use any resources outside of the instructor and textbook when it came to ray tracing. 
However, it has been over a year since I have done any serious project in C++, so I needed to use resources as a refresher.
This was primarily from official documentation, as well as looking over my past projects.

HOW TO RUN:
./RayTrace "inputFile.nff" "outputFile.ppm"
If an output file is not given or has an invalid extension, it will default to "hide.ppm".
If an input file is not given or has an invalid extension, it will throw a runtime error.

NOTE:
I have a progress bar to set up to make it clear that the program is working and not stalling. However, when I moved the code to the gl server to test functionality of *nix systems, it didn't print properly.