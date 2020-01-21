 SHELL = bash.exe
 
build:
	g++ -w -std=c++14 -Wfatal-errors \
	-I. \
	./src/*.cpp \
	-o game.exe \
	-L. \
	-lglfw3 \
	-lopengl32 \
	-lgdi32 \

clean:
	rm game.exe
 
run:
	game.exe
