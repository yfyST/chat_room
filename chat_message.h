#include <boost/asio>
#include <string>

namespace chat {
using boost::asio::ip::tcp;
const uint32_t header_length = 4;
const uint32_t max_body_length = 512;
class chat_message {
 public:
  chat_message();
  std::string data() {
    return data_;
  }
  const std::string data() const {
    return data_;
  }
  uint32_t length() {
    return data_.size();
  }
  const std::string body() const {
    return data_.substr(header_length, -1);
  }
  std::string body() {
    return data_.substr(header_length, -1);
  }

  bool decode_header();
  void encode_header();
  void set_body_length(uint32_t newlength) {
    body_length_ = newlength > max_body_length ? max_body_length : newlength;
  }

 private:
  uint32_t body_length_;
  std::string data_;
};
}  // namespace chat
