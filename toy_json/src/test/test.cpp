#include <gtest/gtest.h>
#include <string>
#include "toyjson.h"

using std::string;

using ErrorCode = ToyValue::ErrorCode;
using ToyType = ToyValue::ToyType;

TEST(TestParse, ParseNull) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse("null");
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(ErrorCode::TOY_PARSE_OK, er);
  EXPECT_EQ(ToyType::TOY_NULL, toyType);
}

TEST(TestParse, ParseTrue) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse("true");
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(ErrorCode::TOY_PARSE_OK, er);
  EXPECT_EQ(ToyType::TOY_TRUE, toyType);
}

TEST(TestParse, ParseFalse) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse("false");
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(ErrorCode::TOY_PARSE_OK, er);
  EXPECT_EQ(ToyType::TOY_FALSE, toyType);
}

void testError(const string& str, ErrorCode ER = ErrorCode::TOY_PARSE_INVALID_VALUE) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse(str);
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(ER, er);
  EXPECT_EQ(ToyType::TOY_NULL, toyType);
}

TEST(TestParse, ParseExpectValue) {
  testError("", ErrorCode::TOY_PARSE_EXPECT_VALUE);
  testError("   ", ErrorCode::TOY_PARSE_EXPECT_VALUE);
}

TEST(TestParse, ParseInvalidValue) {
  testError("k");
  testError("nul");
  testError("?");
  
  testError("+12");
  testError("012");
  testError("+1");
  testError("+0");
  testError(".123");
  testError("1.");
  testError("INF");
  testError("inf");
  testError("NAN");
  testError("nan");

  testError("1e309", ToyValue::TOY_PARSE_NUM_OVERFLOW);
  testError("-1e309", ToyValue::TOY_PARSE_NUM_OVERFLOW);
}

TEST(TestParse, ParseRootNotSingular) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse("false true");
  ToyType toyType = toyValue.getToyType();
  EXPECT_EQ(ErrorCode::TOY_PARSE_ROOT_NOT_SINGULAR, er);
  EXPECT_EQ(ToyType::TOY_NULL, toyType);
}

void testNumber(double n, const string& sn) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse(sn);
  ToyType toyType = toyValue.getToyType();
  double num = toyValue.getToyNumber();
  EXPECT_EQ(ErrorCode::TOY_PARSE_OK, er);
  EXPECT_EQ(ToyType::TOY_NUMBER, toyType);
  EXPECT_EQ(n, num);
}

TEST(TestParse, ParseNumber) {
  testNumber(-1, "-1");
  testNumber(-0, "0");
  testNumber(-0.123, "-0.123");
  testNumber(-12.123, "-12.123");
  testNumber(-0.1432e12, "-0.1432e12");
  testNumber(-0.1432e-12, "-0.1432e-12");
  testNumber(-0.1432e+12, "-0.1432e+12");
  
  testNumber(1, "1");
  testNumber(0, "0");
  testNumber(0.123, "0.123");
  testNumber(12.123, "12.123");
  testNumber(0.1432e12, "0.1432e12");
  testNumber(0.1432e-12, "0.1432e-12");
  testNumber(0.1432e+12, "0.1432e+12");

  testNumber(0.0, "0");
  testNumber(0.0, "-0");
  testNumber(0.0, "-0.0");
  testNumber(1.0, "1");
  testNumber(-1.0, "-1");
  testNumber(1.5, "1.5");
  testNumber(-1.5, "-1.5");
  testNumber(3.1416, "3.1416");
  testNumber(1E10, "1E10");
  testNumber(1e10, "1e10");
  testNumber(1E+10, "1E+10");
  testNumber(1E-10, "1E-10");
  testNumber(-1E10, "-1E10");
  testNumber(-1e10, "-1e10");
  testNumber(-1E+10, "-1E+10");
  testNumber(-1E-10, "-1E-10");
  testNumber(1.234E+10, "1.234E+10");
  testNumber(1.234E-10, "1.234E-10");

  testNumber(1e-10000, "1e-10000");  /* underflow */
  
  testNumber(1.0000000000000002, "1.0000000000000002"); /* the
  							    smallest number > 1 */
  testNumber(4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum
  								       denormal */
  testNumber(-4.9406564584124654e-324, "-4.9406564584124654e-324");
  testNumber( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max
  									subnormal double */
  testNumber(-2.2250738585072009e-308, "-2.2250738585072009e-308");
  testNumber( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min
  									normal positive double */
  testNumber(-2.2250738585072014e-308, "-2.2250738585072014e-308");
  testNumber( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max
  									double */
  testNumber(-1.7976931348623157e+308, "-1.7976931348623157e+308");
}

void testString(string n, const string& sn) {
  ToyValue toyValue;
  ErrorCode er = toyValue.toyParse(sn);
  ToyType toyType = toyValue.getToyType();
  string str = toyValue.getToyString();
  EXPECT_EQ(ErrorCode::TOY_PARSE_OK, er);
  EXPECT_EQ(ToyType::TOY_STRING, toyType);
  EXPECT_EQ(n, str);
}

TEST(TestParse, ParseString) {
  testString("123", "\"123\"");
  testString("", "\"\"");
  testString("\"", "\"\\\"\"");
  testString("\\", "\"\\\\\"");
  testString("Hello", "\"Hello\"");
  testString("Hello\nWorld", "\"Hello\\nWorld\"");
  testString("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
  testString("\0abc", "\"\u0000abc\"");  
  testString("\x00", "\"\\u0000\""); 
  testString("\x24", "\"\\u0024\""); 
  testString("\x7f", "\"\\u007f\""); 
  testString("\xC2\x80", "\"\\u0080\""); 
  testString("\xC2\xA2", "\"\\u00A2\""); 
  testString("\xdf\xbf", "\"\\u07ff\""); 
  testString("\xE0\xA0\x80", "\"\\u0800\"");
  testString("\xE2\x82\xAC", "\"\\u20AC\""); 
  testString("\xEf\xbf\xbf", "\"\\uffff\""); 
  testString("\xF0\x90\x80\x80", "\"\\uD800\\uDc00\""); 
  testString("\xF4\x8f\xbf\xbf", "\"\\udbff\\udfff\"");  
}
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
