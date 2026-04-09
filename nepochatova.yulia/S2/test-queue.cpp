#include <boost/test/unit_test.hpp>
#include "queue.h"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_and_drop) {
  nepochatova::Queue< int > queue;
  queue.push(10);
  queue.push(20);
  queue.push(30);
  BOOST_CHECK_EQUAL(queue.front(), 10);
  BOOST_CHECK_EQUAL(queue.drop(), 10);
  BOOST_CHECK_EQUAL(queue.front(), 20);
  BOOST_CHECK_EQUAL(queue.drop(), 20);
  BOOST_CHECK_EQUAL(queue.front(), 30);
  BOOST_CHECK_EQUAL(queue.drop(), 30);
  BOOST_CHECK(queue.empty());
}

BOOST_AUTO_TEST_CASE(queue_drop_empty) {
  nepochatova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(queue_front_empty) {
  nepochatova::Queue< int > queue;
  BOOST_CHECK_THROW(queue.front(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()