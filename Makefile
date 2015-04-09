all: max comparison calculation drt

calculation:
	g++ ./src/calculation/MyClass.cpp ./src/calculation/main.cpp -lncurses -lform -o ./calculate -lntl

max:
	g++ ./src/max/*.cpp -o ./max -lmpfr

comparison:
	g++ ./src/comparison/*.cpp -o ./comparison -lglut -lGL -lGLU

drt:
	g++ ./src/drt/*.cpp -o ./drt -lglut -lGL -lGLU



everything: all old_max old_calculation old_drawgraph

old_calculation:
	g++ ./src/outdated/prev_calculation.cpp -o ./old_calculate -lmpfr

old_max:
	g++ ./src/outdated/prev_max.cpp -o ./old_max -lmpfr

old_drawgraph:
	g++ ./src/outdated/drawgraph.cpp -o ./old_draw -lglut -lGL -lGLU
