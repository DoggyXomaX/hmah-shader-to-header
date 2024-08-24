# hmah-shader-to-header

It takes the vertex and fragment shader files and converts them into a header file (.h) and a source file (.c).

## Downloads

* [v1.0.0]()

## Usage

```shell
> hsth --help
Common usage: hsth -n <shader_name>

	-n --name - Shader name
	-s --shaders - Shaders path
	-o --output - Output dir
	-m --header-output - Header output override
	-i --include - Source header include path override
	-g --guard - Header guard override
	-h --help - Show this help
```

## Build

![CMake 3.14^](https://img.shields.io/badge/CMake-3.14^-green)

```shell
cd /path/to/hmah-shader-to-header
mkdir build
cd build
cmake ..

# Windows
cmake --build . --config Release

# Linux
cmake --build .
```

### Options

`HMAH_MAX_STRING_SIZE` (default: 65535) - Max string size for the source/header file

## Example

Directory files: `image.vs`, `image.fs`

```shell
hsth -n image
```

Will generate: `image.h`, `image.c`

**image.h**
```c++
#ifndef __HSTH_IMAGE_H__
#define __HSTH_IMAGE_H__

extern const char* image_vs;
extern const char* image_fs;

#endif // __HSTH_IMAGE_H__
```

**image.c**
```c++
#include "image.h"

const char* image_vs = ""
  "#version 330 core\n"
  "\n"
  "void main() {\n"
  "    gl_Position = vec4(0, 0, 0, 1);\n"
  "}";

const char* image_fs = ""
  "#version 330 core\n"
  "\n"
  "void main() {\n"
  "    gl_FragColor = vec4(0, 0, 0, 1);\n"
  "}";
```

## License

MIT