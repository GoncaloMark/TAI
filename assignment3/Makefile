.PHONY: all clean linux windows

all: linux windows

linux:
	mkdir -p build.linux
	cd build.linux && cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/linux.toolchain.cmake && make

windows:
	mkdir -p build.windows
	cd build.windows && /opt/mxe/usr/bin/i686-w64-mingw32.static-cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchains/windows.toolchain.cmake && make

clean:
	rm -rf build.linux build.windows bin
	mkdir -p bin

