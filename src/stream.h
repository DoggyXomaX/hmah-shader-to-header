#ifndef __STREAM_H__
#define __STREAM_H__

#include <stdio.h>
#include <stdbool.h>

enum StreamType {
  ST_VERTEX,
  ST_FRAGMENT,
  ST_HEADER,
  ST_SOURCE,
  MAX_STREAM_COUNT,
};

typedef struct Stream_s {
  FILE* target;
  size_t fileSize;
} Stream;

extern Stream g_Streams[MAX_STREAM_COUNT];

void Stream_Open(Stream* this, const char* path, const char* type);
bool Stream_IsOpened(const Stream* this);
void Stream_CloseAll(void);
void Stream_Close(Stream* this);

#endif // __STREAM_H__