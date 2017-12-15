#ifndef TOY_JSON_H
#define TOY_JSON_H
#include <string>

class ToyValue{
 public:
  /// JSON support 7 types
  enum struct ToyType {TOY_NULL, TOY_TRUE, TOY_FALSE, TOY_STRING, TOY_ARRAY, TOY_OBJECT, TOY_NUMBER};
      
  /**
   * TOY_PARSE_OK : no error
   * TOY_PARSE_EXPECT_VALUE : Content of ToyValue is empty
   * TOY_PARSE_INVALID_VALUE : Content of ToyValue is not supported literals
   * TOY_PARSE_ROOT_NOT_SINGULAR : Multiple content of ToyValue exist
   */
  enum ErrorCode {TOY_PARSE_OK = 0, TOY_PARSE_EXPECT_VALUE, TOY_PARSE_INVALID_VALUE, TOY_PARSE_ROOT_NOT_SINGULAR};

  ErrorCode toyParse(const std::string&);
  ToyType getToyType() const;

 private:
  ToyType toyType_ = ToyType::TOY_NULL;
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
};

#endif
