#include "unity.h"
#include "mock_MemAlloc.h"
#include "LabelInfo.h"
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_createLabelInfo_through_mocking_memAlloc() {
  LabelInfo info = {"END", 5, 6};
  LabelInfo info1 = {"SKIP", 1, 2};
  LabelInfo *infoPtr;
  
  memAlloc_ExpectAndReturn(sizeof(LabelInfo), &info);
  infoPtr = createLabelInfo(&info1);
  TEST_ASSERT_EQUAL_PTR(&info, infoPtr);
  TEST_ASSERT_EQUAL_STRING("SKIP", infoPtr->name);
  TEST_ASSERT_EQUAL(1, infoPtr->indexNo);
  TEST_ASSERT_EQUAL(2, infoPtr->lineNo);
  
  memFree_Expect(infoPtr->name);
  memFree_Expect(infoPtr);
  freeLabelInfo(infoPtr);
}

void test_freeLabelInfo_through_mocking_memFree() {
  LabelInfo info = {"HERE", 5, 7};
  LabelInfo *infoPtr = &info;
  
  memFree_Expect(infoPtr->name);
  memFree_Expect(infoPtr);
  freeLabelInfo(infoPtr);
}

void test_freeLabelInfo_through_mocking_memFree_given_empty_label_name_expect_only_info_is_freed() {
  LabelInfo info = {NULL, 5, 7};
  LabelInfo *infoPtr = &info;

  memFree_Expect(infoPtr);
  freeLabelInfo(infoPtr);
}

void test_freeLabelInfo_given_empty_info_expect_memFree_not_called() {
  LabelInfo *infoPtr = NULL;
  freeLabelInfo(infoPtr);
}

void test_createLabelName_through_mocking_memAlloc() {
  char *label = "HELLO";
  char tempLabel[strlen(label) + 1];
  
  memAlloc_ExpectAndReturn(strlen(label) + 1, tempLabel);
  char *labelCreated = createLabelName(label);
  TEST_ASSERT_EQUAL_STRING("HELLO", labelCreated);
  TEST_ASSERT_EQUAL_PTR(tempLabel, labelCreated);
  
  memFree_Expect(labelCreated);
  freeLabelName(labelCreated);
}

void test_freeLabelName_through_mocking_memFree() {
  char *labelName = "DELAY";
  
  memFree_Expect(labelName);
  freeLabelName(labelName);
}

void test_freeLabelName_given_NULL_label_expect_memFree_not_called() {
  char *labelName = NULL;
  freeLabelName(labelName);
}