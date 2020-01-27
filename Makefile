 SHELL = bash.exe
 
build:
	g++ -w -std=c++14 -Wfatal-errors \
	-I. \
	./src/main.cpp \
	./src/classes/*.cpp \
	-o game.exe \
	-lglew32 -lopengl32 \
	-lglfw3 \
	-lgdi32 \
	-lassimp-vc142-mt 

clean:
	rm game.exe
 
run:
	game.exe
