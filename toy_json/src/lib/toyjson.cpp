#include "toyjson.h"
#include <cmath>
#include <cstdlib>

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
  *json = jsonC;
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
    case '"':
      return parseString(json);
    default:
      return parseNumber(json);
  }
}

ToyValue::ErrorCode ToyValue::parseNumber(const char** json) {
  const char *jsonC = *json;
  char first = *jsonC, second = *(jsonC + 1);
  if ((first != '-' && !isdigit(first))
      || (first == '0' && (second == 'x' || second == 'X' || isdigit(second)))) 
      return TOY_PARSE_INVALID_VALUE;
  char *endPtr;
  errno = 0;
  num = strtod(jsonC, &endPtr);
  if ((*endPtr != '\0' && *endPtr != ' ') || *(endPtr - 1) == '.') return TOY_PARSE_INVALID_VALUE;
  if (errno == ERANGE && (num == HUGE_VAL || num == -HUGE_VAL))  return TOY_PARSE_NUM_OVERFLOW;
  
  *json = endPtr;
  toyType_ = ToyType::TOY_NUMBER;
  return TOY_PARSE_OK;
}

ToyValue::ErrorCode ToyValue::parseLiteral(const char** json, const char *str, ToyType toyType) {
  const char* jsonC = *json;
  while (*str != '\0'  && *jsonC == *str) {
    ++jsonC;
    ++str;
  }
  
  *json = jsonC;
  if (*str != '\0') return TOY_PARSE_INVALID_VALUE;
  toyType_ = toyType;
  return TOY_PARSE_OK;
}

double ToyValue::getToyNumber() const {
  return num;
}

string ToyValue::getToyString() const {
  return str;
}

ToyValue::ErrorCode ToyValue::parseString(const char** json) {
  const char* jsonC = *json;
  ++jsonC;
  string tmp;
  while (true) {
    char c = *jsonC++;
    switch(c) {
      case '"':
	*json = jsonC;
	new (&str) string(tmp);
	toyType_ = ToyType::TOY_STRING;
	return TOY_PARSE_OK;
      case '\\':
	switch(*jsonC++) { 
	  case '"':
	    tmp.push_back('"');
	    break;
	  case '\\':
	    tmp.push_back('\\');
	    break;
	  case '/':
	    tmp.push_back('/');
	    break;
	  case 'b':
	    tmp.push_back('\b');
	    break;
	  case 'f':
	    tmp.push_back('\f');
	    break;
	  case 'n':
	    tmp.push_back('\n');
	    break;
	  case 'r': 
	    tmp.push_back('\r');
	    break;
	  case 't':
	    tmp.push_back('\t');
	    break;
	  case 'u': {
	    auto jsonTmp = jsonC;
	    unsigned int n = 0;
	    unsigned int n1 = parseHex(&jsonC);
	    if (jsonTmp + 4 != jsonC) return TOY_PARSE_INVALID_UNICODE_HEX;
	    if (n1 >= 0xd800 && n1 <= 0xdbff) {
	      if (*jsonC != '\\' && *(jsonC + 1) != 'u') return TOY_PARSE_INVALID_UNICODE_SURROGATE;
	      jsonC += 2;
	      jsonTmp = jsonC;
	      unsigned int n2 = parseHex(&jsonC);
	      if (jsonTmp + 4 != jsonC) return TOY_PARSE_INVALID_UNICODE_HEX;
	      if (n2 >= 0xdc00 && n2 <= 0xdfff) {
		n = 0x10000 + (n1 - 0xd800) * 0x400 + (n2 - 0xdc00);
	      } else {
		return TOY_PARSE_INVALID_UNICODE_SURROGATE;
	      }
	    } else {
	      n = n1;
	    }
	    if (n >= 0x0000 && n <= 0x007f) {
	      tmp.push_back(n);
	    } else if (n >= 0x0080 && n <= 0x07ff) {
	      tmp.push_back(0xc0 | n >> 6 & 0xff);
	      tmp.push_back(0x80 | n & 0x3f);
	    } else if (n >= 0x0800 && n <= 0xffff) {
	      tmp.push_back(0xe0 | n >> 12 & 0xf);
	      tmp.push_back(0x80 | n >> 6 & 0x3f);
	      tmp.push_back(0x80 | n & 0x3f);
	    } else {
	      tmp.push_back(0xf0 | n >> 18 & 0xf);
	      tmp.push_back(0x80 | n >> 12 & 0x3f);
	      tmp.push_back(0x80 | n >> 6 &  0x3f);
	      tmp.push_back(0x80 | n & 0x3f);
	    }
	  }
	break;
	  default:
	    return TOY_PARSE_INVALID_STRING_ESCAPE;
	}
	break;
      default:
	if ((unsigned char)c <0x20) {
	  return TOY_PARSE_INVALID_STRING_CHAR;
	}
	tmp.push_back(c);
	break;

    }
  }
}

unsigned int ToyValue::parseHex(const char** json) {
  const char *jsonC = *json;
  unsigned int ans = 0;
  int i = 0;
  for (i = 0; *jsonC && i < 4; ++i) {
    ans *= 16;
    char c = *(jsonC + i);
    if (c >= '0' && c <='9') {
      ans += c - '0';
    } else if (c >= 'a' && c <= 'f') {
      ans += c - 'a' + 10;
    } else if (c >= 'A' && c <= 'F') {
      ans += c - 'A' + 10;
    } else{
      return -1;
    }
  }
  if (i == 4) {
    *json += 4;
    return ans;
  }
  return -1;
}
