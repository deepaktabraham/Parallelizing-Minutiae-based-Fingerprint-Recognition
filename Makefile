API = api/thin.cpp api/orient.cpp api/normalizer.cpp api/ridgeorient.cpp api/ridgefilter.cpp

all: SeqApp ParApp

SeqApp: SeqApp.cpp
	@echo -n "Compiling the Sequential App... "
	@mkdir -p built
	@g++ `pkg-config --cflags opencv` -o built/SeqApp SeqApp.cpp $(API) `pkg-config --libs opencv` -std=c++11
	@echo "Done!"

ParApp: ParApp.cpp
	@echo -n "Compiling the Parallel App... "
	@mkdir -p built
	@g++ `pkg-config --cflags opencv` -o built/ParApp ParApp.cpp `pkg-config --libs opencv`
	@echo "Done!"

clean:
	@echo -n "Removing executables... "
	@rm -rf built/
	@echo "Done!"
