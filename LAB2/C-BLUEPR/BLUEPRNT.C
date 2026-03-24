#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint16_t AX = 0;
bool is_large_unsigned = false;

int main(void) {
  printf("Enter a number: [-32669; 65535]\n");

  char buf[256];
  if (!fgets(buf, sizeof(buf), stdin))
    return 1;
  buf[strcspn(buf, "\r\n")] = 0;

  bool is_negative = false;
  int i = 0;

  if (buf[0] == '-') {
    is_negative = true;
    i = 1;
  }

  uint32_t temp_acc = 0;

  for (; buf[i] != '\0'; i++) {
    if (buf[i] < '0' || buf[i] > '9') {
      printf("Error: Invalid character in buffer!\n");
      return 1;
    }

    temp_acc = temp_acc * 10 + (buf[i] - '0');
    if (is_negative && temp_acc > 32669) {
      printf("Error: Underflow! Minimum allowed is -32669.\n");
      return 1;
    }
    if (temp_acc > 65535) {
      printf("Number too large. Please enter a number in the range [-32669; "
             "65535].\n");
      return 1;
    }
  }

  if (is_negative) {
    AX = (uint16_t)(-(int32_t)temp_acc);
  } else {
    AX = (uint16_t)temp_acc;

    if (temp_acc > 32767) {
      is_large_unsigned = true;
    }
  }

  AX -= 99;

  printf("Result: ");

  if (!is_large_unsigned && ((int16_t)AX < 0)) {
    printf("-");
    AX = (uint16_t)(-(int16_t)AX);
  }

  if (AX == 0) {
    printf("0\n");
    return 0;
  }

  char out_buf[10];
  int out_idx = 0;

  while (AX > 0) {
    out_buf[out_idx++] = (AX % 10) + '0';
    AX /= 10;
  }

  for (int j = out_idx - 1; j >= 0; j--) {
    printf("%c", out_buf[j]);
  }
  printf("\n");

  return 0;
}
