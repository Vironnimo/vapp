# Vapp

## Usage

### Build

- in root directory, to build as release:
- 1. cmake --preset release
- 2. cmake --build . --preset release-build
- 3. enjoy!
- for Debug build replace release with debug and release-build with debug-build
- if VCPKG_ROOT is not set you have to add the toolchain file to the first cmake command, for example:
- cmake --preset release -DCMAKE_TOOLCHAIN_FILE=C:\Development\Tools\vcpkg\scripts\buildsystems\vcpkg.cmake