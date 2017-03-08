all: SeqApp ParApp

SeqApp: SeqApp.cpp
	g++ -ggdb `pkg-config --cflags opencv` -o `basename SeqApp.cpp .cpp` SeqApp.cpp `pkg-config --libs opencv`

ParApp: ParApp.cpp
	g++ -ggdb `pkg-config --cflags opencv` -o `basename ParApp.cpp .cpp` ParApp.cpp `pkg-config --libs opencv`

clean:
	rm -rf SeqApp ParApp

