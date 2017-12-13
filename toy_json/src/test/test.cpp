#include <gtest/gtest.h>
#include <string>
#include "toyjson.h"

using std::string;

TEST(TestParser, ParserNull) {
  ToyValue toyValue("null");
  toyValue.toyParser();
  ToyValue::ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(toyType, ToyValue::ToyType::TOY_NULL);
}

TEST(TestParser, ParserTrue) {
  ToyValue toyValue("true");
  toyValue.toyParser();
  ToyValue::ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(toyType,ToyValue:: ToyType::TOY_TRUE);
}

TEST(TestParser, ParserFalse) {
  ToyValue toyValue("false");
  toyValue.toyParser();
  ToyValue::ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(toyType,ToyValue:: ToyType::TOY_FALSE);
}

TEST(TestParser, ParserExpectValue) {
  ToyValue toyValue("");
  toyValue.toyParser();
  ToyValue::ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(toyType,ToyValue:: ToyType::TOY_NULL);
}

TEST(TestParser, ParserInvalidValue) {
}

TEST(TestParser, ParserRootNotSingular) {
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
