#include <thread>

#include "chat_client.h"

int main() {
  try {
    boost::asio::io_service my_io_service;
    tcp::resolver my_resolver(my_io_service);
    tcp::resolver::iterator ep_it = my_resolver.resolver({"localhost", "8080"});
    chat_client client(io_service, ep_it);

    std::thread t([&my_io_service]() {
      my_io_service.run();
    });
    std::string line;
    while (std::cin >> line) {
      chat_message msg;
      msg.set_body_length(line.size());
      msg.body() = line;
      msg.encode_header();
      client.write();
    }
    c.close();
    t.join();
  } catch (std::exception &e) {
    std::cout << e << std::endl;
  }
  return 0;
}
