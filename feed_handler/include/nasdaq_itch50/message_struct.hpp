#ifndef __FEEDHANDLER_NASDAQITCH50_MESSAGE_STRUCT_HPP__
#define __FEEDHANDLER_NASDAQITCH50_MESSAGE_STRUCT_HPP__

#include "boost/endian/conversion.hpp"
#include <cstdint>
#include <string>

namespace FeedHandler {
namespace NasdaqITCH50 {

struct __attribute__((packed)) SystemEventMessage {
  explicit SystemEventMessage(char *buffer) {
    ::memcpy(this, buffer, sizeof(SystemEventMessage));
    stock_locate = boost::endian::big_to_native(stock_locate);
    tracking_number = boost::endian::big_to_native(tracking_number);
    timestamp = boost::endian::big_to_native(timestamp) >> 16;
  }

  std::uint16_t stock_locate;
  std::uint16_t tracking_number;
  std::uint64_t timestamp : 48;
  char event_code;
};
static_assert(sizeof(SystemEventMessage) == 11,
              "System Event Message has wrong size");

struct __attribute__((packed)) AddOrderMessage {
  explicit AddOrderMessage(char *buffer) {
    ::memcpy(this, buffer, sizeof(AddOrderMessage));
    stock_locate = boost::endian::big_to_native(stock_locate);
    tracking_number = boost::endian::big_to_native(tracking_number);
    timestamp = boost::endian::big_to_native(timestamp) >> 16;
    order_ref_number = boost::endian::big_to_native(order_ref_number);
    shares = boost::endian::big_to_native(shares);
    price = boost::endian::big_to_native(price);
  }

  std::string symbol() noexcept { return std::string(stock_symbol, 8); }

  std::uint16_t stock_locate;
  std::uint16_t tracking_number;
  std::uint64_t timestamp : 48;
  std::uint64_t order_ref_number;
  bool is_buy_side;
  std::uint32_t shares;
  char stock_symbol[8];
  std::uint32_t price;
};
static_assert(sizeof(AddOrderMessage) == 35, "AddOrderMessage has wrong size");

}; // namespace NasdaqITCH50
}; // namespace FeedHandler

#endif // __FEEDHANDLER_NASDAQITCH50_MESSAGE_STRUCT_HPP__
