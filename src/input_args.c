#include "input_args.h"

#include <stdio.h>
#include <string.h>

Arg g_Args[MAX_ARG_COUNT] = {
  { .shortName = "-n", .name = "--name", .description = "Name of the shader", .value = NULL, .hasValue = true, .isRequired = true },
  { .shortName = "-v", .name = "--vertex", .description = "Vertex shader path", .value = NULL, .hasValue = true, .isRequired = true },
  { .shortName = "-f", .name = "--fragment", .description = "Fragment shader path", .value = NULL, .hasValue = true, .isRequired = true },
  { .shortName = "-m", .name = "--header", .description = "Output header path", .value = NULL, .hasValue = true, .isRequired = true },
  { .shortName = "-s", .name = "--source", .description = "Output source path", .value = NULL, .hasValue = true, .isRequired = true },
  { .shortName = "-h", .name = "--help", .description = "Show this help", .value = NULL, .hasValue = false, .isRequired = false },
};

int InputArgs_Get(const int argc, char* argv[]) {
  if (argc < 2) {
    InputArgs_PrintHelp();
    return 1;
  }

  for (int i = 1; i < argc; i += 2) {
    if (i + 1 >= argc) break;

    const char* name = argv[i];
    const char* value = argv[i + 1];

    for (int j = 0; j < MAX_ARG_COUNT; j++) {
      if (!g_Args[j].hasValue) continue;
      if (
        g_Args[j].shortName != NULL && strcmp(name, g_Args[j].shortName) == 0 ||
        g_Args[j].name != NULL && strcmp(name, g_Args[j].name) == 0
      ) {
        g_Args[j].value = value;
        break;
      }
    }
  }

  for (int i = 0; i < MAX_ARG_COUNT; i++) {
    const Arg* arg = &g_Args[i];
    if (
      !arg->isRequired ||
      arg->value != NULL ||
      arg->shortName == NULL && arg->name == NULL
    ) {
      continue;
    }

    fprintf(stderr, "Missed required parameter ");

    if (arg->shortName != NULL) fprintf(stderr, "%s", arg->shortName);

    if (arg->name != NULL) {
      if (arg->shortName != NULL) fprintf(stderr, " ");
      fprintf(stderr, "%s", arg->name);
    }

    fprintf(stderr, "!\n");

    return 1;
  }

  return 0;
}

void InputArgs_PrintHelp() {
  printf("Usage: hsth -n name -v vertex.vs -f fragment.fs -m shader.h -s shader.c\n\n");

  for (int i = 0; i < MAX_ARG_COUNT; i++) {
    const Arg* arg = &g_Args[i];
    if (arg->shortName == NULL && arg->name == NULL) continue;

    putchar('\t');

    if (arg->shortName != NULL) printf("%s", arg->shortName);

    if (arg->name) {
      if (arg->shortName != NULL) putchar(' ');
      printf("%s", arg->name);
    }

    if (arg->description)  printf(" - %s", arg->description);

    putchar('\n');
  }
}