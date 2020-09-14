#!/bin/bash


run=false

rm -rf ./bin
rm -rf ./build

# loop options
while test $# != 0; do
	case "$1" in
	-G) # generate build files
		cmake -S ./ -B ./build
		;;
	-E) # run project on completion
		run=true
		;;
	*) ;;
	esac
	shift
done

# make project
cd ./build
make
cd ..

if [ "$run" = true ] ; then
	# run project
	cd ./bin
	./Lighting
	cd ..
fi

