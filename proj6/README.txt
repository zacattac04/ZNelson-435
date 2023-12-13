Zachary Nelson
Project 6
CMSC 435

This program will take in either a .png or .jpg, and use seam carving to reduce the size of the image, and output as either a .png or .jpg. 
Other input types may work, but it can only output as jpg or png.

I had a lot of help from the class discord, as well as of course getting information from the paper provided. 
As this was a fairly specific topic, there wasn't a whole lot of information easily available. 
I did ask chatGPT for help a few times, but the only part that made it in was when I asked it about transposing an array representing a matrix. 
the exact question was "Suppose I had an array of length(height*width) representing a matrix. Show me a C++ function to transpose this matrix"
I had also asked about convolutions, as I was planning to use the sobel operators, but this didn't pan out.

One thing to note is that if you already have a file matching the name of the output in the folder,
it won't overwrite that file. You must first delete the file if you want the new output.

There is a "trash" folder with a few files in it that aren't important to running the program.
There is also a "pics" folder with the input images that I tested. However, these won't show up on Github due to the
.gitignore specifications.

HOW TO COMPILE:
"make" or "make seamCarve" will work
This may leave behind a .o file. "make clean" will remove all files of this type.

HOW TO RUN:
./seamCarve "image" "imageOutput" "outputWidth" "outputHeight"