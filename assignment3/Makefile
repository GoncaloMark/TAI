.PHONY: all clean linux windows clean_linux clean_windows

all: linux windows

# Build for Linux (cleaning included)
linux: clean_linux
	mkdir -p build/linux
	cd build/linux && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/linux.toolchain.cmake && make

# Build for Windows (cleaning included)
windows: clean_windows
	mkdir -p build/windows
	cd build/windows && /opt/mxe/usr/bin/i686-w64-mingw32.static-cmake ../.. -DCMAKE_TOOLCHAIN_FILE=../../cmake/toolchains/windows.toolchain.cmake && make

# Clean all builds
clean: clean_linux clean_windows

# Clean Linux build
clean_linux:
	rm -rf build/linux bin
	mkdir -p bin

# Clean Windows build
clean_windows:
	rm -rf build/windows bin
	mkdir -p bin
