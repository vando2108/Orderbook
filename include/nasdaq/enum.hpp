#ifndef __EXHANGE__NASDAQ__ENUM_HPP__
#define __EXHANGE__NASDAQ__ENUM_HPP__

#include <string_view>

namespace exchange {
namespace nasdaq {

enum class SystemEvent : char {
  StartOfMessages = 'O',
  StartOfSystemHours = 'S',
  StartOfMarketHours = 'Q',
  EndOfMarketHours = 'M',
  EndOfSystemHours = 'E',
  EndOfMessages = 'C'
};
const std::string_view system_event_to_string(const SystemEvent &);

enum class MessageType : char {
  SystemEvent = 'S',
  AddOrderMPID = 'A',
  AddorderMPID = 'F'
};

} // namespace nasdaq
} // namespace exchange

#endif // !__EXHANGE__NASDAQ__ENUM_HPP__
