#include <boost/asio>
#include <deque>
#include <set>

#include "chat_message.h"

namespace chat {
namespace server {
using boost::asio::tcp;
using boost::system::error_code;
const uint32_t max_msg_len = 100;
typedef std::deque<chat_message> message_deque;
class chat_member {
 public:
  virtual ~chat_member(){};
  virtual void deliver(const chat_message &msg) = 0;
};

class chat_room {
 public:
  void join(chat_member *people);
  void leave(chat_member *people);
  void deliver(const chat_message &msg);

 private:
  message_deque rec_msgs_;
  std::set<chat_member *> all_people_;
};

class chat_session : public chat_member {
 public:
  chat_session(tcp::socket isocket) : socket_(socket) {
  }
  void run();
  void deliver(const chat_message &msg);

 private:
  void read_header();
  void read_body();
  void write();

  tcp::socket socket_;
  chat_room room;
  chat_message read_;
  message_deque write_;
};
class chat_server {
 public:
  chat_server();

 private:
  void do_accept();
  tcp::socket socket_;
  tcp::accept accept_;
  chat_room room_;
};
}  // namespace server
}  // namespace chat