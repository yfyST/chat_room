#include <boost/asio.hpp>
#include <string>

namespace chat {
using boost::asio::ip::tcp;
const uint32_t header_length = 4;
const uint32_t max_body_length = 512;
class chat_message {
 public:
  chat_message();

  const char *data() const {
    return data_;
  }

  char *data() {
    return data_;
  }
  uint32_t length() const {
    return header_length + body_length_;
  }

  const char *body() const {
    return data_ + header_length;
  }
  char *body() {
    return data_ + header_length;
  }

  bool decode_header();
  void encode_header();
  void set_body_length(uint32_t newlength) {
    body_length_ = newlength > max_body_length ? max_body_length : newlength;
  }

  uint32_t body_length() {
    return body_length_;
  }

 private:
  uint32_t body_length_;
  char data_[header_length + max_body_length];
};
}  // namespace chat
