# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lobakkang/active_github/uppexo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lobakkang/active_github/uppexo

# Include any dependencies generated for this target.
include CMakeFiles/dev.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/dev.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/dev.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dev.dir/flags.make

CMakeFiles/dev.dir/tests/dev.cpp.o: CMakeFiles/dev.dir/flags.make
CMakeFiles/dev.dir/tests/dev.cpp.o: tests/dev.cpp
CMakeFiles/dev.dir/tests/dev.cpp.o: CMakeFiles/dev.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lobakkang/active_github/uppexo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dev.dir/tests/dev.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/dev.dir/tests/dev.cpp.o -MF CMakeFiles/dev.dir/tests/dev.cpp.o.d -o CMakeFiles/dev.dir/tests/dev.cpp.o -c /home/lobakkang/active_github/uppexo/tests/dev.cpp

CMakeFiles/dev.dir/tests/dev.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dev.dir/tests/dev.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lobakkang/active_github/uppexo/tests/dev.cpp > CMakeFiles/dev.dir/tests/dev.cpp.i

CMakeFiles/dev.dir/tests/dev.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dev.dir/tests/dev.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lobakkang/active_github/uppexo/tests/dev.cpp -o CMakeFiles/dev.dir/tests/dev.cpp.s

# Object files for target dev
dev_OBJECTS = \
"CMakeFiles/dev.dir/tests/dev.cpp.o"

# External object files for target dev
dev_EXTERNAL_OBJECTS =

dev: CMakeFiles/dev.dir/tests/dev.cpp.o
dev: CMakeFiles/dev.dir/build.make
dev: /lib/libvulkan.so
dev: /usr/lib/libglfw.so.3.3
dev: libuppexo.so
dev: third_party/obj/libtinyobjloader.a
dev: CMakeFiles/dev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lobakkang/active_github/uppexo/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable dev"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dev.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dev.dir/build: dev
.PHONY : CMakeFiles/dev.dir/build

CMakeFiles/dev.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dev.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dev.dir/clean

CMakeFiles/dev.dir/depend:
	cd /home/lobakkang/active_github/uppexo && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lobakkang/active_github/uppexo /home/lobakkang/active_github/uppexo /home/lobakkang/active_github/uppexo /home/lobakkang/active_github/uppexo /home/lobakkang/active_github/uppexo/CMakeFiles/dev.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dev.dir/depend

