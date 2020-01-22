 SHELL = bash.exe
 
build:
	g++ -w -std=c++14 -Wfatal-errors \
	-I. \
	./src/*.cpp \
	-o game.exe \
	-lglew32 -lopengl32 \
	-lglfw3 \
	-lgdi32

clean:
	rm game.exe
 
run:
	game.exe
