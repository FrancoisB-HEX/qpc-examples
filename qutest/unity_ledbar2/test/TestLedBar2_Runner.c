// AUTOGENERATED FILE. DO NOT EDIT.

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include "cmock.h"
#include "LedBar2.h"
#include <stdint.h>
#include "MockLed.h"
#include "MockSpi.h"

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_LedBar_0_percent_all_off(void);
extern void test_LedBar_100_percent_all_on(void);
extern void test_LedBar_200_percent_ASSERT(void);
extern void test_LedBar_19_percent_all_off(void);
extern void test_LedBar_20_percent_one_on(void);
extern void test_LedBar_50_percent_two_on_FLAWED(void);
extern void test_LedBar_99_percent_four_on_FLAWED(void);


//=======Mock Management=====
static void CMock_Init(void)
{
  MockLed_Init();
  MockSpi_Init();
}
static void CMock_Verify(void)
{
  MockLed_Verify();
  MockSpi_Verify();
}
static void CMock_Destroy(void)
{
  MockLed_Destroy();
  MockSpi_Destroy();
}

//=======Test Reset Options=====
void resetTest(void);
void resetTest(void)
{
  tearDown();
  CMock_Verify();
  CMock_Destroy();
  CMock_Init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  CMock_Verify();
}

//=======Test Runner Used To Run Each Test=====
static void run_test(UnityTestFunction func, const char* name, UNITY_LINE_TYPE line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    CMock_Init();
    if (TEST_PROTECT())
    {
        setUp();
        func();
    }
    if (TEST_PROTECT())
    {
        tearDown();
        CMock_Verify();
    }
    CMock_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

//=======MAIN=====
int main(void)
{
  UnityBegin("TestLedBar2.c");
  run_test(test_LedBar_0_percent_all_off, "test_LedBar_0_percent_all_off", 15);
  run_test(test_LedBar_100_percent_all_on, "test_LedBar_100_percent_all_on", 25);
  run_test(test_LedBar_200_percent_ASSERT, "test_LedBar_200_percent_ASSERT", 43);
  run_test(test_LedBar_19_percent_all_off, "test_LedBar_19_percent_all_off", 51);
  run_test(test_LedBar_20_percent_one_on, "test_LedBar_20_percent_one_on", 61);
  run_test(test_LedBar_50_percent_two_on_FLAWED, "test_LedBar_50_percent_two_on_FLAWED", 74);
  run_test(test_LedBar_99_percent_four_on_FLAWED, "test_LedBar_99_percent_four_on_FLAWED", 89);

  CMock_Guts_MemFreeFinal();
  return UnityEnd();
}
