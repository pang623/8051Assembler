#include "TrimWhiteSpacesIfPresent.h"
#include <ctype.h>

void trimWhiteSpacesIfPresent(char *line) {
  int index = 0;
  while(isspace(line[index])) {
    index++;
  }
  if(index != 0) {
    int i = 0;
    while(line[i + index] != '\0') {
      line[i] = line[i + index];
      i++;
    }
   line[i] = '\0';
  }
}