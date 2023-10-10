#include "../../include/nasdaq/enum.hpp"
#include "../../include/nasdaq/nasdaq.hpp"

#include <cassert>
#include <unordered_map>

namespace exchange {
namespace nasdaq {

const std::string_view system_event_to_string(const SystemEvent &e) {
  const std::unordered_map<SystemEvent, std::string_view> enum_strings = {
      {SystemEvent::StartOfMessages, "Start of Messages"},
      {SystemEvent::StartOfSystemHours, "Start of System Hours"},
      {SystemEvent::StartOfMarketHours, "Start of Market Hours"},
      {SystemEvent::EndOfMarketHours, "End of Market Hours"},
      {SystemEvent::EndOfSystemHours, "End of System Hours"},
      {SystemEvent::EndOfMessages, "End of Messages"}};

  auto it = enum_strings.find(e);
  assert(it != enum_strings.end());

  return it->second;
}

} // namespace nasdaq
} // namespace exchange
