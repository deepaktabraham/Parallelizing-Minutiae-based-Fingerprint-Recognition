API = 	api/normalizer.cpp \
	api/ridgeorient.cpp \
	api/ridgefilter.cpp \
	api/thin.cpp \
	api/mask.cpp 

INC_DIR = inc
BUILD_DIR = built
CFLAGS = -std=c++11
OPENCV_FLAGS = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

all: SeqApp ParApp

SeqApp: SeqApp.cpp
	@echo -n "Compiling the Sequential App... "
	@mkdir -p built
	@g++ $(OPENCV_FLAGS) $(CFLAGS) -I$(INC_DIR) $(API) $< -o$(BUILD_DIR)/$@ $(OPENCV_LIBS)
	@echo "Done!"

ParApp: ParApp.cpp
	@echo -n "Compiling the Parallel App... "
	@mkdir -p built
	@g++ $(OPENCV_FLAGS) $(CFLAGS) -I$(INC_DIR) $< -o$(BUILD_DIR)/$@ $(OPENCV_LIBS)
	@echo "Done!"

clean:
	@echo -n "Removing executables... "
	@rm -rf built/
	@echo "Done!"
