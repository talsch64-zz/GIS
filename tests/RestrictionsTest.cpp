
#include <gtest/gtest.h>
#include "../Restrictions.h"


TEST(RestrictionsTest, basicRestrictions) {
    Restrictions r("aa, bb cc ,");
    std::vector<std::string> restrictions = r.getRestrictions();
    EXPECT_EQ(restrictions.size(), 3);
    EXPECT_NE(std::find(restrictions.begin(), restrictions.end(), "aa"), restrictions.end());
    EXPECT_NE(std::find(restrictions.begin(), restrictions.end(), "bb"), restrictions.end());
    EXPECT_NE(std::find(restrictions.begin(), restrictions.end(), "cc"), restrictions.end());
}

TEST(RestrictionsTest, emptyRestrictions) {
    Restrictions r1(", , ,  ");
    Restrictions r2("");
    std::vector<std::string> restrictions = r1.getRestrictions();
    EXPECT_EQ(restrictions.size(), 0);
    restrictions = r2.getRestrictions();
    EXPECT_EQ(restrictions.size(), 0);
}