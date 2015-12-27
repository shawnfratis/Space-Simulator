#!/bin/bash

COMPILER=g++
LINKER=g++
LINKER_LIBS="-lglfw3 -lGL -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor"
INCLUDE_LOCAL=$PWD

remove_main() {
	if [ -e main.o ]; then
		rm main.o
	fi
}

remove_previous() {
	echo "CLEAR DATA"
	if [ -e out.o ]; then
		rm out.o
	fi
	for i in common/*.o
	do
		rm $i
	done
}

compile() {
	echo "Compile main.cpp"
	$COMPILER -Wall -I$INCLUDE_LOCAL main.cpp -c -o out.o
	
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
	LINE="$LINKER out.o $LINK_ADD -o main.o $LINKER_LIBS"
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

if [ "$@" != "" ];
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
