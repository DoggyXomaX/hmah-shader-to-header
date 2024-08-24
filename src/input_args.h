#ifndef __INPUT_ARGS_H__
#define __INPUT_ARGS_H__

#include <stdbool.h>

enum ArgType {
  AT_NAME,
  AT_SHADERS,
  AT_OUTPUT,
  AT_HEADER_OUTPUT,
  AT_INCLUDE,
  AT_HEADER_GUARD,
  MAX_ARG_COUNT,
};

typedef struct Arg_s {
  const char* shortName;
  const char* name;
  const char* value;
  const char* description;
  bool isRequired;
} Arg;

extern Arg g_Args[MAX_ARG_COUNT];

int InputArgs_Get(int argc, char* argv[]);
void InputArgs_PrintHelp(void);

#endif // __INPUT_ARGS_H__