# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running tests..."
	/usr/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test
.PHONY : test/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components
.PHONY : list_install_components/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lobakkang/active_github/uppexo/CMakeFiles /home/lobakkang/active_github/uppexo//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/lobakkang/active_github/uppexo/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Shaders

# Build rule for target.
Shaders: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Shaders
.PHONY : Shaders

# fast build rule for target.
Shaders/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Shaders.dir/build.make CMakeFiles/Shaders.dir/build
.PHONY : Shaders/fast

#=============================================================================
# Target rules for targets named uppexo

# Build rule for target.
uppexo: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 uppexo
.PHONY : uppexo

# fast build rule for target.
uppexo/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/build
.PHONY : uppexo/fast

#=============================================================================
# Target rules for targets named uppexo_demo

# Build rule for target.
uppexo_demo: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 uppexo_demo
.PHONY : uppexo_demo

# fast build rule for target.
uppexo_demo/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo_demo.dir/build.make CMakeFiles/uppexo_demo.dir/build
.PHONY : uppexo_demo/fast

#=============================================================================
# Target rules for targets named forward_rendering

# Build rule for target.
forward_rendering: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 forward_rendering
.PHONY : forward_rendering

# fast build rule for target.
forward_rendering/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/forward_rendering.dir/build.make CMakeFiles/forward_rendering.dir/build
.PHONY : forward_rendering/fast

#=============================================================================
# Target rules for targets named tinyobjloader

# Build rule for target.
tinyobjloader: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 tinyobjloader
.PHONY : tinyobjloader

# fast build rule for target.
tinyobjloader/fast:
	$(MAKE) $(MAKESILENT) -f third_party/obj/CMakeFiles/tinyobjloader.dir/build.make third_party/obj/CMakeFiles/tinyobjloader.dir/build
.PHONY : tinyobjloader/fast

#=============================================================================
# Target rules for targets named uninstall

# Build rule for target.
uninstall: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 uninstall
.PHONY : uninstall

# fast build rule for target.
uninstall/fast:
	$(MAKE) $(MAKESILENT) -f third_party/obj/CMakeFiles/uninstall.dir/build.make third_party/obj/CMakeFiles/uninstall.dir/build
.PHONY : uninstall/fast

demo/main.o: demo/main.cpp.o
.PHONY : demo/main.o

# target to build an object file
demo/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo_demo.dir/build.make CMakeFiles/uppexo_demo.dir/demo/main.cpp.o
.PHONY : demo/main.cpp.o

demo/main.i: demo/main.cpp.i
.PHONY : demo/main.i

# target to preprocess a source file
demo/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo_demo.dir/build.make CMakeFiles/uppexo_demo.dir/demo/main.cpp.i
.PHONY : demo/main.cpp.i

demo/main.s: demo/main.cpp.s
.PHONY : demo/main.s

# target to generate assembly for a file
demo/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo_demo.dir/build.make CMakeFiles/uppexo_demo.dir/demo/main.cpp.s
.PHONY : demo/main.cpp.s

src/base/buffer.o: src/base/buffer.cpp.o
.PHONY : src/base/buffer.o

# target to build an object file
src/base/buffer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/buffer.cpp.o
.PHONY : src/base/buffer.cpp.o

src/base/buffer.i: src/base/buffer.cpp.i
.PHONY : src/base/buffer.i

# target to preprocess a source file
src/base/buffer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/buffer.cpp.i
.PHONY : src/base/buffer.cpp.i

src/base/buffer.s: src/base/buffer.cpp.s
.PHONY : src/base/buffer.s

# target to generate assembly for a file
src/base/buffer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/buffer.cpp.s
.PHONY : src/base/buffer.cpp.s

src/base/commandbuffer.o: src/base/commandbuffer.cpp.o
.PHONY : src/base/commandbuffer.o

# target to build an object file
src/base/commandbuffer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/commandbuffer.cpp.o
.PHONY : src/base/commandbuffer.cpp.o

src/base/commandbuffer.i: src/base/commandbuffer.cpp.i
.PHONY : src/base/commandbuffer.i

# target to preprocess a source file
src/base/commandbuffer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/commandbuffer.cpp.i
.PHONY : src/base/commandbuffer.cpp.i

src/base/commandbuffer.s: src/base/commandbuffer.cpp.s
.PHONY : src/base/commandbuffer.s

# target to generate assembly for a file
src/base/commandbuffer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/commandbuffer.cpp.s
.PHONY : src/base/commandbuffer.cpp.s

src/base/descriptor.o: src/base/descriptor.cpp.o
.PHONY : src/base/descriptor.o

# target to build an object file
src/base/descriptor.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/descriptor.cpp.o
.PHONY : src/base/descriptor.cpp.o

src/base/descriptor.i: src/base/descriptor.cpp.i
.PHONY : src/base/descriptor.i

# target to preprocess a source file
src/base/descriptor.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/descriptor.cpp.i
.PHONY : src/base/descriptor.cpp.i

src/base/descriptor.s: src/base/descriptor.cpp.s
.PHONY : src/base/descriptor.s

# target to generate assembly for a file
src/base/descriptor.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/descriptor.cpp.s
.PHONY : src/base/descriptor.cpp.s

src/base/device.o: src/base/device.cpp.o
.PHONY : src/base/device.o

# target to build an object file
src/base/device.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/device.cpp.o
.PHONY : src/base/device.cpp.o

src/base/device.i: src/base/device.cpp.i
.PHONY : src/base/device.i

# target to preprocess a source file
src/base/device.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/device.cpp.i
.PHONY : src/base/device.cpp.i

src/base/device.s: src/base/device.cpp.s
.PHONY : src/base/device.s

# target to generate assembly for a file
src/base/device.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/device.cpp.s
.PHONY : src/base/device.cpp.s

src/base/framebuffer.o: src/base/framebuffer.cpp.o
.PHONY : src/base/framebuffer.o

# target to build an object file
src/base/framebuffer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/framebuffer.cpp.o
.PHONY : src/base/framebuffer.cpp.o

src/base/framebuffer.i: src/base/framebuffer.cpp.i
.PHONY : src/base/framebuffer.i

# target to preprocess a source file
src/base/framebuffer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/framebuffer.cpp.i
.PHONY : src/base/framebuffer.cpp.i

src/base/framebuffer.s: src/base/framebuffer.cpp.s
.PHONY : src/base/framebuffer.s

# target to generate assembly for a file
src/base/framebuffer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/framebuffer.cpp.s
.PHONY : src/base/framebuffer.cpp.s

src/base/graphicPipeline.o: src/base/graphicPipeline.cpp.o
.PHONY : src/base/graphicPipeline.o

# target to build an object file
src/base/graphicPipeline.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/graphicPipeline.cpp.o
.PHONY : src/base/graphicPipeline.cpp.o

src/base/graphicPipeline.i: src/base/graphicPipeline.cpp.i
.PHONY : src/base/graphicPipeline.i

# target to preprocess a source file
src/base/graphicPipeline.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/graphicPipeline.cpp.i
.PHONY : src/base/graphicPipeline.cpp.i

src/base/graphicPipeline.s: src/base/graphicPipeline.cpp.s
.PHONY : src/base/graphicPipeline.s

# target to generate assembly for a file
src/base/graphicPipeline.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/graphicPipeline.cpp.s
.PHONY : src/base/graphicPipeline.cpp.s

src/base/image.o: src/base/image.cpp.o
.PHONY : src/base/image.o

# target to build an object file
src/base/image.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/image.cpp.o
.PHONY : src/base/image.cpp.o

src/base/image.i: src/base/image.cpp.i
.PHONY : src/base/image.i

# target to preprocess a source file
src/base/image.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/image.cpp.i
.PHONY : src/base/image.cpp.i

src/base/image.s: src/base/image.cpp.s
.PHONY : src/base/image.s

# target to generate assembly for a file
src/base/image.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/image.cpp.s
.PHONY : src/base/image.cpp.s

src/base/instance.o: src/base/instance.cpp.o
.PHONY : src/base/instance.o

# target to build an object file
src/base/instance.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/instance.cpp.o
.PHONY : src/base/instance.cpp.o

src/base/instance.i: src/base/instance.cpp.i
.PHONY : src/base/instance.i

# target to preprocess a source file
src/base/instance.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/instance.cpp.i
.PHONY : src/base/instance.cpp.i

src/base/instance.s: src/base/instance.cpp.s
.PHONY : src/base/instance.s

# target to generate assembly for a file
src/base/instance.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/instance.cpp.s
.PHONY : src/base/instance.cpp.s

src/base/renderpass.o: src/base/renderpass.cpp.o
.PHONY : src/base/renderpass.o

# target to build an object file
src/base/renderpass.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/renderpass.cpp.o
.PHONY : src/base/renderpass.cpp.o

src/base/renderpass.i: src/base/renderpass.cpp.i
.PHONY : src/base/renderpass.i

# target to preprocess a source file
src/base/renderpass.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/renderpass.cpp.i
.PHONY : src/base/renderpass.cpp.i

src/base/renderpass.s: src/base/renderpass.cpp.s
.PHONY : src/base/renderpass.s

# target to generate assembly for a file
src/base/renderpass.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/renderpass.cpp.s
.PHONY : src/base/renderpass.cpp.s

src/base/sampler.o: src/base/sampler.cpp.o
.PHONY : src/base/sampler.o

# target to build an object file
src/base/sampler.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/sampler.cpp.o
.PHONY : src/base/sampler.cpp.o

src/base/sampler.i: src/base/sampler.cpp.i
.PHONY : src/base/sampler.i

# target to preprocess a source file
src/base/sampler.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/sampler.cpp.i
.PHONY : src/base/sampler.cpp.i

src/base/sampler.s: src/base/sampler.cpp.s
.PHONY : src/base/sampler.s

# target to generate assembly for a file
src/base/sampler.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/sampler.cpp.s
.PHONY : src/base/sampler.cpp.s

src/base/shader.o: src/base/shader.cpp.o
.PHONY : src/base/shader.o

# target to build an object file
src/base/shader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/shader.cpp.o
.PHONY : src/base/shader.cpp.o

src/base/shader.i: src/base/shader.cpp.i
.PHONY : src/base/shader.i

# target to preprocess a source file
src/base/shader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/shader.cpp.i
.PHONY : src/base/shader.cpp.i

src/base/shader.s: src/base/shader.cpp.s
.PHONY : src/base/shader.s

# target to generate assembly for a file
src/base/shader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/shader.cpp.s
.PHONY : src/base/shader.cpp.s

src/base/synchronizer.o: src/base/synchronizer.cpp.o
.PHONY : src/base/synchronizer.o

# target to build an object file
src/base/synchronizer.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/synchronizer.cpp.o
.PHONY : src/base/synchronizer.cpp.o

src/base/synchronizer.i: src/base/synchronizer.cpp.i
.PHONY : src/base/synchronizer.i

# target to preprocess a source file
src/base/synchronizer.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/synchronizer.cpp.i
.PHONY : src/base/synchronizer.cpp.i

src/base/synchronizer.s: src/base/synchronizer.cpp.s
.PHONY : src/base/synchronizer.s

# target to generate assembly for a file
src/base/synchronizer.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/base/synchronizer.cpp.s
.PHONY : src/base/synchronizer.cpp.s

src/core/commandBufferRecorder.o: src/core/commandBufferRecorder.cpp.o
.PHONY : src/core/commandBufferRecorder.o

# target to build an object file
src/core/commandBufferRecorder.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/commandBufferRecorder.cpp.o
.PHONY : src/core/commandBufferRecorder.cpp.o

src/core/commandBufferRecorder.i: src/core/commandBufferRecorder.cpp.i
.PHONY : src/core/commandBufferRecorder.i

# target to preprocess a source file
src/core/commandBufferRecorder.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/commandBufferRecorder.cpp.i
.PHONY : src/core/commandBufferRecorder.cpp.i

src/core/commandBufferRecorder.s: src/core/commandBufferRecorder.cpp.s
.PHONY : src/core/commandBufferRecorder.s

# target to generate assembly for a file
src/core/commandBufferRecorder.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/commandBufferRecorder.cpp.s
.PHONY : src/core/commandBufferRecorder.cpp.s

src/core/datatype.o: src/core/datatype.cpp.o
.PHONY : src/core/datatype.o

# target to build an object file
src/core/datatype.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/datatype.cpp.o
.PHONY : src/core/datatype.cpp.o

src/core/datatype.i: src/core/datatype.cpp.i
.PHONY : src/core/datatype.i

# target to preprocess a source file
src/core/datatype.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/datatype.cpp.i
.PHONY : src/core/datatype.cpp.i

src/core/datatype.s: src/core/datatype.cpp.s
.PHONY : src/core/datatype.s

# target to generate assembly for a file
src/core/datatype.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/datatype.cpp.s
.PHONY : src/core/datatype.cpp.s

src/core/mesh.o: src/core/mesh.cpp.o
.PHONY : src/core/mesh.o

# target to build an object file
src/core/mesh.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/mesh.cpp.o
.PHONY : src/core/mesh.cpp.o

src/core/mesh.i: src/core/mesh.cpp.i
.PHONY : src/core/mesh.i

# target to preprocess a source file
src/core/mesh.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/mesh.cpp.i
.PHONY : src/core/mesh.cpp.i

src/core/mesh.s: src/core/mesh.cpp.s
.PHONY : src/core/mesh.s

# target to generate assembly for a file
src/core/mesh.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/core/mesh.cpp.s
.PHONY : src/core/mesh.cpp.s

src/engine/compute.o: src/engine/compute.cpp.o
.PHONY : src/engine/compute.o

# target to build an object file
src/engine/compute.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/compute.cpp.o
.PHONY : src/engine/compute.cpp.o

src/engine/compute.i: src/engine/compute.cpp.i
.PHONY : src/engine/compute.i

# target to preprocess a source file
src/engine/compute.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/compute.cpp.i
.PHONY : src/engine/compute.cpp.i

src/engine/compute.s: src/engine/compute.cpp.s
.PHONY : src/engine/compute.s

# target to generate assembly for a file
src/engine/compute.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/compute.cpp.s
.PHONY : src/engine/compute.cpp.s

src/engine/deferred.o: src/engine/deferred.cpp.o
.PHONY : src/engine/deferred.o

# target to build an object file
src/engine/deferred.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/deferred.cpp.o
.PHONY : src/engine/deferred.cpp.o

src/engine/deferred.i: src/engine/deferred.cpp.i
.PHONY : src/engine/deferred.i

# target to preprocess a source file
src/engine/deferred.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/deferred.cpp.i
.PHONY : src/engine/deferred.cpp.i

src/engine/deferred.s: src/engine/deferred.cpp.s
.PHONY : src/engine/deferred.s

# target to generate assembly for a file
src/engine/deferred.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/deferred.cpp.s
.PHONY : src/engine/deferred.cpp.s

src/engine/engine.o: src/engine/engine.cpp.o
.PHONY : src/engine/engine.o

# target to build an object file
src/engine/engine.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/engine.cpp.o
.PHONY : src/engine/engine.cpp.o

src/engine/engine.i: src/engine/engine.cpp.i
.PHONY : src/engine/engine.i

# target to preprocess a source file
src/engine/engine.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/engine.cpp.i
.PHONY : src/engine/engine.cpp.i

src/engine/engine.s: src/engine/engine.cpp.s
.PHONY : src/engine/engine.s

# target to generate assembly for a file
src/engine/engine.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/engine.cpp.s
.PHONY : src/engine/engine.cpp.s

src/engine/forward.o: src/engine/forward.cpp.o
.PHONY : src/engine/forward.o

# target to build an object file
src/engine/forward.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/forward.cpp.o
.PHONY : src/engine/forward.cpp.o

src/engine/forward.i: src/engine/forward.cpp.i
.PHONY : src/engine/forward.i

# target to preprocess a source file
src/engine/forward.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/forward.cpp.i
.PHONY : src/engine/forward.cpp.i

src/engine/forward.s: src/engine/forward.cpp.s
.PHONY : src/engine/forward.s

# target to generate assembly for a file
src/engine/forward.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/engine/forward.cpp.s
.PHONY : src/engine/forward.cpp.s

src/uppexo.o: src/uppexo.cpp.o
.PHONY : src/uppexo.o

# target to build an object file
src/uppexo.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/uppexo.cpp.o
.PHONY : src/uppexo.cpp.o

src/uppexo.i: src/uppexo.cpp.i
.PHONY : src/uppexo.i

# target to preprocess a source file
src/uppexo.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/uppexo.cpp.i
.PHONY : src/uppexo.cpp.i

src/uppexo.s: src/uppexo.cpp.s
.PHONY : src/uppexo.s

# target to generate assembly for a file
src/uppexo.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/uppexo.cpp.s
.PHONY : src/uppexo.cpp.s

src/utils/device_util.o: src/utils/device_util.cpp.o
.PHONY : src/utils/device_util.o

# target to build an object file
src/utils/device_util.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/device_util.cpp.o
.PHONY : src/utils/device_util.cpp.o

src/utils/device_util.i: src/utils/device_util.cpp.i
.PHONY : src/utils/device_util.i

# target to preprocess a source file
src/utils/device_util.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/device_util.cpp.i
.PHONY : src/utils/device_util.cpp.i

src/utils/device_util.s: src/utils/device_util.cpp.s
.PHONY : src/utils/device_util.s

# target to generate assembly for a file
src/utils/device_util.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/device_util.cpp.s
.PHONY : src/utils/device_util.cpp.s

src/utils/log.o: src/utils/log.cpp.o
.PHONY : src/utils/log.o

# target to build an object file
src/utils/log.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/log.cpp.o
.PHONY : src/utils/log.cpp.o

src/utils/log.i: src/utils/log.cpp.i
.PHONY : src/utils/log.i

# target to preprocess a source file
src/utils/log.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/log.cpp.i
.PHONY : src/utils/log.cpp.i

src/utils/log.s: src/utils/log.cpp.s
.PHONY : src/utils/log.s

# target to generate assembly for a file
src/utils/log.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/log.cpp.s
.PHONY : src/utils/log.cpp.s

src/utils/vulkan_util.o: src/utils/vulkan_util.cpp.o
.PHONY : src/utils/vulkan_util.o

# target to build an object file
src/utils/vulkan_util.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/vulkan_util.cpp.o
.PHONY : src/utils/vulkan_util.cpp.o

src/utils/vulkan_util.i: src/utils/vulkan_util.cpp.i
.PHONY : src/utils/vulkan_util.i

# target to preprocess a source file
src/utils/vulkan_util.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/vulkan_util.cpp.i
.PHONY : src/utils/vulkan_util.cpp.i

src/utils/vulkan_util.s: src/utils/vulkan_util.cpp.s
.PHONY : src/utils/vulkan_util.s

# target to generate assembly for a file
src/utils/vulkan_util.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/uppexo.dir/build.make CMakeFiles/uppexo.dir/src/utils/vulkan_util.cpp.s
.PHONY : src/utils/vulkan_util.cpp.s

tests/forward_rendering.o: tests/forward_rendering.cpp.o
.PHONY : tests/forward_rendering.o

# target to build an object file
tests/forward_rendering.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/forward_rendering.dir/build.make CMakeFiles/forward_rendering.dir/tests/forward_rendering.cpp.o
.PHONY : tests/forward_rendering.cpp.o

tests/forward_rendering.i: tests/forward_rendering.cpp.i
.PHONY : tests/forward_rendering.i

# target to preprocess a source file
tests/forward_rendering.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/forward_rendering.dir/build.make CMakeFiles/forward_rendering.dir/tests/forward_rendering.cpp.i
.PHONY : tests/forward_rendering.cpp.i

tests/forward_rendering.s: tests/forward_rendering.cpp.s
.PHONY : tests/forward_rendering.s

# target to generate assembly for a file
tests/forward_rendering.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/forward_rendering.dir/build.make CMakeFiles/forward_rendering.dir/tests/forward_rendering.cpp.s
.PHONY : tests/forward_rendering.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... install"
	@echo "... install/local"
	@echo "... install/strip"
	@echo "... list_install_components"
	@echo "... rebuild_cache"
	@echo "... test"
	@echo "... Shaders"
	@echo "... uninstall"
	@echo "... forward_rendering"
	@echo "... tinyobjloader"
	@echo "... uppexo"
	@echo "... uppexo_demo"
	@echo "... demo/main.o"
	@echo "... demo/main.i"
	@echo "... demo/main.s"
	@echo "... src/base/buffer.o"
	@echo "... src/base/buffer.i"
	@echo "... src/base/buffer.s"
	@echo "... src/base/commandbuffer.o"
	@echo "... src/base/commandbuffer.i"
	@echo "... src/base/commandbuffer.s"
	@echo "... src/base/descriptor.o"
	@echo "... src/base/descriptor.i"
	@echo "... src/base/descriptor.s"
	@echo "... src/base/device.o"
	@echo "... src/base/device.i"
	@echo "... src/base/device.s"
	@echo "... src/base/framebuffer.o"
	@echo "... src/base/framebuffer.i"
	@echo "... src/base/framebuffer.s"
	@echo "... src/base/graphicPipeline.o"
	@echo "... src/base/graphicPipeline.i"
	@echo "... src/base/graphicPipeline.s"
	@echo "... src/base/image.o"
	@echo "... src/base/image.i"
	@echo "... src/base/image.s"
	@echo "... src/base/instance.o"
	@echo "... src/base/instance.i"
	@echo "... src/base/instance.s"
	@echo "... src/base/renderpass.o"
	@echo "... src/base/renderpass.i"
	@echo "... src/base/renderpass.s"
	@echo "... src/base/sampler.o"
	@echo "... src/base/sampler.i"
	@echo "... src/base/sampler.s"
	@echo "... src/base/shader.o"
	@echo "... src/base/shader.i"
	@echo "... src/base/shader.s"
	@echo "... src/base/synchronizer.o"
	@echo "... src/base/synchronizer.i"
	@echo "... src/base/synchronizer.s"
	@echo "... src/core/commandBufferRecorder.o"
	@echo "... src/core/commandBufferRecorder.i"
	@echo "... src/core/commandBufferRecorder.s"
	@echo "... src/core/datatype.o"
	@echo "... src/core/datatype.i"
	@echo "... src/core/datatype.s"
	@echo "... src/core/mesh.o"
	@echo "... src/core/mesh.i"
	@echo "... src/core/mesh.s"
	@echo "... src/engine/compute.o"
	@echo "... src/engine/compute.i"
	@echo "... src/engine/compute.s"
	@echo "... src/engine/deferred.o"
	@echo "... src/engine/deferred.i"
	@echo "... src/engine/deferred.s"
	@echo "... src/engine/engine.o"
	@echo "... src/engine/engine.i"
	@echo "... src/engine/engine.s"
	@echo "... src/engine/forward.o"
	@echo "... src/engine/forward.i"
	@echo "... src/engine/forward.s"
	@echo "... src/uppexo.o"
	@echo "... src/uppexo.i"
	@echo "... src/uppexo.s"
	@echo "... src/utils/device_util.o"
	@echo "... src/utils/device_util.i"
	@echo "... src/utils/device_util.s"
	@echo "... src/utils/log.o"
	@echo "... src/utils/log.i"
	@echo "... src/utils/log.s"
	@echo "... src/utils/vulkan_util.o"
	@echo "... src/utils/vulkan_util.i"
	@echo "... src/utils/vulkan_util.s"
	@echo "... tests/forward_rendering.o"
	@echo "... tests/forward_rendering.i"
	@echo "... tests/forward_rendering.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

