cmake_minimum_required(VERSION 3.1)

project (carsetupeditor)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glui CONFIG REQUIRED)

set(loader_srcs
	ac3dLoader/ac3d.cpp
	ac3dLoader/mainAC3dloader.cpp
	ac3dLoader/render.cpp
	ac3dLoader/texture.cpp
	ac3dLoader/ac3d.h
        )

set(srcs
	calcParmamsABSG.cpp
	export-xml.cpp
	graphic.cpp
	loadEngine.cpp
	open-car-data.cpp
	save-car-data.cpp
	command_line.cpp
	fileBrowser.cpp
	help-window.cpp
	main.cpp
	portability.cpp
	tracks.cpp
	engine.cpp
	gears.cpp
        import-xml.cpp
	read-dir.cpp
	warningWindow.cpp
	cardata.h
	cardatadef.h
	portability.h
	torcs-car-setup-editor_private.h
	)

add_executable(carsetupeditor ${loader_srcs} ${srcs})

target_link_libraries(carsetupeditor PUBLIC glui::glui)
