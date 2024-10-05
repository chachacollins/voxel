build:
	  gcc *.c `sdl2-config --libs --cflags` -lGLEW -lGL -lpthread -lm -o voxel

run: 
	./voxel
