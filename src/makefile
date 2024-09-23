#OBJS specifies which files to compile as part of the project
OBJS = main.c mapping/map.c player/player.c render/render.c maths.c

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = efst

#This is the target that compiles our executable
all : $(OBJS)
	gcc $(OBJS) -w -lSDL2 -lm -o $(OBJ_NAME)