# hmah-shader-to-header

It takes the vertex and fragment shader files and converts them into a header file (.h) and a source file (.c).

## Requirements

![CMake 3.14^](https://img.shields.io/badge/CMake-3.14^-green)

## Usage

> Note: The `shader_name` should be the same as the header and source filenames

```shell
./hsth -n shader_name -v vertex.vs -f fragment.fs -m shader_name.h -s shader_name.c
```

Will generate:

**shader_name.h**
```c++
#ifndef __SHADER_NAME_H__
#define __SHADER_NAME_H__

extern const char* shader_name_vs;
extern const char* shader_name_fs;

#endif // __SHADER_NAME_H__
```

**shader_name.c**
```c++
#include "shader_name.h"

const char* shader_name_vs = ""
  "#version 330 core\n"
  "\n"
  "void main() {\n"
  "    gl_Position = vec4(0, 0, 0, 1);\n"
  "}";

const char* shader_name_fs = ""
  "#version 330 core\n"
  "\n"
  "void main() {\n"
  "    gl_FragColor = vec4(0, 0, 0, 1);\n"
  "}";

```

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