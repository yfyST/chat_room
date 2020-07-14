#include "chat_message.h"

namespace chat {
chat_message::chat_message() : body_length_(0) {
  // data_.reserve(header_length + max_body_length);
}
bool chat_message::decode_header() {
  char header[header_length + 1] = "";
  std::strncat(header, data_, header_length);  // get header
  body_length_ = std::atoi(header);            // header is info of body_length
  if (body_length_ > max_body_length) {
    body_length_ = 0;
    return false;
  }
  return true;
}

void chat_message::encode_header() {
  char header[header_length + 1] = "";
  std::sprintf(header, "%4d",
               static_cast<int>(body_length_));  // 4 bytes header
  std::memcpy(data_, header, header_length);
}
}  // namespace chat
