
#include <gtest/gtest.h>
#include "../Restrictions.h"
#include "../GISNamedTypes.h"


//TODO make it work! it works on main but for some reason it doesn't compile here

//TEST(RestrictionsTest, basicRestrictions) {
//    Restrictions r("aa, bb cc ,");
//    std::unordered_set<std::string> restrictions = r.getRestrictions();
//    EXPECT_EQ(restrictions.size(), 3);
//    EXPECT_NE(restrictions.find("aa"), restrictions.end());
//    EXPECT_NE(restrictions.find("bb"), restrictions.end());
//    EXPECT_NE(restrictions.find("cc"), restrictions.end());
//
//
//}

//TEST(RestrictionsTest, emptyRestrictions) {
//    Restrictions r1(", , ,  ");
//    Restrictions r2("");
//    std::unordered_set<std::string> restrictions = r1.getRestrictions();
//    EXPECT_EQ(restrictions.size(), 0);
//    restrictions = r2.getRestrictions();
//    EXPECT_EQ(restrictions.size(), 0);
//}