#include "toyjson.h"

ToyValue::ErrorCode ToyValue::toyParse(const std::string& json) {
  const char* jsonC = json.c_str();
  skipWs(&jsonC);
  ErrorCode ec = ErrorCode::TOY_PARSE_OK;
  if ((ec = parse(&jsonC)) == TOY_PARSE_OK) {
    skipWs(&jsonC);
    if (*jsonC != '\0') {
      toyType_ = ToyType::TOY_NULL;
      return TOY_PARSE_ROOT_NOT_SINGULAR;
    }
      
  }
  return ec;
  
}

ToyValue::ToyType ToyValue::getToyType() const{
  return toyType_;
}

bool ToyValue::isWs(char c) {
  return (c == 0x20 || c == 0x09 || c == 0x0a || c == 0x0d);
}

void ToyValue::skipWs(const char** json) {
  const char* jsonC = *json;
  while (isWs(*jsonC)) ++jsonC;
  json = &jsonC;
}

ToyValue::ErrorCode ToyValue::parse(const char** json) {
  char c = **json;

  switch (c) {
    case 'n':
      return parseLiteral(json, "null", ToyType::TOY_NULL);
    case 't':
      return parseLiteral(json, "true", ToyType::TOY_TRUE);
    case 'f':
      return parseLiteral(json, "false", ToyType::TOY_FALSE);
    case '\0':
      return TOY_PARSE_EXPECT_VALUE;
    default:
      return TOY_PARSE_INVALID_VALUE;
  }
}

ToyValue::ErrorCode ToyValue::parseLiteral(const char** json, const char *str, ToyType toyType) {
  const char* jsonC = *json;
  while (*str != '\0'  && *jsonC++ == *str++);
  
  *json = jsonC;
  if (*str != '\0') return TOY_PARSE_INVALID_VALUE;
  toyType_ = toyType;
  return TOY_PARSE_OK;
}
