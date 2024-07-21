#include "string_builder.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

String String_Create(const char* text) {
  if (text == NULL) {
    return (String){ .length = 0, .data = { 0 }};
  }

  String string;

  string.length = strlen(text);
  strncpy(string.data, text, MAX_STRING_SIZE);

  return string;
}

void String_Push(String* this, const char* text) {
  if (text == NULL) return;

  const size_t textLength = strlen(text);
  const int maxCopyLength = MAX_STRING_SIZE - (int)this->length - 1;
  if (maxCopyLength <= 0) return;

  strncpy(this->data + this->length, text, maxCopyLength);

  this->length += maxCopyLength < textLength ? maxCopyLength : textLength;
}

void String_Add_impl(String* this, const char* text, ...) {
  va_list args;
  va_start(args, text);

  const char* path = text;
  while (path != NULL) {
    String_Push(this, path);
    path = va_arg(args, const char*);
  }

  va_end(args);
}

void String_AddChar(String* this, const char c) {
  const char s[2] = { c, 0 };
  String_Push(this, s);
}

void String_ToUpper(String* this) {
  for (size_t i = 0; i < this->length; i++) {
    this->data[i] = toupper(this->data[i]);
  }
}

void String_ToLower(String* this) {
  for (size_t i = 0; i < this->length; i++) {
    this->data[i] = tolower(this->data[i]);
  }
}

void String_Clear(String* this) {
  this->length = 0;
  this->data[0] = 0;
}

void String_Print(const String* this) {
  printf("%s", this->data);
}

void String_PrintLn(const String* this) {
  printf("%s\n", this->data);
}
