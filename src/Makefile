CFLAG=`pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`




Project1: Main.cpp
	g++ $(CFLAG) $(LIBS) -o Project1 Main.cpp
clean:
	@for obj in $(OBJ); do\
		if test -f $$obj; then rm $$obj; fi; done
	@if (test -f Project1); then rm Project1; fi;

