#include "unity.h"
#include "TrimWhiteSpacesIfPresent.h"
#include <stdlib.h>
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_trimWhiteSpacesIfPresent_given_a_string_with_no_leading_whitespaces_expect_string_remains_unchanged() {
  //set up the line to be trimmed
  char *stringToCreate ="hello, good morning";
  int len = strlen(stringToCreate);
  char *line = malloc(len + 1);
  strncpy(line, stringToCreate, len);
  line[len] = '\0';

  line = trimWhiteSpacesIfPresent(line);
  TEST_ASSERT_EQUAL_STRING("hello, good morning", line);
  free(line);
}

void test_trimWhiteSpacesIfPresent_given_a_string_with_leading_whitespaces_expect_whitespaces_are_trimmed() {
  //set up the line to be trimmed
  char *stringToCreate = "\t\t   mov A, r0\n";
  int len = strlen(stringToCreate);
  char *line = malloc(len + 1);
  strncpy(line, stringToCreate, len);
  line[len] = '\0';

  line = trimWhiteSpacesIfPresent(line);
  TEST_ASSERT_EQUAL_STRING("mov A, r0\n", line);
  free(line);
}

void test_trimWhiteSpacesIfPresent_given_a_string_with_whitespaces_only_expect_all_whitespaces_are_trimmed_and_terminates_with_NULL() {
  //set up the line to be trimmed
  char *stringToCreate = "\t\t\n";
  int len = strlen(stringToCreate);
  char *line = malloc(len + 1);
  strncpy(line, stringToCreate, len);
  line[len] = '\0';

  TEST_ASSERT_EQUAL_STRING("\t\t\n", line);   //before trimming
  line = trimWhiteSpacesIfPresent(line);
  TEST_ASSERT_EQUAL_PTR('\0', *line);         //after trimming, string terminates with NULL, no whitespaces left
  free(line);
}