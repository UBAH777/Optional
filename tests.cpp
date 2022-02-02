#include "optional.h"
#include <gtest/gtest.h>

TEST(OptionalTests, Bad_Optional_Access) {
    BMSTU::Optional<int> a{};
    try {
        int n = a.value();
    } catch (const BMSTU::BadOptionalAccess &e) {
        ASSERT_STREQ("Bad optional access", e.what());
    }
}

TEST(OptionalTests, Optional_constructor_value) {
    BMSTU::Optional<int> a(12);
    ASSERT_EQ(a.value(), 12);
}

TEST(OptionalTests, Optional_constructor_another_optional) {
    BMSTU::Optional<int> b(12);
    BMSTU::Optional<int> a(b);
    ASSERT_EQ(a.value(), 12);

    BMSTU::Optional<int> c = b;
    ASSERT_EQ(c.value(), 12);

    BMSTU::Optional<int> d;
    BMSTU::Optional<int> e(d);
    ASSERT_FALSE(e.has_value());

    BMSTU::Optional<int> f(c);
    ASSERT_EQ(f.value(), 12);
}

TEST(OptionalTests, AssignmentOperator1) {
    BMSTU::Optional<int> a = 10;
    a = a;
    ASSERT_EQ(a.value(), 10);

    BMSTU::Optional<int> b;
    BMSTU::Optional<int> c = 10;
    b = c;
    ASSERT_EQ(b.value(), 10);

    BMSTU::Optional<int> d = 10;
    BMSTU::Optional<int> e;
    d = e;
    ASSERT_FALSE(d.has_value());

    BMSTU::Optional<int> f;
    BMSTU::Optional<int> g;
    f = g;
    ASSERT_FALSE(f.has_value());
}

TEST(OptionalTests, AssignmentOperator2) {
    BMSTU::Optional<int> a = 10;
    int x = 1;
    a = x;
    ASSERT_EQ(a.value(), 1);

    BMSTU::Optional<int> b;
    int y = 1;
    b = y;
    ASSERT_EQ(b.value(), 1);
}

BMSTU::Optional<int> create_with_value(int value) {
    BMSTU::Optional<int> a(value);
    return a;
}

BMSTU::Optional<int> create_empty() {
    BMSTU::Optional<int> a;
    return a;
}

TEST(OptionalTests, AssignmentOperator3) {
    BMSTU::Optional<int> a;
    a = create_with_value(1);
    ASSERT_EQ(a.value(), 1);

    BMSTU::Optional<int> b(1);
    b = create_empty();
    ASSERT_FALSE(b.has_value());

    BMSTU::Optional<int> c;
    c = create_empty();
    ASSERT_FALSE(c.has_value());

    BMSTU::Optional<int> d(4);
    d = create_with_value(1);
    ASSERT_EQ(d.value(), 1);
}

TEST(OptionalTests, Optional_has_value) {
    BMSTU::Optional<int> a;
    ASSERT_FALSE(a.has_value());
    a = 12;
    ASSERT_TRUE(a.has_value());
}

TEST(OptionalTests, Optional_reset) {
    BMSTU::Optional<int> a(12);
    ASSERT_TRUE(a.has_value());
    a.reset();
    ASSERT_FALSE(a.has_value());
}

TEST(OptionalTests, Optional_operator_equal_int) {
    BMSTU::Optional<int> a(12);
    a = 24;
    ASSERT_EQ(a.value(), 24);
}

TEST(OptionalTests, Optional_operator_equal_Optional) {
    BMSTU::Optional<int> a = 12;
    ASSERT_EQ(a.value(), 12);
    BMSTU::Optional<int> b(24);
    b = a;
    ASSERT_TRUE(b.value() == a.value());
    a = 12;
    a = b;
    ASSERT_TRUE(a.value() == b.value());
    BMSTU::Optional<int> c = 12;
    a = c;
    ASSERT_TRUE(a.value() == c.value());
    BMSTU::Optional<int> d(24);
    c = d;
    ASSERT_TRUE(c.value() == d.value());
}

TEST(OptionalTests, Optional_operator_ukaz) {
    BMSTU::Optional<int> a(12);
    ASSERT_EQ(*a, 12);
    ASSERT_EQ(*a = 14, 14);
}

TEST(OptionalTests, Optional_operator_ukazatel_ekzemp) {
    BMSTU::Optional<int> a(12);
    a.operator->();
    ASSERT_EQ(a.value(), 12);
    *a = 14;
    *a = 13;
    a.operator->();
    ASSERT_EQ(*a.operator->(), 13);
}

TEST(OptionalTests, Optional_value) {
    BMSTU::Optional<int> opt = {};

    try {
        int n = opt.value();
    } catch (const BMSTU::BadOptionalAccess &e) {
        ASSERT_STREQ("Bad optional access", e.what());
    }
    try {
        opt.value() = 42;
    } catch (const BMSTU::BadOptionalAccess &e) {
        ASSERT_STREQ("Bad optional access", e.what());
    }

    opt = 43;
    ASSERT_EQ(opt.value(), 43);

    opt.value() = 44;
    ASSERT_EQ(opt.value(), 44);
}

TEST(OptionalTests, Optional_swap) {
    BMSTU::Optional<int> a(12);
    BMSTU::Optional<int> b(24);
    swap(a, b);
    ASSERT_EQ(a.value(), 24);
    ASSERT_EQ(b.value(), 12);
}

TEST(OptionalTests, Optional_emplace) {
    BMSTU::Optional<int> a(12);
    a.Emplace(24);
    ASSERT_EQ(a.value(), 24);
}

TEST(OptionalTests, SparkOperator1) {
    const BMSTU::Optional<int> p(12);
    const int x = *p;
    ASSERT_EQ(x, 12);
}

TEST(OptionalTests, RValueSparkOperator) {
    ASSERT_EQ(*(BMSTU::Optional<int>(12)), 12);
}

TEST(OptionalTests, RValueValueOperator) {
    ASSERT_EQ((BMSTU::Optional<int>(12)).value(), 12);

    try {
        (BMSTU::Optional<int>()).value();
    } catch (const BMSTU::BadOptionalAccess &e) {
        ASSERT_STREQ("Bad optional access", e.what());
    }
}

TEST(OptionalTests, ArrowOperator) {
    const BMSTU::Optional<int> p(12);
    const int x = *p.operator->();
    ASSERT_EQ(x, 12);
}