COMPILER = clang++
SOURCE_LIBS = -Ilib/
MACOS_OPT = -std=c++17 -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
MACOS_OUT = -o "bin/build_osx"
CFILES = src/*.cpp

build_osx:
	$(COMPILER) $(CFILES) $(SOURCE_LIBS) $(MACOS_OUT) $(MACOS_OPT)