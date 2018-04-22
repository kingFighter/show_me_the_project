#ifndef TOY_JSON_H
#define TOY_JSON_H

#include <string>
using std::string;

class ToyValue{
 public:
  /// JSON support 7 types
  enum struct ToyType {TOY_NULL, TOY_TRUE, TOY_FALSE, TOY_STRING, TOY_ARRAY, TOY_OBJECT, TOY_NUMBER};
      
  /**
   * TOY_PARSE_OK : no error
   * TOY_PARSE_EXPECT_VALUE : Content of ToyValue is empty
   * TOY_PARSE_INVALID_VALUE : Content of ToyValue is not supported literals
   * TOY_PARSE_ROOT_NOT_SINGULAR : Multiple content of ToyValue exist
   * TOY_PARSE_NUM_OVERFLOW ： Content of ToyValue is overflow
   * TOY_PARSE_INVALID_STRING_CHAR : Content of ToyValue has invalid  char
   * TOY_PARSE_INVALID_STRING_ESCAPE : Content of ToyValue has invalid escape char
   * TOY_PARSE_INVALID_UNICODE_SURROGATE : Content of ToyValue has invalid unicode surrogate
   * TOY_PARSE_INVALID_UNICODE_HEX : Content of ToyValue has invalid unicode hex value
   */
  enum ErrorCode {TOY_PARSE_OK = 0, TOY_PARSE_EXPECT_VALUE, TOY_PARSE_INVALID_VALUE, TOY_PARSE_ROOT_NOT_SINGULAR, TOY_PARSE_NUM_OVERFLOW, 
		  TOY_PARSE_INVALID_STRING_CHAR, TOY_PARSE_INVALID_STRING_ESCAPE, TOY_PARSE_INVALID_UNICODE_SURROGATE, TOY_PARSE_INVALID_UNICODE_HEX};

  ErrorCode toyParse(const std::string&);
  ToyType getToyType() const;
  double getToyNumber() const;
  string getToyString() const;
  
  ToyValue():tok(DOU), num(0){}
  ~ToyValue() { if (tok== STR) str.~string();}
 private:
  ToyType toyType_ = ToyType::TOY_NULL;
  enum {DOU, STR} tok;
  union {
    double num;
    string str;
  };
  /**
   * skip ws
   * ws = *(
   *         %x20 /              ; Space
   *         %x09 /              ; Horizontal tab
   *         %x0A /              ; Line feed or New line
   *         %x0D )              ; Carriage return
   */
  void skipWs(const char**);
  /**
   * ws = *(
   *         %x20 /              ; Space
   *         %x09 /              ; Horizontal tab
   *         %x0A /              ; Line feed or New line
   *         %x0D )              ; Carriage return
   */
  bool isWs(char);
  ErrorCode parse(const char**);
  ErrorCode parseLiteral(const char**, const char*, ToyType);
  ErrorCode parseNumber(const char**);
  ErrorCode parseString(const char**);
  unsigned int parseHex(const char**);
};

#endif
