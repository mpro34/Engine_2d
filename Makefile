 SHELL = bash.exe
 
build:
	g++ -w -std=c++14 -Wfatal-errors -m32 -static-libstdc++ \
	./src/*.cpp \
	-o game.exe \
	-IC:\libsdl\SDL2\include \
	-LC:\libsdl\SDL2\lib \
	-I.\lib\lua \
	-L.\lib\lua \
	-llua53 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2_image \
	-lSDL2_ttf \
	-lSDL2_mixer \
	-lSDL2
 
clean:
	rm game.exe
 
run:
	game.exe
