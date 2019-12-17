// Copyright © 2020 Giorgio Audrito. All Rights Reserved.

#include <string>
#include <typeinfo>

#include "gtest/gtest.h"

#include "lib/device/multi_component.hpp"
#include "lib/device/data_component.hpp"


struct tag {};
struct gat {};
struct oth {};
struct hto {};

template <typename T>
struct expose_storage : public T {
    using T::T;
    using T::storage;
};

template <typename T>
using comp1 = fcpp::storage_component<fcpp::tagged_tuple<T,int>>;

using csmall = expose_storage<comp1<tag>>;

using comp3 = fcpp::multi_component<comp1<tag>, comp1<gat>, comp1<oth>>;

using comp4 = fcpp::storage_component<fcpp::tagged_tuple<hto,char>, comp3>;

using cbig = expose_storage<comp4>;


TEST(ComponentTest, Operators) {
    cbig  x;
    cbig y(x);
    cbig z(fcpp::make_tagged_tuple<tag,hto,void>(3,'v',2.5));
    char c;
    c = z.storage<hto>();
    EXPECT_EQ('v', c);
    z = y;
    y = x;
    z = std::move(y);
    EXPECT_EQ(x, z);
}

TEST(ComponentTest, Manager) {
    cbig::manager m;
    times_t t, inf = std::numeric_limits<times_t>::max();
    t = m.next();
    EXPECT_EQ(inf, t);
    t = m.update();
    EXPECT_EQ(inf, t);
    t = m.update();
    EXPECT_EQ(inf, t);
    std::string ex, res;
    ex  = typeid(fcpp::tagged_tuple<>).name();
    res = typeid(cbig::message_t).name();
    EXPECT_EQ(ex, res);
}

TEST(ComponentTest, Functions) {
    cbig y;
    cbig::manager n;
    y.round_start(n);
    cbig::message_t g = y.round_end(n);
    y.insert(n, g);
}