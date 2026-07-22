# PICOTEST - A lightweight, single header unit test framework.

Picotest is a tool i developped for my c projects in about an hour. It allows to create simple unit tests. 
This library has zero dependencies, appart for some headers of the standard library (stdio and time).

## Exemple

```c
#include <picotest.h>

TEST(MyTest) {

  // A section is a way to organize tests. it has access to the global test context.
  SECTION("A section doing stuff") {
    // You must put all your asserts and requirement into a section.

    // Asserts are conditions that must be satisfied for a section and a test to succeed.
    ASSERT(24 % 2 == 0);
    
    // Requirements are just like asserts, except that if they fail, they skip the rest of the section.
    REQUIRE(0 != 1);

    ASSERT(true, "This is a comment, it's written in the output next to the assert's result. it also works for requirements.");

  }

  SECTION("Other section") {
    // You can of course add other sections, but NOT nest them.
  }

}

int main(void) {
  // You must run tests in between TESTS_START and TESTS_END.
  TESTS_START
  RUN_TEST(MyTest)
  TESTS_END
  // TESTS_END returns the final output code for you, so you don't have to.
  // You can also put this testing block in a function returning it, and call it whenever and wherever you want.
}

```

## Roadmap

I don't have a specific roadmap for this project since it's more of a small tool i made quickly for the needs of another project.
There are some things i do think i'll include though, like the ability to print the output in a file, or an ASSERT_EQ macro which will
do the same as an assert testing an equality, except it will indicate the provided and expected value if it fails.
(There will probably also be a REQUIRE_EQ macro too if i do include ASSERT_EQ.)
