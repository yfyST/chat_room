#include <iostream>

#include "chat_server.h"
using boost::asio::ip::tcp;
int main() {
  try {
    boost::asio::io_service my_io_service;
    tcp::endpoint ep(tcp::v4(), 8080);
    chat::server::chat_server myserver(my_io_service, ep);
    my_io_service.run();
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
