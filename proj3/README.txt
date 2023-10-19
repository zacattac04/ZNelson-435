Zachary Nelson
Project 3
CMSC 435



This program will read the information from an NFF file, and use a pipeline to create a PPM file. It will take into account specular and diffuse components, and will be able to show basic shading.

I did not use any resources outside of the instructor and textbook when it came to the pipeline.
However, aside from the previous two projects, it has been over a year since I have done any serious project in C++, so I needed to use resources as a refresher.
This was primarily from official documentation, as well as looking over my past projects.

HOW TO RUN:
./RayTrace "inputFile.nff" "outputFile.ppm"
If an output file is not given or has an invalid extension, it will default to "hide.ppm".
If an input file is not given or has an invalid extension, it will throw a runtime error.