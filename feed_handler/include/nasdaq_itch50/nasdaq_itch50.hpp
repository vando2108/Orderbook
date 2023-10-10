#ifndef __FEEDHANDLER_NASDAQITCH50_HPP__
#define __FEEDHANDLER_NASDAQITCH50_HPP__

#include "../feed_handler.hpp"
#include "./message_struct.hpp"

#include <boost/endian/conversion.hpp>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <glog/logging.h>
#include <ios>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace FeedHandler {
namespace NasdaqITCH50 {
class NasdaqITCH50 : public IFeedHandler {
public:
  explicit NasdaqITCH50(msg_queue_t &queue) : IFeedHandler(queue) {}
  ~NasdaqITCH50() = default;

public:
  void handle_historical_data(std::string_view file_path) override {
    std::ifstream file(file_path.data(), std::ios::binary);
    assert(file.is_open());

    while (!file.eof()) {
      uint16_t msg_len;
      file.read(reinterpret_cast<char *>(&msg_len), sizeof(uint16_t));
      msg_len = boost::endian::big_to_native<uint16_t>(msg_len);

      char msg_type;
      file.read(&msg_type, 1);

      char *buffer = new char[msg_len - 1];
      file.read(buffer, msg_len - 1);

      emit(msg_type, buffer);
    }

    file.close();
  }
};
}; // namespace NasdaqITCH50
}; // namespace FeedHandler

#endif // __FEEDHANDLER_NASDAQITCH50_HPP__
