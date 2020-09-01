#include "unity.h"
#include "mock_MemAlloc.h"
#include "LabelInfo.h"

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
  
  memFree_Expect(infoPtr);
  freeLabelInfo(infoPtr);
}

void test_freeLabelInfo_through_mocking_memAlloc() {
  LabelInfo info = {"HERE", 5, 7};
  LabelInfo *infoPtr = &info;
  
  memFree_Expect(infoPtr);
  freeLabelInfo(infoPtr);
}

void test_freeLabelInfo_through_mocking_memAlloc_given_empty_info_expect_memFree_not_called() {
  LabelInfo *infoPtr = NULL;
  freeLabelInfo(infoPtr);
}