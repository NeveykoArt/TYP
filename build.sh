#!/bin/bash

readonly antlr=antlr-4.11.1-complete.jar

FILE=build
if test -d "$FILE"; then
	echo "$FILE существует"
	cmake --build --preset debug
	ctest --preset debug
else
	echo "$FILE не существует"
	curl -O https://www.antlr.org/download/$antlr
	cmake --preset debug
	cmake --build --preset debug
	ctest --preset debug
fi
