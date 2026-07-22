#pragma once

#include <stdio.h>
#include <time.h>

#define PICO_RESET "\x1b[0m"

#define PICO_RED   "\x1b[31m"
#define PICO_GREEN "\x1b[32m"
#define PICO_YELLOW "\x1b[33m"
#define PICO_BLUE  "\x1b[34m"
#define PICO_DARK_RED "\033[38;2;180;40;40m"
#define PICO_DARK_GREEN "\033[38;2;40;150;40m"

#define PICO_BOLD "\x1b[1m"
#define PICO_UNDERLINE "\x1b[4m"
#define PICO_ITALIC "\033[3m"
#define PICO_DIM "\033[2m"

struct TEST_DATA {
    const char* name;
    int successCounter, failCounter;
};

#define TEST(Name) void test_##Name(struct TEST_DATA *_testData)

struct SECTION_DATA {
    bool running, success, abort;
    int successCount, failCount;
    const char* name;
};

static inline struct SECTION_DATA _StartSection(const char* name) {
    printf( PICO_RESET "  Starting section: %s\n", name);

    return (struct SECTION_DATA) {true, true, false, 0, 0, name};
}

static inline void _EndSection(const char* name, struct SECTION_DATA *sectionData, struct TEST_DATA *testData) {
    if (sectionData->success)
        printf(PICO_RESET PICO_GREEN "  Section '%s' passed successfully\n", name);
    else printf(PICO_RESET PICO_RED "  Section '%s' : %d passed, %d failed.\n", name, sectionData->successCount, sectionData->failCount);

    testData->failCounter += sectionData->failCount;
    testData->successCounter += sectionData->successCount;

    sectionData->running = false;
}

#define SECTION(Name) for ( \
    struct SECTION_DATA _sectionData = _StartSection((Name));_sectionData.running;_EndSection((Name), &_sectionData, _testData)\
)

#define ASSERT(Expression, ...) do { if(_sectionData.abort) break; if (!(Expression)) {_sectionData.success = false;_sectionData.failCount++;\
    printf(PICO_RESET PICO_DARK_RED"    ✗ Assert #%d '%s'    " PICO_ITALIC __VA_ARGS__ "\n      at %s:%d\n", _sectionData.failCount + _sectionData.successCount, #Expression, __FILE__, __LINE__);} else {_sectionData.successCount++;printf(PICO_RESET PICO_DARK_GREEN"    ✓ Assert #%d '%s'    " PICO_ITALIC __VA_ARGS__ "\n", _sectionData.failCount + _sectionData.successCount, #Expression);}\
    }while(0)

#define REQUIRE(Expression, ...) do { if(_sectionData.abort) break; if (!(Expression)) {_sectionData.abort=true;_sectionData.success = false;_sectionData.failCount++;\
    printf(PICO_DARK_RED PICO_BOLD"    ✗ Requirement #%d '%s'    " PICO_ITALIC __VA_ARGS__ "\n      at %s:%d\n", _sectionData.failCount + _sectionData.successCount, #Expression, __FILE__, __LINE__);} else {_sectionData.successCount++;printf(PICO_DARK_GREEN PICO_BOLD"    ✓ Requirement #%d '%s'    " PICO_ITALIC __VA_ARGS__ "\n", _sectionData.failCount + _sectionData.successCount, #Expression);}\
    }while(0)

#define RUN_TEST(Name) do { printf(PICO_BLUE PICO_BOLD"Running test '%s'\n", #Name);\
        struct TEST_DATA _testData = {#Name, 0, 0}; \
        clock_t _begin = clock();test_##Name(&_testData);clock_t _end = clock();double _delay=(double)(_end - _begin)*1000.0/CLOCKS_PER_SEC; \
        if(_testData.failCounter == 0) {printf(PICO_GREEN PICO_BOLD"Test '%s' successfully completed in %.2f ms..\n", #Name, _delay);}\
        else {printf(PICO_RED PICO_BOLD "Test '%s' completed : %d passed, %d failed in %.2f ms.\n", #Name, _testData.successCounter, _testData.failCounter, _delay);_testMainData.failCounter++;_testMainData.output=1;}\
    } while(0);

struct TEST_MAIN_DATA {
    int output, failCounter;
};

static inline struct TEST_MAIN_DATA _StartTestMain() {
    printf( PICO_RESET "Started running tests...\n");

    return (struct TEST_MAIN_DATA) {true, 0};
}

static inline int _EndTestMain(struct TEST_MAIN_DATA *mainData) {
    if (mainData->output)
            printf(PICO_RESET "Finished running tests with %d test fail(s).\n", mainData->failCounter);
    else    printf(PICO_RESET "Finished running tests successfully.\n");
    return mainData->output;
}

#define TESTS_START struct TEST_MAIN_DATA _testMainData = _StartTestMain();
#define TESTS_END return _EndTestMain(&_testMainData);