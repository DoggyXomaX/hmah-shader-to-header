#include "input_args.h"

#include <stdio.h>
#include <string.h>

Arg g_Args[MAX_ARG_COUNT] = {
  { .shortName = "-n", .name = "--name", .description = "Shader name", .isRequired = true },
  { .shortName = "-s", .name = "--shaders", .description = "Shaders path", .isRequired = false },
  { .shortName = "-o", .name = "--output", .description = "Output dir", .isRequired = false },
  { .shortName = "-m", .name = "--header-output", .description = "Header output override", .isRequired = false },
  { .shortName = "-i", .name = "--include", .description = "Source header include path override", .isRequired = false },
  { .shortName = "-g", .name = "--guard", .description = "Header guard override", .isRequired = false },
};

int InputArgs_Get(const int argc, char* argv[]) {
  if (argc < 2) {
    InputArgs_PrintHelp();
    return 1;
  }

  // Check if help flag was provided in args
  for (int i = 1; i < argc; i++) {
    if (strcmp("-h", argv[i]) == 0 || strcmp("--help", argv[i]) == 0) {
      InputArgs_PrintHelp();
      return 1;
    }
  }

  // Reset args values to NULL
  for (int i = 0; i < MAX_ARG_COUNT; i++) {
    g_Args[i].value = NULL;
  }

  // Collect args
  for (int i = 1; i < argc; i += 2) {
    if (i + 1 >= argc) break;

    const char* name = argv[i];
    const char* value = argv[i + 1];

    for (int j = 0; j < MAX_ARG_COUNT; j++) {
      if (
        g_Args[j].shortName != NULL && strcmp(name, g_Args[j].shortName) == 0 ||
        g_Args[j].name != NULL && strcmp(name, g_Args[j].name) == 0
      ) {
        g_Args[j].value = value;
        break;
      }
    }
  }

  // Validate required args
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
  printf("Common usage: hsth -n <shader_name>\n\n");

  for (int i = 0; i < MAX_ARG_COUNT; i++) {
    const Arg* arg = &g_Args[i];
    if (arg->shortName == NULL && arg->name == NULL) continue;

    putchar('\t');

    if (arg->shortName != NULL) printf("%s", arg->shortName);

    if (arg->name) {
      if (arg->shortName != NULL) putchar(' ');
      printf("%s", arg->name);
    }

    if (arg->description) printf(" - %s", arg->description);

    putchar('\n');
  }

  printf("\t-h --help - Show this help\n");
}