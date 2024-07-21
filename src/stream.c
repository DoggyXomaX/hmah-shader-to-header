#include "stream.h"

Stream g_Streams[MAX_STREAM_COUNT];

void Stream_Open(Stream* this, const char* path, const char* type) {
  this->target = fopen(path, type);
  if (this->target == NULL || type[0] != 'r') return;

  const int seekResult = fseek(this->target, 0, SEEK_END);
  if (seekResult != 0) {
    fclose(this->target);
    this->target = NULL;
    return;
  }

  const size_t fileSize = ftell(this->target);
  if (fileSize == -1L) {
    fclose(this->target);
    this->target = NULL;
    return;
  }

  fseek(this->target, 0, SEEK_SET);

  this->fileSize = fileSize;
}

bool Stream_IsOpened(const Stream* this) {
  return this->target != NULL;
}

void Stream_Close(Stream* this) {
  if (this->target == NULL) return;

  fclose(this->target);
  this->target = NULL;
}

void Stream_CloseAll() {
  for (int i = 0; i < MAX_STREAM_COUNT; i++) {
    Stream_Close(&g_Streams[i]);
  }
}
