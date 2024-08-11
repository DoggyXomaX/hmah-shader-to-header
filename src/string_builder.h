#ifndef __STRING_BUILDER_H__
#define __STRING_BUILDER_H__

#include <stddef.h>

#ifndef MAX_STRING_SIZE
  #define MAX_STRING_SIZE 65535
#endif

typedef struct String_s {
  char data[MAX_STRING_SIZE];
  size_t length;
} String;

String String_Create(const char* text);

void String_Push(String* this, const char* text);
void String_Add_impl(String* this, const char* text, ...);
void String_AddChar(String* this, char c);
void String_ToUpper(String* this);
void String_ToLower(String* this);
void String_Clear(String* this);

void String_Print(const String* this);
void String_PrintLn(const String* this);

#define String_Add(this, ...) String_Add_impl((this), __VA_ARGS__, NULL)

#endif // __STRING_BUILDER_H__