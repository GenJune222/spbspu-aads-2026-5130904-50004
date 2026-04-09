#include <boost/test/unit_test.hpp>
#include "stack.h"

BOOST_AUTO_TEST_SUITE(StackTests)

BOOST_AUTO_TEST_CASE(stack_push_and_drop) {
  nepochatova::Stack< int > stack;
  stack.push(10);
  stack.push(20);
  stack.push(30);
  BOOST_CHECK_EQUAL(stack.top(), 30);
  BOOST_CHECK_EQUAL(stack.drop(), 30);
  BOOST_CHECK_EQUAL(stack.top(), 20);
  BOOST_CHECK_EQUAL(stack.drop(), 20);
  BOOST_CHECK_EQUAL(stack.top(), 10);
  BOOST_CHECK_EQUAL(stack.drop(), 10);
  BOOST_CHECK(stack.empty());
}

BOOST_AUTO_TEST_CASE(stack_drop_empty) {
  nepochatova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(stack_top_empty) {
  nepochatova::Stack< int > stack;
  BOOST_CHECK_THROW(stack.top(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()