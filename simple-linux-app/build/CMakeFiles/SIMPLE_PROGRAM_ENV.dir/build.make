# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/emiile-8/Documents/Git/assignment1/simple-linux-app

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/emiile-8/Documents/Git/assignment1/simple-linux-app/build

# Include any dependencies generated for this target.
include CMakeFiles/SIMPLE_PROGRAM_ENV.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SIMPLE_PROGRAM_ENV.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SIMPLE_PROGRAM_ENV.dir/flags.make

CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o: CMakeFiles/SIMPLE_PROGRAM_ENV.dir/flags.make
CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/emiile-8/Documents/Git/assignment1/simple-linux-app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o -c /home/emiile-8/Documents/Git/assignment1/simple-linux-app/src/main.cpp

CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/emiile-8/Documents/Git/assignment1/simple-linux-app/src/main.cpp > CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.i

CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/emiile-8/Documents/Git/assignment1/simple-linux-app/src/main.cpp -o CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.s

# Object files for target SIMPLE_PROGRAM_ENV
SIMPLE_PROGRAM_ENV_OBJECTS = \
"CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o"

# External object files for target SIMPLE_PROGRAM_ENV
SIMPLE_PROGRAM_ENV_EXTERNAL_OBJECTS =

../bin/SIMPLE_PROGRAM_ENV: CMakeFiles/SIMPLE_PROGRAM_ENV.dir/src/main.cpp.o
../bin/SIMPLE_PROGRAM_ENV: CMakeFiles/SIMPLE_PROGRAM_ENV.dir/build.make
../bin/SIMPLE_PROGRAM_ENV: CMakeFiles/SIMPLE_PROGRAM_ENV.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/emiile-8/Documents/Git/assignment1/simple-linux-app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/SIMPLE_PROGRAM_ENV"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SIMPLE_PROGRAM_ENV.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SIMPLE_PROGRAM_ENV.dir/build: ../bin/SIMPLE_PROGRAM_ENV

.PHONY : CMakeFiles/SIMPLE_PROGRAM_ENV.dir/build

CMakeFiles/SIMPLE_PROGRAM_ENV.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SIMPLE_PROGRAM_ENV.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SIMPLE_PROGRAM_ENV.dir/clean

CMakeFiles/SIMPLE_PROGRAM_ENV.dir/depend:
	cd /home/emiile-8/Documents/Git/assignment1/simple-linux-app/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/emiile-8/Documents/Git/assignment1/simple-linux-app /home/emiile-8/Documents/Git/assignment1/simple-linux-app /home/emiile-8/Documents/Git/assignment1/simple-linux-app/build /home/emiile-8/Documents/Git/assignment1/simple-linux-app/build /home/emiile-8/Documents/Git/assignment1/simple-linux-app/build/CMakeFiles/SIMPLE_PROGRAM_ENV.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SIMPLE_PROGRAM_ENV.dir/depend

