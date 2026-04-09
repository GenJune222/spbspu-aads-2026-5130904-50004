#include <boost/test/unit_test.hpp>
#include "operations.h"

BOOST_AUTO_TEST_SUITE(BitwiseOperationTests)

BOOST_AUTO_TEST_CASE(bitwise_not_basic) {
  BOOST_CHECK_EQUAL(nepochatova::bitwise_not(0), -1);
  BOOST_CHECK_EQUAL(nepochatova::bitwise_not(-1), 0);
  BOOST_CHECK_EQUAL(nepochatova::bitwise_not(5), -6);
  BOOST_CHECK_EQUAL(nepochatova::bitwise_not(1), -2);
  BOOST_CHECK_EQUAL(nepochatova::bitwise_not(-2), 1);
}

BOOST_AUTO_TEST_SUITE_END()
