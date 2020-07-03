#include "chat_client.h"

namespace chat {
namespace client {
chat_client::chat_client(boost::asio::io_service &io_service,
                         tcp::resolver::iterator endpoint_it)
    : io_service_(io_service), socket_(io_service) {
  do_connect(endpoint_it);
}
void chat_client::do_connect(tcp::resolver::iterator endpoint_it) {
  boost::asio::async_connect(
      socket_, endpoint_it,
      [this](boost::system::error_code er, tcp::resolver::iterator) {
        if (!er) {
          do_read_header();
        }
      });
}
void chat_client::do_read_header() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(read_.data(), chat::header_length),
      [this](boost::system::error_code er, uint32_t) {
        if (!er && read_.decode_header()) {
          do_read_body();
        } else {
          socket_.close();
        }
      });
}

void chat_client::do_read_body() {
  boost::asio::async_read(
      socket_, boost::asio::buffer(read_.body(), read_.body_length_()),
      [this](boost::system::error_code er, uint32_t) {
        if (!er) {
          std::cout << write(read_.body(), read_.body_length_()) << std::endl;
          do_read_header();
        } else {
          socket_.close();
        }
      });
}

void chat_client::do_write() {
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(write_.front().data(), write_.front().length(),
                          [this](boost::system::error_code er, uint32_t) {
                            if (!er) {
                              write_.pop_front();
                              if (!write_.empty()) {
                                do_write();
                              }
                            } else {
                              socket_.close();
                            }
                          }))
}
void chat_client::close() {
  io_service.post([this]() {
    socket_.close();
  })
}
}  // namespace client
}  // namespace chat
