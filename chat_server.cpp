#include "chat_server.h"

namespace chat {
namespace server {
void chat_room::join(people) {
  all_people_.insert(people);
  for (auto &msg : rec_msgs_) {
    people->deliver(msg);
  }
}
void chat_room::leave(chat_member *people) {
  all_people_.erase(people);
}
void chat_room::deliver(const chat_message &msg) {
  rec_msgs_.push_back(msg);
  if (rec_msgs_.size() > max_msg_len) {
    rec_msgs_.pop_front();
  }
  for (auto human : all_people_) {
    human->deliver(msg);
  }
}

chat_session::chat_session(tcp::socket isocket, chat_room room)
    : socket_(std ::move(isocket)), room_(room) {
}

void chat_session::run() {
  room_.join(this);
  read_header();
}

void chat_session::deliver(const chat_message &msg) {
  if (write_.empty()) {
    write_.push_back(msg);
    write();
  } else {
    write_.push_back(msg);
  }
}
void chat_session::read_header() {
  boost::asio::async_read(socket_,
                          boost::asio::buffer(read_.data(), header_length),
                          [this](error_code er, uint32_t) {
                            if (!er && read_.decode_header()) {
                              read_body();
                            } else {
                              auto endpoint_it = socket_.remote_endpoint();
                              room_.leave(this);
                            }
                          });
}

void chat_session::read_body() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(read_.body(), read_.body_length()),
      [this](error_code er, uint32_t) {
        if (!er) {
          this->deliver(read_);
          read_header();
        } else {
          room_.leave(this);
        }
      });
}

void chat_session::write() {
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(write_.front().data(), write_.front().length()),
      [this](error_code er, uint32_t) {
        if (!er) {
          write_.pop_front();
          if (!write_.empty()) {
            write();
          }
        } else {
          room_.leave(this);
        }
      });
}

chat_server::chat_server(boost::asio::io_service &io_service,
                         tcp::endpoint &endpoint)
    : accept_(io_service, endpoint), socket_(io_service) {
  do_accept();
}

void chat_server::do_accept() {
  accept_.async_accept(socket_, [this](error_code er) {
    if (!er) {
      auto endpoint = socket_.remote_endpoint();
      chat_session newsession(std::move(socket_), room_);
      newsession.run();
    }
    do_accept();
  });
}
}  // namespace server
}  // namespace chat
