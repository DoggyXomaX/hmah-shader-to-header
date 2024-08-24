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

    if (c == '\r') {
      continue;
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

  // Get shader name
  String name = String_Create(g_Args[AT_NAME].value);

  // Generate vertex and fragment paths
  String vertexPath;
  String fragmentPath;
  {
    String path = String_Create(g_Args[AT_SHADERS].value != NULL ? g_Args[AT_SHADERS].value : ".");
    if (path.data[path.length - 1] != PATH_SEPARATOR) {
      String_AddChar(&path, PATH_SEPARATOR);
    }
    String_PushString(&path, &name);
    vertexPath = String_From(&path);
    fragmentPath = String_From(&path);
  }
  String_Push(&vertexPath, ".vs");
  String_Push(&fragmentPath, ".fs");

  Stream_Open(&g_Streams[ST_VERTEX], vertexPath.data, "r");
  if (!Stream_IsOpened(&g_Streams[ST_VERTEX])) {
    fprintf(stderr, "Failed open vertex shader \"%s\" to read!\n", vertexPath.data);
    Stream_CloseAll();
    return 1;
  }

  Stream_Open(&g_Streams[ST_FRAGMENT], fragmentPath.data, "r");
  if (!Stream_IsOpened(&g_Streams[ST_FRAGMENT])) {
    fprintf(stderr, "Failed open fragment shader \"%s\" to read!\n", fragmentPath.data);
    Stream_CloseAll();
    return 1;
  }

  const String output = String_Create(g_Args[AT_OUTPUT].value != NULL ? g_Args[AT_OUTPUT].value: ".");
  const String headerOutput = String_Create(g_Args[AT_HEADER_OUTPUT].value != NULL ? g_Args[AT_HEADER_OUTPUT].value : output.data);

  String sourcePath = String_From(&output);
  if (sourcePath.data[sourcePath.length - 1] != PATH_SEPARATOR) {
    String_AddChar(&sourcePath, PATH_SEPARATOR);
  }
  String_PushString(&sourcePath, &name);
  String_Push(&sourcePath, ".c");

  String headerPath = String_From(&headerOutput);
  if (headerPath.data[headerPath.length - 1] != PATH_SEPARATOR) {
    String_AddChar(&headerPath, PATH_SEPARATOR);
  }
  String_PushString(&headerPath, &name);
  String_Push(&headerPath, ".h");

  Stream_Open(&g_Streams[ST_HEADER], headerPath.data, "w");
  if (!Stream_IsOpened(&g_Streams[ST_HEADER])) {
    fprintf(stderr, "Failed open \"%s\" output header file to write!\n", headerPath.data);
    Stream_CloseAll();
    return 1;
  }

  {
    String headerGuard;
    if (g_Args[AT_HEADER_GUARD].value == NULL) {
      headerGuard = String_Create(NULL);
      String_Add(&headerGuard, "__HSTH_", name.data, "_H__");
      String_ToUpper(&headerGuard);
    } else {
      headerGuard = String_Create(g_Args[AT_HEADER_GUARD].value);
    }

    String headerFile = String_Create(NULL);
    String_Add(&headerFile, "#ifndef ", headerGuard.data, "\n");
    String_Add(&headerFile, "#define ", headerGuard.data, "\n\n");

    String_Add(&headerFile, "extern const char* ", name.data, "_vs;\n");
    String_Add(&headerFile, "extern const char* ", name.data, "_fs;\n\n");

    String_Add(&headerFile, "#endif // ", headerGuard.data, "\n");

    // String_Print(&headerFile);
    fwrite(headerFile.data, 1, headerFile.length, g_Streams[ST_HEADER].target);
    Stream_Close(&g_Streams[ST_HEADER]);
  }

  Stream_Open(&g_Streams[ST_SOURCE], sourcePath.data, "w");
  if (!Stream_IsOpened(&g_Streams[ST_SOURCE])) {
    fprintf(stderr, "Failed open \"%s\" output source file to write!\n", sourcePath.data);
    Stream_CloseAll();
    return 1;
  }

  {
    String sourceFile = String_Create(NULL);

    String includePath;
    if (g_Args[AT_INCLUDE].value != NULL) {
      includePath = String_Create(g_Args[AT_INCLUDE].value);
    }
    else {
      includePath = String_Create(NULL);
      String_Add(&includePath, name.data, ".h");
    }

    String_Add(&sourceFile, "#include \"", includePath.data , "\"\n\n");

    String_Add(&sourceFile, "const char* ", name.data, "_vs = \"\"\n");

    AddStreamToString(&sourceFile, &g_Streams[ST_VERTEX]);

    String_AddChar(&sourceFile, '\n');

    String_Add(&sourceFile, "const char* ", name.data, "_fs = \"\"\n");

    AddStreamToString(&sourceFile, &g_Streams[ST_FRAGMENT]);
    // String_Print(&sourceFile);

    fwrite(sourceFile.data, 1, sourceFile.length, g_Streams[ST_SOURCE].target);
    Stream_Close(&g_Streams[ST_SOURCE]);
  }

  Stream_CloseAll();

  printf("Generated \"%s\" and \"%s\" files\n", headerPath.data, sourcePath.data);

  return 0;
}
