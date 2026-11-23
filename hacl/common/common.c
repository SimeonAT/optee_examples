#include <common.h>
#include <stdio.h>

void hexdump(uint8_t *buffer, size_t size, const char* print_string)
{
  printf("%s\n", print_string);
  for (size_t i = 0; i < size; i++) {
    printf("0x%x ", buffer[i]);
  }
  printf("\n\n");
  return;
}