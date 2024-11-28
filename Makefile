all:
	g++ -I src/include -l src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2