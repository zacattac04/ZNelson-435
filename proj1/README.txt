Zachary Nelson
Project 1
CMSC 435

****************************************************************************
NOTE

This is me attempting to do the extra credit. The working copy is in the main branch.
****************************************************************************

This program will read the information from an NFF file, and use ray tracing to create a PPM file.
It is only set up to trace triangles, and does not care which is closer.
It is also not set up to differentiate colors between surfaces.
However, I have hopefully created it in such a way that it will not be difficult to modify to allow it to do these in the future.

I did not use any resources outside of the instructor and textbook when it came to ray tracing. 
However, it has been over a year since I have done any serious project in C++, so I needed to use resources as a refresher.
This was primarily from official documentation, as well as looking over my past projects.

HOW TO RUN:
./RayTrace "inputFile.nff" "Outputfile.ppm"
If an output file is not given, it will default to "hide.ppm"
If an input file is not given it will throw a runtime error
