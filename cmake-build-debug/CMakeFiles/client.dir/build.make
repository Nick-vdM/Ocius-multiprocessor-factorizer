# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /cygdrive/c/Users/nicol/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/nicol/AppData/Local/JetBrains/CLion2020.2/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/client.cpp.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/client.cpp.o: ../client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client.dir/client.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/client.cpp.o -c "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/client.cpp"

CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/client.cpp" > CMakeFiles/client.dir/client.cpp.i

CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/client.cpp" -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client.exe: CMakeFiles/client.dir/client.cpp.o
client.exe: CMakeFiles/client.dir/build.make
client.exe: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client.exe

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius" "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius" "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug" "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug" "/cygdrive/e/OneDrive - Griffith University/Second Year/2. Second Trimester/D. 2803ICT - Systems and Distributed Computing/Ocius/cmake-build-debug/CMakeFiles/client.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

