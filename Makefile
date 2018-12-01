all: compile

compile:
	mkdir -p build &&\
	cd build &&\
	echo "\n\033[1;33m█ Generating makefiles...\033[0m\n" &&\
	cmake .. &&\
	echo "\n\033[1;93m█ Compiling...\033[0m\n" &&\
	make