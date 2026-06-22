#define BOOST_TEST_MODULE NodeTests

#include <boost/test/included/unit_test.hpp>
#include "Node.h"

using namespace nepochatova;


BOOST_AUTO_TEST_SUITE(NodeTests)


  BOOST_AUTO_TEST_CASE(Constructor) {
    Node root("html");

    BOOST_CHECK_EQUAL(root.getTag(), "html");
    BOOST_CHECK(root.getParent() == nullptr);
    BOOST_CHECK_EQUAL(root.size(), 1);
  }


  BOOST_AUTO_TEST_CASE(AddChild) {
    Node root("root");

    Node *child = root.addChild("child");

    BOOST_CHECK_EQUAL(root.getChildren().getSize(), 1);
    BOOST_CHECK_EQUAL(child->getTag(), "child");

    BOOST_CHECK(child->getParent() == &root);
    BOOST_CHECK_EQUAL(root.size(), 2);
  }


  BOOST_AUTO_TEST_CASE(AddMultipleChildren) {
    Node root("root");

    root.addChild("a");
    root.addChild("b");
    root.addChild("c");


    BOOST_CHECK_EQUAL(root.getChildren().getSize(), 3);
    BOOST_CHECK_EQUAL(root.size(), 4);
  }


  BOOST_AUTO_TEST_CASE(RemoveChild) {
    Node root("root");

    Node *child = root.addChild("child");

    root.removeChild(child);


    BOOST_CHECK_EQUAL(root.getChildren().getSize(), 0);
    BOOST_CHECK_EQUAL(root.size(), 1);
  }


  BOOST_AUTO_TEST_CASE(Attributes) {
    Node node("div");


    node.setAttribute("id", "main");
    node.setAttribute("class", "container");


    BOOST_CHECK(node.hasAttribute("id"));
    BOOST_CHECK(node.hasAttribute("class"));


    BOOST_CHECK_EQUAL(
      node.getAttribute("id"),
      "main"
    );


    node.removeAttribute("id");


    BOOST_CHECK(!node.hasAttribute("id"));
  }


  BOOST_AUTO_TEST_CASE(MoveTo) {
    Node root("root");

    Node *a = root.addChild("a");
    Node *b = root.addChild("b");


    a->moveTo(b);


    BOOST_CHECK(a->getParent() == b);

    BOOST_CHECK_EQUAL(
      root.getChildren().getSize(),
      1
    );

    BOOST_CHECK_EQUAL(
      b->getChildren().getSize(),
      1
    );
  }


  BOOST_AUTO_TEST_CASE(MoveToSameParent) {
    Node root("root");

    Node *child = root.addChild("child");


    child->moveTo(&root);


    BOOST_CHECK(child->getParent() == &root);
    BOOST_CHECK_EQUAL(root.getChildren().getSize(), 1);
  }


  BOOST_AUTO_TEST_CASE(MoveNullParent) {
    Node root("root");

    Node *child = root.addChild("child");


    BOOST_CHECK_THROW(
      child->moveTo(nullptr),
      std::invalid_argument
    );
  }


  BOOST_AUTO_TEST_CASE(CycleProtection) {
    Node root("root");

    Node *child = root.addChild("child");

    Node *grand = child->addChild("grand");


    BOOST_CHECK_THROW(
      root.moveTo(grand),
      std::invalid_argument
    );
  }


  BOOST_AUTO_TEST_CASE(Size) {
    Node root("root");


    Node *a = root.addChild("a");
    a->addChild("aa");

    root.addChild("b");


    BOOST_CHECK_EQUAL(root.size(), 4);
  }


  BOOST_AUTO_TEST_CASE(Depth) {
    Node root("root");


    Node *a = root.addChild("a");
    Node *b = a->addChild("b");


    b->addChild("c");


    BOOST_CHECK_EQUAL(root.depth(), 4);
  }


  BOOST_AUTO_TEST_CASE(FindByTag) {
    Node root("html");


    Node *body = root.addChild("body");

    body->addChild("div");
    body->addChild("div");


    Vector<Node *> result;


    root.findByTag("div", result);


    BOOST_CHECK_EQUAL(result.getSize(), 2);
  }


  BOOST_AUTO_TEST_CASE(FindById) {
    Node root("root");


    Node *a = root.addChild("div");


    a->setAttribute("id", "test");


    Vector<Node *> result;


    root.findById("test", result);


    BOOST_CHECK_EQUAL(result.getSize(), 1);
    BOOST_CHECK(result[0] == a);
  }


  BOOST_AUTO_TEST_CASE(FindByClass) {
    Node root("root");


    Node *a = root.addChild("div");
    Node *b = root.addChild("span");


    a->setAttribute("class", "box");
    b->setAttribute("class", "box");


    Vector<Node *> result;


    root.findByClass("box", result);


    BOOST_CHECK_EQUAL(result.getSize(), 2);
  }


  BOOST_AUTO_TEST_CASE(ChildrenAccess) {
    Node root("root");


    root.addChild("a");


    BOOST_CHECK_EQUAL(
      root.getChildren()[0]->getTag(),
      "a"
    );
  }


BOOST_AUTO_TEST_SUITE_END()
