Zachary Nelson
Project 3
CMSC 435



This program will read the input from a .obj file, and smooth the mesh, based on the stepsize and iteration count, and output it in another .obj file

I did not use any resources outside of the instructor and textbook when it came to the pipeline.
However, aside from the previous three projects, it has been over a year since I have done any serious project in C++, so I needed to use resources as a refresher.
This was primarily from official documentation, as well as looking over my past projects. I have found that over time, geeksforgeeks.org has been my go to for general programming help, as well as stackoverflow if there happens to be a solution to my specific problem.
I have not restricted the stepsize. However, if you set it to be more than 1 with a sufficient amount of iterations, it will likely result in very strange or "broken" results. I believe this is because it causes the points to move away from eachother, rather than towards.

HOW TO RUN:
./smoothing "inputFile.obj" "outputFile.obj" "stepsize" "niterations"
