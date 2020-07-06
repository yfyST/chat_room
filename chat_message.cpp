#include "chat_message.h"

namespace chat {
chat_message::chat_message() : body_length_(0) {
  data_.reserve(header_length + max_body_length);
}
bool chat_message::decode_header() {
  std::string head = data_.substr(0, header_length + 1);
  body_length_ = stoi(head);
  if (body_length_ > max_body_length) {
    return false;
  }
  return true;
}

void chat_message::encode_header() {
  std::string head = to_string(body_length_);
  while (head.size() < header_length) {
    head = "0" + head;
  }
  data_ = data_.replace(data_.begin(), data_.begin() + header_length, head);
}
}  // namespace chat
