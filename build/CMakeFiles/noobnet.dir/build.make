# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_COMMAND = /snap/cmake/1210/bin/cmake

# The command to remove a file.
RM = /snap/cmake/1210/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hzycpp/Desktop/MyTemplate/noobnet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hzycpp/Desktop/MyTemplate/noobnet/build

# Include any dependencies generated for this target.
include CMakeFiles/noobnet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/noobnet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/noobnet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/noobnet.dir/flags.make

CMakeFiles/noobnet.dir/net/log.cc.o: CMakeFiles/noobnet.dir/flags.make
CMakeFiles/noobnet.dir/net/log.cc.o: /home/hzycpp/Desktop/MyTemplate/noobnet/net/log.cc
CMakeFiles/noobnet.dir/net/log.cc.o: CMakeFiles/noobnet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hzycpp/Desktop/MyTemplate/noobnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/noobnet.dir/net/log.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/noobnet.dir/net/log.cc.o -MF CMakeFiles/noobnet.dir/net/log.cc.o.d -o CMakeFiles/noobnet.dir/net/log.cc.o -c /home/hzycpp/Desktop/MyTemplate/noobnet/net/log.cc

CMakeFiles/noobnet.dir/net/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/noobnet.dir/net/log.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hzycpp/Desktop/MyTemplate/noobnet/net/log.cc > CMakeFiles/noobnet.dir/net/log.cc.i

CMakeFiles/noobnet.dir/net/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/noobnet.dir/net/log.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hzycpp/Desktop/MyTemplate/noobnet/net/log.cc -o CMakeFiles/noobnet.dir/net/log.cc.s

CMakeFiles/noobnet.dir/net/config.cc.o: CMakeFiles/noobnet.dir/flags.make
CMakeFiles/noobnet.dir/net/config.cc.o: /home/hzycpp/Desktop/MyTemplate/noobnet/net/config.cc
CMakeFiles/noobnet.dir/net/config.cc.o: CMakeFiles/noobnet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hzycpp/Desktop/MyTemplate/noobnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/noobnet.dir/net/config.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/noobnet.dir/net/config.cc.o -MF CMakeFiles/noobnet.dir/net/config.cc.o.d -o CMakeFiles/noobnet.dir/net/config.cc.o -c /home/hzycpp/Desktop/MyTemplate/noobnet/net/config.cc

CMakeFiles/noobnet.dir/net/config.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/noobnet.dir/net/config.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hzycpp/Desktop/MyTemplate/noobnet/net/config.cc > CMakeFiles/noobnet.dir/net/config.cc.i

CMakeFiles/noobnet.dir/net/config.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/noobnet.dir/net/config.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hzycpp/Desktop/MyTemplate/noobnet/net/config.cc -o CMakeFiles/noobnet.dir/net/config.cc.s

CMakeFiles/noobnet.dir/net/thread.cc.o: CMakeFiles/noobnet.dir/flags.make
CMakeFiles/noobnet.dir/net/thread.cc.o: /home/hzycpp/Desktop/MyTemplate/noobnet/net/thread.cc
CMakeFiles/noobnet.dir/net/thread.cc.o: CMakeFiles/noobnet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hzycpp/Desktop/MyTemplate/noobnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/noobnet.dir/net/thread.cc.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/noobnet.dir/net/thread.cc.o -MF CMakeFiles/noobnet.dir/net/thread.cc.o.d -o CMakeFiles/noobnet.dir/net/thread.cc.o -c /home/hzycpp/Desktop/MyTemplate/noobnet/net/thread.cc

CMakeFiles/noobnet.dir/net/thread.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/noobnet.dir/net/thread.cc.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hzycpp/Desktop/MyTemplate/noobnet/net/thread.cc > CMakeFiles/noobnet.dir/net/thread.cc.i

CMakeFiles/noobnet.dir/net/thread.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/noobnet.dir/net/thread.cc.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hzycpp/Desktop/MyTemplate/noobnet/net/thread.cc -o CMakeFiles/noobnet.dir/net/thread.cc.s

# Object files for target noobnet
noobnet_OBJECTS = \
"CMakeFiles/noobnet.dir/net/log.cc.o" \
"CMakeFiles/noobnet.dir/net/config.cc.o" \
"CMakeFiles/noobnet.dir/net/thread.cc.o"

# External object files for target noobnet
noobnet_EXTERNAL_OBJECTS =

/home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so: CMakeFiles/noobnet.dir/net/log.cc.o
/home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so: CMakeFiles/noobnet.dir/net/config.cc.o
/home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so: CMakeFiles/noobnet.dir/net/thread.cc.o
/home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so: CMakeFiles/noobnet.dir/build.make
/home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so: CMakeFiles/noobnet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hzycpp/Desktop/MyTemplate/noobnet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library /home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/noobnet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/noobnet.dir/build: /home/hzycpp/Desktop/MyTemplate/noobnet/lib/libnoobnet.so
.PHONY : CMakeFiles/noobnet.dir/build

CMakeFiles/noobnet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/noobnet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/noobnet.dir/clean

CMakeFiles/noobnet.dir/depend:
	cd /home/hzycpp/Desktop/MyTemplate/noobnet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hzycpp/Desktop/MyTemplate/noobnet /home/hzycpp/Desktop/MyTemplate/noobnet /home/hzycpp/Desktop/MyTemplate/noobnet/build /home/hzycpp/Desktop/MyTemplate/noobnet/build /home/hzycpp/Desktop/MyTemplate/noobnet/build/CMakeFiles/noobnet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/noobnet.dir/depend

