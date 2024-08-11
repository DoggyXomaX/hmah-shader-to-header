# hmah-shader-to-header

It takes the vertex and fragment shader files and converts them into a header file (.h) and a source file (.c).

## Requirements

![CMake 3.14^](https://img.shields.io/badge/CMake-3.14^-green)

## Build

```shell
cd /path/to/hmah-shader-to-header
mkdir build
cd build
cmake -S .. -B .
cmake --build .
# *hsth appears*
```

### Options

`HMAH_MAX_STRING_SIZE` (default: 65535) - Max string size for the source/header file

## License

MIT