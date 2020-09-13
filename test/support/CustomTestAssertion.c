#include "CustomTestAssertion.h"

char *createMessage(char *message, ...) {
	size_t len;
	char *buffer;

	va_list va;
	va_start(va, message);
	len = vsnprintf(NULL, 0, message, va);
	buffer = malloc(len + 1);
	vsprintf(buffer, message, va);
	va_end(va);

	return buffer;
}

void testAssertEqualBinFile(uint8_t *expectedBinArr, char *filename, int length, int lineNum) {
  FILE *fptr;
  char *msg;
  uint8_t actualBinArr[length];

  if((fptr = fopen(filename, "rb")) == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }

  fread(actualBinArr, sizeof(uint8_t), length, fptr);

  for(int i = 0; i < length; i++) {
    if(expectedBinArr[i] == actualBinArr[i])
      continue;
    else{
      msg = createMessage("Expected HEX 0x%x at index %d, but is 0x%x", expectedBinArr[i], i, actualBinArr[i]);
      UNITY_TEST_FAIL(lineNum, msg);
    }
  }
  fclose(fptr);
}