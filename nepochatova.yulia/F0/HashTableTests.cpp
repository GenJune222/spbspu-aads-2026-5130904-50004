#define BOOST_TEST_MODULE HashTbleTests
#include <boost/test/included/unit_test.hpp>
#include <string>

#include "HashTable.h"

using namespace nepochatova;

struct BadHash {
  size_t operator()(const int &) const {
    return 1;
  }
};

BOOST_AUTO_TEST_SUITE(HashTableTests)

  BOOST_AUTO_TEST_CASE(Constructor_Default) {
    HashTable<int, std::string> table;

    BOOST_CHECK(table.empty());
    BOOST_CHECK_EQUAL(table.size(), 0);
  }


  BOOST_AUTO_TEST_CASE(Insert_Find) {
    HashTable<int, std::string> table;

    table.insert(1, "one");
    table.insert(2, "two");

    BOOST_CHECK_EQUAL(table.size(), 2);

    BOOST_CHECK(table.contains(1));
    BOOST_CHECK(table.contains(2));

    BOOST_CHECK_EQUAL(table.find(1), "one");
    BOOST_CHECK_EQUAL(table.find(2), "two");
  }


  BOOST_AUTO_TEST_CASE(Insert_ReplaceValue) {
    HashTable<int, std::string> table;

    table.insert(1, "old");
    table.insert(1, "new");

    BOOST_CHECK_EQUAL(table.size(), 1);
    BOOST_CHECK_EQUAL(table.find(1), "new");
  }


  BOOST_AUTO_TEST_CASE(Erase) {
    HashTable<int, std::string> table;

    table.insert(1, "one");
    table.insert(2, "two");

    auto value = table.erase(1);

    BOOST_CHECK_EQUAL(value, "one");

    BOOST_CHECK(!table.contains(1));
    BOOST_CHECK_EQUAL(table.size(), 1);
  }


  BOOST_AUTO_TEST_CASE(Erase_NotFound) {
    HashTable<int, std::string> table;

    BOOST_CHECK_THROW(
    table.erase(100),
    std::out_of_range
);
  }


  BOOST_AUTO_TEST_CASE(CopyConstructor) {
    HashTable<int, std::string> a;

    a.insert(1, "hello");

    HashTable b(a);

    BOOST_CHECK_EQUAL(b.size(), 1);
    BOOST_CHECK_EQUAL(b.find(1), "hello");

    b.insert(2, "world");

    BOOST_CHECK(!a.contains(2));
  }


  BOOST_AUTO_TEST_CASE(MoveConstructor) {
    HashTable<int, std::string> a;

    a.insert(1, "hello");

    HashTable<int, std::string> b(std::move(a));

    BOOST_CHECK_EQUAL(b.size(), 1);
    BOOST_CHECK(a.empty());
  }


  BOOST_AUTO_TEST_CASE(Collision_Test) {
    HashTable<int, std::string, BadHash> table;

    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    BOOST_CHECK_EQUAL(table.size(), 3);

    BOOST_CHECK_EQUAL(table.find(1), "one");
    BOOST_CHECK_EQUAL(table.find(2), "two");
    BOOST_CHECK_EQUAL(table.find(3), "three");
  }


  BOOST_AUTO_TEST_CASE(Iterator_Test) {
    HashTable<int, std::string> table;

    table.insert(1, "one");
    table.insert(2, "two");
    table.insert(3, "three");

    size_t count = 0;

    for (auto it = table.begin();
         it != table.end();
         ++it) {
      count++;
    }

    BOOST_CHECK_EQUAL(count, 3);
  }


  BOOST_AUTO_TEST_CASE(Iterator_Access) {
    HashTable<int, std::string> table;

    table.insert(1, "hello");
    auto it = table.begin();

    BOOST_CHECK_EQUAL(it->first, 1);
    BOOST_CHECK_EQUAL(it->second, "hello");
  }


  BOOST_AUTO_TEST_CASE(Clear) {
  HashTable<int, std::string> table;

    table.insert(1, "10");
    table.insert(2, "20");

    table.clear();

    BOOST_CHECK(table.empty());
    BOOST_CHECK_EQUAL(table.size(), 0);
  }

  BOOST_AUTO_TEST_CASE(Rehash) {
    HashTable<int, int> table(2);

    for (int i = 0; i < 100; i++) {
      table.insert(i, i * 10);
    }

    table.rehash(200);

    for (int i = 0; i < 100; i++) {
      BOOST_CHECK_EQUAL(
        table.find(i),
        i*10
      );
    }
  }

BOOST_AUTO_TEST_SUITE_END()
