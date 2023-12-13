# Project: torcs-car-setup-editor
# Makefile created from the makefilo of Dev-C++ 4.9.9.0

CPP  = g++
CC   = gcc

OBJ  = main.o export-xml.o open-car-data.o save-car-data.o tracks.o import-xml.o engine.o help-window.o loadEngine.o calcParmamsABSG.o warningWindow.o fileBrowser.o ac3dLoader/ac3d.o ac3dLoader/mainAC3dloader.o ac3dLoader/render.o ac3dLoader/texture.o gears.o graphic.o command_line.o portability.o read-dir.o $(RES)
LINKOBJ  = main.o export-xml.o open-car-data.o save-car-data.o tracks.o import-xml.o engine.o help-window.o loadEngine.o calcParmamsABSG.o warningWindow.o fileBrowser.o ac3dLoader/ac3d.o ac3dLoader/mainAC3dloader.o ac3dLoader/render.o ac3dLoader/texture.o gears.o graphic.o command_line.o portability.o read-dir.o $(RES)
LIBS =  -L"/usr/local/lib" -lGL -lGLU -lglut -lglui
INCS =  -I"/usr/local/include"
CXXINCS =  -I"/usr/local/include"
BIN  = torcs-car-setup-editor
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  

.PHONY: all all-before all-after clean clean-custom

all: all-before torcs-car-setup-editor all-after


clean: clean-custom
	rm -f $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o "torcs-car-setup-editor" $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)

export-xml.o: export-xml.cpp
	$(CPP) -c export-xml.cpp -o export-xml.o $(CXXFLAGS)

open-car-data.o: open-car-data.cpp
	$(CPP) -c open-car-data.cpp -o open-car-data.o $(CXXFLAGS)

save-car-data.o: save-car-data.cpp
	$(CPP) -c save-car-data.cpp -o save-car-data.o $(CXXFLAGS)

tracks.o: tracks.cpp
	$(CPP) -c tracks.cpp -o tracks.o $(CXXFLAGS)

import-xml.o: import-xml.cpp
	$(CPP) -c import-xml.cpp -o import-xml.o $(CXXFLAGS)

engine.o: engine.cpp
	$(CPP) -c engine.cpp -o engine.o $(CXXFLAGS)

help-window.o: help-window.cpp
	$(CPP) -c help-window.cpp -o help-window.o $(CXXFLAGS)

loadEngine.o: loadEngine.cpp
	$(CPP) -c loadEngine.cpp -o loadEngine.o $(CXXFLAGS)

calcParmamsABSG.o: calcParmamsABSG.cpp
	$(CPP) -c calcParmamsABSG.cpp -o calcParmamsABSG.o $(CXXFLAGS)

warningWindow.o: warningWindow.cpp
	$(CPP) -c warningWindow.cpp -o warningWindow.o $(CXXFLAGS)

fileBrowser.o: fileBrowser.cpp
	$(CPP) -c fileBrowser.cpp -o fileBrowser.o $(CXXFLAGS)

ac3dLoader/ac3d.o: ac3dLoader/ac3d.cpp
	$(CPP) -c ac3dLoader/ac3d.cpp -o ac3dLoader/ac3d.o $(CXXFLAGS)

ac3dLoader/mainAC3dloader.o: ac3dLoader/mainAC3dloader.cpp
	$(CPP) -c ac3dLoader/mainAC3dloader.cpp -o ac3dLoader/mainAC3dloader.o $(CXXFLAGS)

ac3dLoader/render.o: ac3dLoader/render.cpp
	$(CPP) -c ac3dLoader/render.cpp -o ac3dLoader/render.o $(CXXFLAGS)

ac3dLoader/texture.o: ac3dLoader/texture.cpp
	$(CPP) -c ac3dLoader/texture.cpp -o ac3dLoader/texture.o $(CXXFLAGS)

gears.o: gears.cpp
	$(CPP) -c gears.cpp -o gears.o $(CXXFLAGS)

graphic.o: graphic.cpp
	$(CPP) -c graphic.cpp -o graphic.o $(CXXFLAGS)

command_line.o: command_line.cpp
	$(CPP) -c command_line.cpp -o command_line.o $(CXXFLAGS)

portability.o: portability.cpp
	$(CPP) -c portability.cpp -o portability.o $(CXXFLAGS)

read-dir.o: read-dir.cpp
	$(CPP) -c read-dir.cpp -o read-dir.o $(CXXFLAGS)
