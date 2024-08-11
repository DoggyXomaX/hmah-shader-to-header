#include <math.h>
#include <stdio.h>

#include "input_args.h"
#include "stream.h"
#include "string_builder.h"

#ifdef _WIN32
  #define PATH_SEPARATOR '\\'
#else
  #define PATH_SEPARATOR '/'
#endif

void AddStreamToString(String* sourceFile, const Stream* stream) {
  int x = 0;
  while (1) {
    const char c = fgetc(stream->target);
    if (x == 0) String_Add(sourceFile, "  \"");
    x++;

    if (c == EOF) {
      String_Add(sourceFile, "\";\n");
      break;
    }

    if (c == '\"' || c == '\\') {
      String_AddChar(sourceFile, '\\');
    }
    else if (c == '\n') {
      const size_t prevPosition = ftell(stream->target);
      const char next = fgetc(stream->target);
      if (next == EOF) {
        String_Add(sourceFile, "\";\n");
        break;
      }

      String_Add(sourceFile, "\\n\"");
      x = 0;
      fseek(stream->target, prevPosition, SEEK_SET);
    }

    String_AddChar(sourceFile, c);
  }
}

int main(const int argc, char* argv[]) {
  const int argsResult = InputArgs_Get(argc, argv);
  if (argsResult != 0) {
    return argsResult;
  }

  const char* vertexPath = g_Args[AT_VERTEX].value;
  Stream_Open(&g_Streams[ST_VERTEX], vertexPath, "r");
  if (!Stream_IsOpened(&g_Streams[ST_VERTEX])) {
    fprintf(stderr, "Failed open vertex shader \"%s\" to read!\n", vertexPath);
    Stream_CloseAll();
    return 1;
  }

  const char* fragmentPath = g_Args[AT_FRAGMENT].value;
  Stream_Open(&g_Streams[ST_FRAGMENT], fragmentPath, "r");
  if (!Stream_IsOpened(&g_Streams[ST_FRAGMENT])) {
    fprintf(stderr, "Failed open fragment shader \"%s\" to read!\n", fragmentPath);
    Stream_CloseAll();
    return 1;
  }

  const char* headerPath = g_Args[AT_HEADER].value;
  Stream_Open(&g_Streams[ST_HEADER], headerPath, "w");
  if (!Stream_IsOpened(&g_Streams[ST_HEADER])) {
    fprintf(stderr, "Failed open \"%s\" output header file to write!\n", headerPath);
    Stream_CloseAll();
    return 1;
  }

  const char* name = g_Args[AT_NAME].value;

  {
    String headerGuard = String_Create(NULL);
    String_Add(&headerGuard, "__", name, "_H__");
    String_ToUpper(&headerGuard);

    String headerFile = String_Create(NULL);
    String_Add(&headerFile, "#ifndef ", headerGuard.data, "\n");
    String_Add(&headerFile, "#define ", headerGuard.data, "\n\n");

    String_Add(&headerFile, "extern const char* ", name, "_vs;\n");
    String_Add(&headerFile, "extern const char* ", name, "_fs;\n\n");

    String_Add(&headerFile, "#endif // ", headerGuard.data, "\n");
    // String_Print(&headerFile);

    fwrite(headerFile.data, 1, headerFile.length, g_Streams[ST_HEADER].target);
    Stream_Close(&g_Streams[ST_HEADER]);
  }

  const char* sourcePath = g_Args[AT_SOURCE].value;
  Stream_Open(&g_Streams[ST_SOURCE], sourcePath, "w");
  if (!Stream_IsOpened(&g_Streams[ST_SOURCE])) {
    fprintf(stderr, "Failed open \"%s\" output source file to write!\n", sourcePath);
    Stream_CloseAll();
    return 1;
  }

  {
    String sourceFile = String_Create(NULL);
    String_Add(&sourceFile, "#include \"", name , ".h\"\n\n");
    String_Add(&sourceFile, "const char* ", name, "_vs = \"\"\n");

    AddStreamToString(&sourceFile, &g_Streams[ST_VERTEX]);

    String_AddChar(&sourceFile, '\n');

    String_Add(&sourceFile, "const char* ", name, "_fs = \"\"\n");

    AddStreamToString(&sourceFile, &g_Streams[ST_FRAGMENT]);
    // String_Print(&sourceFile);

    fwrite(sourceFile.data, 1, sourceFile.length, g_Streams[ST_SOURCE].target);
    Stream_Close(&g_Streams[ST_SOURCE]);
  }

  Stream_CloseAll();

  printf("Generated \"%s\" and \"%s\" files\n", headerPath, sourcePath);

  return 0;
}
