#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "List.h"

using namespace nepochatova;

BOOST_AUTO_TEST_CASE(empty)
{
    List<int> lst;
    BOOST_CHECK(lst.empty());
    lst.push_back(1);
    BOOST_CHECK(!lst.empty());
}

BOOST_AUTO_TEST_CASE(size)
{
    List<int> lst;
    BOOST_CHECK_EQUAL(lst.size(), 0);
    lst.push_back(5);
    lst.push_back(10);
    BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(front)
{
    List<int> lst;
    lst.push_back(15);
    lst.push_back(20);
    BOOST_CHECK_EQUAL(lst.front(), 15);
    lst.front() = 150;
    BOOST_CHECK_EQUAL(lst.front(), 150);
}

BOOST_AUTO_TEST_CASE(back)
{
    List<int> lst;
    lst.push_back(50);
    lst.push_back(75);
    BOOST_CHECK_EQUAL(lst.back(), 75);
    lst.back() = 881;
    BOOST_CHECK_EQUAL(lst.back(), 881);
}

BOOST_AUTO_TEST_CASE(push_front)
{
    List<int> lst;
    lst.push_front(3);
    lst.push_front(7);
    BOOST_CHECK_EQUAL(lst.front(), 7);
    BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(push_back)
{
    List<int> lst;
    lst.push_back(25);
    lst.push_back(35);
    BOOST_CHECK_EQUAL(lst.back(), 35);
    BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(insert)
{
    List<int> lst;
    lst.push_back(10);
    lst.push_back(30);
    auto it = lst.begin();
    ++it;
    lst.insert(it, 20);
    BOOST_CHECK_EQUAL(lst.size(), 3);
    BOOST_CHECK_EQUAL(*it, 20);
}

BOOST_AUTO_TEST_CASE(pop_front)
{
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.pop_front();
    BOOST_CHECK_EQUAL(lst.front(), 2);
    BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(pop_back)
{
    List<int> lst;
    lst.push_back(3);
    lst.push_back(4);
    lst.pop_back();
    BOOST_CHECK_EQUAL(lst.back(), 3);
    BOOST_CHECK_EQUAL(lst.size(), 1);
}

BOOST_AUTO_TEST_CASE(clear)
{
  List<int> lst;
  lst.push_back(1);
  lst.push_back(2);
  lst.push_back(3);
  lst.clear();
  BOOST_CHECK(lst.empty());
  BOOST_CHECK_EQUAL(lst.size(), 0);
}

BOOST_AUTO_TEST_CASE(erase)
{
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);
    auto it = lst.begin();
    ++it;
    lst.erase(it);
    BOOST_CHECK_EQUAL(lst.size(), 2);
    BOOST_CHECK_EQUAL(lst.front(), 1);
    BOOST_CHECK_EQUAL(lst.back(), 3);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    List<int> lst1;
    lst1.push_back(77);
    lst1.push_back(88);

    List<int> lst2(lst1);
    BOOST_CHECK_EQUAL(lst2.size(), 2);
    BOOST_CHECK_EQUAL(lst2.front(), 77);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
    List<int> lst1;
    lst1.push_back(42);

    List<int> lst2(std::move(lst1));
    BOOST_CHECK(lst1.empty());
    BOOST_CHECK_EQUAL(lst2.front(), 42);
}


