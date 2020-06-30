#include "chat_message.h"

namespace chat {
chat_message::chat_message() : body_length_(0) {
  data_.reserve(header_length + max_body_length);
}
}  // namespace chat