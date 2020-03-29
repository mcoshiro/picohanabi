ROOTFLAGS=`root-config --cflags --glibs`

all: generate_display

generate_display: src/generate_display.cxx
	g++ -o bin/generate_display src/generate_display.cxx $(ROOTFLAGS)
