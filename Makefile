# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/runner/work/note-c/note-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/runner/work/note-c/note-c/build

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target test
test:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running tests..."
	/usr/local/bin/ctest --force-new-ctest-process $(ARGS)
.PHONY : test

# Special rule for the target test
test/fast: test
.PHONY : test/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake cache editor..."
	/usr/local/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	cd /home/runner/work/note-c/note-c/build && $(CMAKE_COMMAND) -E cmake_progress_start /home/runner/work/note-c/note-c/build/CMakeFiles /home/runner/work/note-c/note-c/build/docs//CMakeFiles/progress.marks
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 docs/all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/runner/work/note-c/note-c/build/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 docs/clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 docs/preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 docs/preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	cd /home/runner/work/note-c/note-c/build && $(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

# Convenience name for target.
docs/CMakeFiles/docs.dir/rule:
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 docs/CMakeFiles/docs.dir/rule
.PHONY : docs/CMakeFiles/docs.dir/rule

# Convenience name for target.
docs: docs/CMakeFiles/docs.dir/rule
.PHONY : docs

# fast build rule for target.
docs/fast:
	cd /home/runner/work/note-c/note-c/build && $(MAKE) $(MAKESILENT) -f docs/CMakeFiles/docs.dir/build.make docs/CMakeFiles/docs.dir/build
.PHONY : docs/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... test"
	@echo "... docs"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	cd /home/runner/work/note-c/note-c/build && $(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

