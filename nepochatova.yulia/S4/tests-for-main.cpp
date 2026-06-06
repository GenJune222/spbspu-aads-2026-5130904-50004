#define BOOST_TEST_MODULE BSTree_tests
#include <boost/test/included/unit_test.hpp>
#include "BSTree.h"
#include "BSTIters.h"

BOOST_AUTO_TEST_SUITE(BSTreeTests)

  BOOST_AUTO_TEST_CASE(test_push_and_get) {
    nepochatova::BSTree<int, std::string> tree;

    tree.push(5, "five");
    tree.push(3, "three");
    tree.push(7, "seven");

    BOOST_CHECK_EQUAL(tree.get(5), "five");
    BOOST_CHECK_EQUAL(tree.get(3), "three");
    BOOST_CHECK_EQUAL(tree.get(7), "seven");

    tree.push(5, "FIVE");
    BOOST_CHECK_EQUAL(tree.get(5), "FIVE");
  }

  BOOST_AUTO_TEST_CASE(test_get_throws) {
    nepochatova::BSTree<int, std::string> tree;
    tree.push(1, "one");

    BOOST_CHECK_THROW(tree.get(2), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(test_drop) {
    nepochatova::BSTree<int, std::string> tree;
    tree.push(5, "five");
    tree.push(3, "three");
    tree.push(7, "seven");

    BOOST_CHECK_EQUAL(tree.drop(3), "three");
    BOOST_CHECK_THROW(tree.get(3), std::out_of_range);

    BOOST_CHECK_EQUAL(tree.get(5), "five");
    BOOST_CHECK_EQUAL(tree.get(7), "seven");

    BOOST_CHECK_EQUAL(tree.drop(5), "five");
    BOOST_CHECK_THROW(tree.get(5), std::out_of_range);
    BOOST_CHECK_EQUAL(tree.get(7), "seven");
  }

  BOOST_AUTO_TEST_CASE(test_clear) {
    nepochatova::BSTree<int, std::string> tree;
    tree.push(1, "one");
    tree.push(2, "two");

    BOOST_CHECK(!tree.empty());
  }

  BOOST_AUTO_TEST_CASE(test_copy) {
    nepochatova::BSTree<int, std::string> tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    nepochatova::BSTree<int, std::string> tree2(tree1);

    BOOST_CHECK_EQUAL(tree2.get(1), "one");
    BOOST_CHECK_EQUAL(tree2.get(2), "two");

    tree1.push(3, "three");
    BOOST_CHECK_THROW(tree2.get(3), std::out_of_range);
  }

  BOOST_AUTO_TEST_CASE(test_move) {
    nepochatova::BSTree<int, std::string> tree1;
    tree1.push(1, "one");
    tree1.push(2, "two");

    nepochatova::BSTree<int, std::string> tree2(std::move(tree1));

    BOOST_CHECK_EQUAL(tree2.get(1), "one");
    BOOST_CHECK_EQUAL(tree2.get(2), "two");
  }

  BOOST_AUTO_TEST_CASE(test_height) {
    nepochatova::BSTree<int, std::string> tree;

    tree.push(5, "five");
    tree.push(3, "three");
    tree.push(7, "seven");
    tree.push(1, "one");
    tree.push(4, "four");

    BOOST_CHECK_EQUAL(tree.height(), 3);

    tree.push(0, "zero");
    BOOST_CHECK_EQUAL(tree.height(), 4);
  }

  BOOST_AUTO_TEST_CASE(test_iterators_inorder) {
    nepochatova::BSTree<int, std::string> tree;
    tree.push(3, "three");
    tree.push(1, "one");
    tree.push(4, "four");
    tree.push(2, "two");

    auto it = tree.cbegin();
    BOOST_CHECK_EQUAL((*it).first, 1);
    BOOST_CHECK_EQUAL((*it).second, "one");

    ++it;
    BOOST_CHECK_EQUAL((*it).first, 2);
    BOOST_CHECK_EQUAL((*it).second, "two");

    ++it;
    BOOST_CHECK_EQUAL((*it).first, 3);
    BOOST_CHECK_EQUAL((*it).second, "three");

    ++it;
    BOOST_CHECK_EQUAL((*it).first, 4);
    BOOST_CHECK_EQUAL((*it).second, "four");

    ++it;
    BOOST_CHECK(it == tree.cend());
  }

  BOOST_AUTO_TEST_CASE(test_rotate_stability) {
    nepochatova::BSTree<int, std::string> tree;
    tree.push(5, "five");
    tree.push(3, "three");
    tree.push(7, "seven");
    tree.push(1, "one");
    tree.push(4, "four");

    auto it = tree.cbegin();
    ++it;

    BOOST_CHECK_NO_THROW(tree.rotateLeft(it));

    BOOST_CHECK_EQUAL((*it).first, 3);
    BOOST_CHECK_EQUAL((*it).second, "three");
  }

BOOST_AUTO_TEST_SUITE_END()
