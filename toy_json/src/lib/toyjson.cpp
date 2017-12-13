#include "toyjson.h"

ToyValue::ToyValue(const std::string& _json):json_(_json) {
}

void ToyValue::toyParser() {
}

ToyValue::ToyType ToyValue::getToyType() const{
  return ToyType::TOY_NULL;
}
