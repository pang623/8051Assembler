#ifndef Common_H
#define Common_H

#ifdef TEST
# define INLINE
#else
# define INLINE   inline
#endif  // TEST

#define NEWLINE                     10

char *skipWhiteSpaces(char *str);
char *skipNonWhiteSpaces(char *str);
char *skipAlphanumerals(char *str);
char *createSubstring(char *str, int start, int len);

#endif // Common_H
