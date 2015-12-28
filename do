#!/bin/bash

COMPILER=g++
LINKER=g++
LINKER_LIBS="-lglfw3 -lGL -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -llua5.2"
INCLUDE_LOCAL=$PWD
INCLUDE_LUA=/usr/include/lua5.2

remove_main() {
	if [ -e main.o ]; then
		rm main.o
	fi
}

remove_previous() {
	echo "CLEAR DATA"
	for i in common/*.o lua_bridge.o ogl.o lua.o
	do
		if [ -e $i ]; then
			rm $i
		fi
	done
}

compile() {
	echo "Compile main.c"
	gcc -I$INCLUDE_LUA -I$INCLUDE_LOCAL -Wall main.c -c -o lua.o
	echo "Compile ogl.cpp"
	$COMPILER -Wall -I$INCLUDE_LOCAL ogl.cpp -c -o ogl.o
	echo "Compile lua_bridge.cpp"
	$COMPILER -Wall -I$INCLUDE_LOCAL lua_bridge.cpp -c -o lua_bridge.o
	
	for i in common/*.cpp
	do
		echo "Compile $i"
		$COMPILER -Wall $i -c -o $i.o
	done
	
	
	#echo "Compile shader.cpp"
	#$COMPILER -Wall shader.cpp -c -o shader.o
	# g++ ogl.cpp -o ogl.o -lglfw -lGL -lGLEW
	
}

link() {
	LINK_ADD=""

	for i in common/*.o
	do
		LINK_ADD="$LINK_ADD $i"
	done
	
	echo "Linking."
	LINE="$LINKER lua_bridge.o ogl.o lua.o $LINK_ADD -o main.o $LINKER_LIBS"
	echo Execute: $LINE
	$LINE
}

none() {
	exit
}

run() {
	./main.o
}

default() {
	remove_main
	compile
	link
	remove_previous
	none
}

if [[ "$@" != "" ]];
then
	for i in "$@"
	do
		case $i in
			--default)
				default
			;;
			*)
		    	echo "Unknown option, exiting..."
			;;
		esac
	done
else
	default
fi
