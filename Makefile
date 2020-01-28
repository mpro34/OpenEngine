 SHELL = bash.exe
 
build:
	g++ -w -std=c++14 -Wfatal-errors \
	-I. \
	./src/main.cpp \
	./src/classes/*.cpp \
	-I./libs/assimp \
	-L./libs/assimp \
	-o game.exe \
	-lglew32 -lopengl32 \
	-lglfw3 \
	-lgdi32 \
	-lassimp-vc140-mt 

clean:
	rm game.exe
 
run:
	game.exe
