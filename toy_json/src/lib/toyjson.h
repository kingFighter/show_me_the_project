#ifndef TOY_JSON_H
#define TOY_JSON_H
#include <string>

class ToyValue{
 public:
  enum struct ToyType {TOY_NULL, TOY_TRUE, TOY_FALSE, TOY_STRING, TOY_ARRAY, TOY_OBJECT, TOY_NUMBER};
  
  ToyValue(const std::string&);
  void toyParser();
  ToyType getToyType() const;

 private:
  enum {TOY_PARSER_OK = 0, TOY_PARSER_EXPECT_VALUE, TOY_PARSER_INVALID_VALUE, TOY_PARSER_ROOT_NOT_SINGULAR};
  ToyType toyType_ = ToyType::TOY_NULL;
  std::string json_;
};

#endif
