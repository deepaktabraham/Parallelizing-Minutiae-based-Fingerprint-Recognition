COMMON_API = api/common/getfiles.cpp

SEQ_API = api/sequential/normalizer.cpp \
	api/sequential/ridgeorient.cpp \
	api/sequential/ridgefilter.cpp \
	api/sequential/mask.cpp \
	api/sequential/thin.cpp \
	api/sequential/minutiae.cpp

PAR_API = api/parallel/normalizer.cpp \
	api/parallel/ridgeorient.cpp \
	api/parallel/ridgefilter.cpp \
	api/parallel/mask.cpp \
	api/parallel/thin.cpp \
	api/parallel/minutiae.cpp
INC_DIR = inc
BUILD_DIR = built
CFLAGS = -std=c++11
PFLAGS = -fopenmp
OPENCV_FLAGS = `pkg-config --cflags opencv`
OPENCV_LIBS = `pkg-config --libs opencv`

all: InitDatabase SeqApp ParApp

InitDatabase: InitDatabase.cpp
	@echo -n "Compiling InitDatabase App... "
	@mkdir -p built
	@g++ $(OPENCV_FLAGS) $(CFLAGS) -I$(INC_DIR) $(COMMON_API) $(SEQ_API) $< -o$(BUILD_DIR)/$@ $(OPENCV_LIBS)
	@echo "Done!"

SeqApp: SeqApp.cpp
	@echo -n "Compiling the Sequential App... "
	@mkdir -p built
	@g++ $(OPENCV_FLAGS) $(CFLAGS) -I$(INC_DIR) $(COMMON_API) $(SEQ_API) $< -o$(BUILD_DIR)/$@ $(OPENCV_LIBS)
	@echo "Done!"

ParApp: ParApp.cpp
	@echo -n "Compiling the Parallel App... "
	@mkdir -p built
	@g++ $(OPENCV_FLAGS) $(CFLAGS) $(PFLAGS) -I$(INC_DIR) $(COMMON_API) $(PAR_API) $< -o$(BUILD_DIR)/$@ $(OPENCV_LIBS)
	@echo "Done!"

clean:
	@echo -n "Removing executables... "
	@rm -rf built/
	@echo "Done!"

distclean: clean
	@echo -n "Removing XML database... "
	@rm -rf xml_database/*
	@echo "Done!"
