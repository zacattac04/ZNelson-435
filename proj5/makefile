CC          = c++ 

#-----------------------------------------
#Optimization ----------------------------
OPT   = -O3 -Wno-deprecated

#GL_LIB = -framework OpenGL -framework GLUT -framework foundation
GL_LIB = -lGL -lglut -lGLU

#-----------------------------------------

TARGETS = fishtank viewer

OBJECTS = kdTree.o

#-----------------------------------------

LIBS = 
INCS = -I C:\Users\zacha\Documents\eigen-3.4.0\

CCOPTS = $(OPT) $(INCS) 
LDOPTS = $(OPT) $(INCS) $(LIBS) 

#-----------------------------------------
#-----------------------------------------

default: $(TARGETS)


clean:
	/bin/rm -f *.o $(TARGETS)

#-----------------------------------------
#-----------------------------------------


.cpp.o: 
	$(CC) $(CCOPTS) -c $< 

#-----------------------------------------
#-----------------------------------------


viewer: $(OBJECTS) viewer.o
	$(CC) $(OBJECTS) viewer.o $(LDOPTS) $(GL_LIB) -o viewer

fishtank: $(OBJECTS) fishtank.o
	$(CC) $(OBJECTS) fishtank.o $(LDOPTS) -o fishtank

#-----------------------------------------
#-----------------------------------------















