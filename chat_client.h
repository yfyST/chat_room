#include <deque>

#include "chat_message.h"

namespace chat {
typedef std::deque<chat_message> message_deque;
namespace client {
class chat_client {
 public:
  chat_client(boost::asio::io_service &io_service,
              tcp::resolver::iterator endpoint_it);
  void write(const chat_message &msg);
  void close();

 private:
  void do_write();
  void do_read_header();
  void do_read_body();
  void do_connect(tcp::resolver::iterator endpoint_it);

  boost::asio::io_service &ip_service_;
  tcp::socket socket_;
  chat_message read_;
  message_deque write_;
};
}  // namespace client
}  // namespace chat
