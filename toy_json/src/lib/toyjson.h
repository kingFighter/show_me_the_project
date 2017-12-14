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

  ToyValue(const std::string&);
  ErrorCode toyParse();
  ToyType getToyType() const;

 private:
  ToyType toyType_ = ToyType::TOY_NULL;
  std::string json_;
};

#endif
