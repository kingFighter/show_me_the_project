#include <gtest/gtest.h>
#include <string>
#include "toyjson.h"

using std::string;

using ErrorCode = ToyValue::ErrorCode;
using ToyType = ToyValue::ToyType;

TEST(TestParser, ParserNull) {
  ToyValue toyValue("null");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_OK);
  EXPECT_EQ(toyType, ToyType::TOY_NULL);
}

TEST(TestParser, ParserTrue) {
  ToyValue toyValue("true");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_OK);
  EXPECT_EQ(toyType, ToyType::TOY_TRUE);
}

TEST(TestParser, ParserFalse) {
  ToyValue toyValue("false");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_OK);
  EXPECT_EQ(toyType,ToyType::TOY_FALSE);
}

TEST(TestParser, ParserExpectValue) {
  ToyValue toyValue("");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_EXPECT_VALUE);
  EXPECT_EQ(toyType, ToyType::TOY_NULL);
}

TEST(TestParser, ParserInvalidValue) {
  ToyValue toyValue("k");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_INVALID_VALUE);
  EXPECT_EQ(toyType, ToyType::TOY_NULL);
}

TEST(TestParser, ParserRootNotSingular) {
  ToyValue toyValue("false");
  ErrorCode er = toyValue.toyParse();
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(er, ErrorCode::TOY_PARSE_ROOT_NOT_SINGULAR);
  EXPECT_EQ(toyType, ToyType::TOY_NULL);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

