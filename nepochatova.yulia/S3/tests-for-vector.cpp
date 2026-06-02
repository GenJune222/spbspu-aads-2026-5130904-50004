#include <boost/test/unit_test.hpp>
#include <string>
#include "Vector.h"

using namespace nepochatova;

BOOST_AUTO_TEST_SUITE(VectorTests)

BOOST_AUTO_TEST_CASE(Constructor_Default) {
    Vector<int> v;
    BOOST_CHECK(v.isEmpty());
    BOOST_CHECK_EQUAL(v.getSize(), 0);
    BOOST_CHECK_EQUAL(v.getCapacity(), 0);
}

BOOST_AUTO_TEST_CASE(Constructor_Size) {
    Vector<int> v(5);
    BOOST_CHECK(!v.isEmpty());
    BOOST_CHECK_EQUAL(v.getSize(), 5);
    BOOST_CHECK_EQUAL(v.getCapacity(), 5);
    // new T[5] инициализирует примитивы нулями
    BOOST_CHECK_EQUAL(v[0], 0);
    BOOST_CHECK_EQUAL(v[4], 0);
}

BOOST_AUTO_TEST_CASE(Constructor_SizeValue) {
    Vector<std::string> v(3, "hello");
    BOOST_CHECK_EQUAL(v.getSize(), 3);
    BOOST_CHECK_EQUAL(v[0], "hello");
    BOOST_CHECK_EQUAL(v[1], "hello");
    BOOST_CHECK_EQUAL(v[2], "hello");
}

BOOST_AUTO_TEST_CASE(CopyConstructor) {
    Vector<int> original(3, 42);
    Vector<int> copy(original);
    
    BOOST_CHECK_EQUAL(copy.getSize(), 3);
    BOOST_CHECK_EQUAL(copy[0], 42);
    copy[0] = 100;
    BOOST_CHECK_EQUAL(original[0], 42);
    BOOST_CHECK_EQUAL(copy[0], 100);
}

BOOST_AUTO_TEST_CASE(CopyAssignment) {
    Vector<int> a(2, 1);
    Vector<int> b(3, 2);
    b = a;
    
    BOOST_CHECK_EQUAL(b.getSize(), 2);
    BOOST_CHECK_EQUAL(b[0], 1);
    BOOST_CHECK_EQUAL(b[1], 1);
}

BOOST_AUTO_TEST_CASE(MoveConstructor) {
    Vector<std::string> original(2, "test");
    size_t orig_cap = original.getCapacity();
    
    Vector<std::string> moved(std::move(original));
    
    BOOST_CHECK_EQUAL(moved.getSize(), 2);
    BOOST_CHECK_EQUAL(moved[0], "test");
    BOOST_CHECK(original.isEmpty());
}

BOOST_AUTO_TEST_CASE(MoveAssignment) {
    Vector<int> a(3, 10);
    Vector<int> b(1, 99);
    
    b = std::move(a);
    
    BOOST_CHECK_EQUAL(b.getSize(), 3);
    BOOST_CHECK_EQUAL(b[0], 10);
    BOOST_CHECK(a.isEmpty());
}

BOOST_AUTO_TEST_CASE(OperatorSubscript) {
    Vector<int> v(3);
    v[0] = 10;
    v[1] = 20;
    v[2] = 30;
    
    BOOST_CHECK_EQUAL(v[0], 10);
    BOOST_CHECK_EQUAL(v[1], 20);
    BOOST_CHECK_EQUAL(v[2], 30);
}

BOOST_AUTO_TEST_CASE(At_BoundsCheck) {
    Vector<int> v(2, 5);
    
    BOOST_CHECK_EQUAL(v.at(0), 5);
    BOOST_CHECK_EQUAL(v.at(1), 5);

    BOOST_CHECK_THROW(v.at(2), std::out_of_range);
    BOOST_CHECK_THROW(v.at(-1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(At_ConstVersion) {
    const Vector<int> v(2, 42);
    BOOST_CHECK_EQUAL(v.at(0), 42);
    BOOST_CHECK_THROW(v.at(5), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PushBack_Growth) {
    Vector<int> v;
    BOOST_CHECK_EQUAL(v.getCapacity(), 0);
    
    v.pushBack(1);
    BOOST_CHECK_EQUAL(v.getSize(), 1);
    BOOST_CHECK_EQUAL(v[0], 1);
    
    v.pushBack(2);
    v.pushBack(3);
    BOOST_CHECK_EQUAL(v.getSize(), 3);
    BOOST_CHECK_EQUAL(v[2], 3);

    size_t prev_cap = v.getCapacity();
    v.pushBack(4);
    BOOST_CHECK(v.getCapacity() > prev_cap);
}

BOOST_AUTO_TEST_CASE(PopBack_Simple) {
    Vector<int> v(3, 10);
    BOOST_CHECK_EQUAL(v.getSize(), 3);
    
    v.popBack();
    BOOST_CHECK_EQUAL(v.getSize(), 2);
    
    v.popBack();
    v.popBack();
    BOOST_CHECK(v.isEmpty());
}

BOOST_AUTO_TEST_CASE(Insert_Index) {
    Vector<int> v(3, 0);
    v.insert(1, 99);
    
    BOOST_CHECK_EQUAL(v.getSize(), 4);
    BOOST_CHECK_EQUAL(v[0], 0);
    BOOST_CHECK_EQUAL(v[1], 99);
    BOOST_CHECK_EQUAL(v[2], 0);
    BOOST_CHECK_EQUAL(v[3], 0);
}

BOOST_AUTO_TEST_CASE(Insert_Range) {
    Vector<int> src(2, 5);
    Vector<int> dst(2, 1);
    
    dst.insert(1, src, 0, 2);
    
    BOOST_CHECK_EQUAL(dst.getSize(), 4);
    BOOST_CHECK_EQUAL(dst[0], 1);
    BOOST_CHECK_EQUAL(dst[1], 5);
    BOOST_CHECK_EQUAL(dst[2], 5);
    BOOST_CHECK_EQUAL(dst[3], 1);
}

BOOST_AUTO_TEST_CASE(Erase_Index) {
    Vector<int> v(4, 10);
    v.erase(1);
    
    BOOST_CHECK_EQUAL(v.getSize(), 3);
    BOOST_CHECK_EQUAL(v[0], 10);
    BOOST_CHECK_EQUAL(v[1], 10);
    BOOST_CHECK_EQUAL(v[2], 10);
}

BOOST_AUTO_TEST_CASE(Erase_Range) {
    Vector<int> v(5, 1);
    v.erase(1, 4);
    
    BOOST_CHECK_EQUAL(v.getSize(), 2);
    BOOST_CHECK_EQUAL(v[0], 1);
    BOOST_CHECK_EQUAL(v[1], 1);
}

BOOST_AUTO_TEST_CASE(Iterator_Basic) {
    Vector<int> v(3, 42);
    
    auto it = v.begin();
    BOOST_CHECK_EQUAL(*it, 42);

    ++it;
    BOOST_CHECK_EQUAL(*it, 42);
    
    BOOST_CHECK(it != v.end());
    ++it;
    BOOST_CHECK(it == v.end());
}

BOOST_AUTO_TEST_CASE(Iterator_Mutation) {
    Vector<int> v(2, 10);
    
    for (auto it = v.begin(); it != v.end(); ++it) {
        *it *= 2;
    }
    
    BOOST_CHECK_EQUAL(v[0], 20);
    BOOST_CHECK_EQUAL(v[1], 20);
}

BOOST_AUTO_TEST_CASE(CIterator_ReadOnly) {
    const Vector<int> v(2, 100);
    
    auto cit = v.cbegin();
    BOOST_CHECK_EQUAL(*cit, 100);

}

BOOST_AUTO_TEST_CASE(Sort_Integers) {
    Vector<int> v(5);
    v[0] = 5; v[1] = 1; v[2] = 4; v[3] = 2; v[4] = 3;
    
    v.sort();
    
    BOOST_CHECK_EQUAL(v[0], 1);
    BOOST_CHECK_EQUAL(v[1], 2);
    BOOST_CHECK_EQUAL(v[2], 3);
    BOOST_CHECK_EQUAL(v[3], 4);
    BOOST_CHECK_EQUAL(v[4], 5);
}

BOOST_AUTO_TEST_CASE(Sort_Strings_Lexicographical) {
    Vector<std::string> v(4);
    v[0] = "dranichki";
    v[1] = "abrikoski";
    v[2] = "chiken";
    v[3] = "bublik";
    
    v.sort();

    BOOST_CHECK_EQUAL(v[0], "abrikoski");
    BOOST_CHECK_EQUAL(v[1], "bublik");
    BOOST_CHECK_EQUAL(v[2], "chiken");
    BOOST_CHECK_EQUAL(v[3], "dranichki");
}

BOOST_AUTO_TEST_CASE(Sort_EmptyOrSingle) {
    Vector<int> empty;
    empty.sort();
    BOOST_CHECK(empty.isEmpty());
    
    Vector<int> single(1, 42);
    single.sort();
    BOOST_CHECK_EQUAL(single[0], 42);
}


BOOST_AUTO_TEST_CASE(Insert_OutOfRange) {
    Vector<int> v(2);
    BOOST_CHECK_THROW(v.insert(3, 10), std::out_of_range);
    BOOST_CHECK_NO_THROW(v.insert(2, 10));
}

BOOST_AUTO_TEST_CASE(Erase_InvalidRange) {
    Vector<int> v(3);
    BOOST_CHECK_THROW(v.erase(5), std::out_of_range);
    BOOST_CHECK_THROW(v.erase(1, 5), std::out_of_range);
    BOOST_CHECK_THROW(v.erase(2, 1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Swap_Basic) {
    Vector<int> a(2, 1);
    Vector<int> b(3, 2);
    
    a.swap(b);
    
    BOOST_CHECK_EQUAL(a.getSize(), 3);
    BOOST_CHECK_EQUAL(a[0], 2);
    BOOST_CHECK_EQUAL(b.getSize(), 2);
    BOOST_CHECK_EQUAL(b[0], 1);
}

BOOST_AUTO_TEST_SUITE_END()
