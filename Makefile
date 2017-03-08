all: SeqApp ParApp
	@echo "Done!"	

SeqApp: SeqApp.cpp
	@echo "Compiling the Sequential App..."
	@mkdir -p built
	@g++ -ggdb `pkg-config --cflags opencv` -o `basename SeqApp.cpp .cpp` SeqApp.cpp `pkg-config --libs opencv`
	@mv SeqApp built/

ParApp: ParApp.cpp
	@echo "Compiling the Parallel App..."
	@mkdir -p built
	@g++ -ggdb `pkg-config --cflags opencv` -o `basename ParApp.cpp .cpp` ParApp.cpp `pkg-config --libs opencv`
	@mv ParApp built/

clean:
	@echo "Removing executables..."
	@rm -rf built/
	@echo "Done!"
