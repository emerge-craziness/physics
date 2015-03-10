all: max comparison calculation drt

calculation:
	g++ calculation/{MyClass,main}.cpp -o ../calculate -lntl

max:
	g++ max/*.cpp -o ../max -lmpfr

comparison:
	g++ comparison/*.cpp -o ../comparison -lglut -lGL -lGLU

drt:
	g++ drt/*.cpp -o ../drt -lglut -lGL -lGLU



everything: all old_max old_calculation old_drawgraph

old_calculation:
	g++ outdated/prev_calculation.cpp -o ../old_calculate -lmpfr

old_max:
	g++ outdated/prev_max.cpp -o ../old_max -lmpfr

old_drawgraph:
	g++ outdated/drawgraph.cpp -o ../old_draw -lglut -lGL -lGLU
