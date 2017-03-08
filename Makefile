all: SeqApp ParApp

SeqApp: SeqApp.cpp
	@echo -n "Compiling the Sequential App... "
	@mkdir -p built
	@g++ -ggdb `pkg-config --cflags opencv` -o `basename SeqApp.cpp .cpp` SeqApp.cpp `pkg-config --libs opencv`
	@mv SeqApp built/
	@echo "Done!"

ParApp: ParApp.cpp
	@echo -n "Compiling the Parallel App... "
	@mkdir -p built
	@g++ -ggdb `pkg-config --cflags opencv` -o `basename ParApp.cpp .cpp` ParApp.cpp `pkg-config --libs opencv`
	@mv ParApp built/
	@echo "Done!"

clean:
	@echo -n "Removing executables... "
	@rm -rf built/
	@echo "Done!"
