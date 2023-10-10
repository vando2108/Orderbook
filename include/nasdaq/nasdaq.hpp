#ifndef __EXCHANGE__NASDAQ__NASDAQ_HPP__
#define __EXCHANGE__NASDAQ__NASDAQ_HPP__

#include "../exchange.hpp"
#include <unordered_map>
#include <unordered_set>

namespace exchange {
namespace nasdaq {
class nasdaq : public iexchange {
public:
  nasdaq() : iexchange() {}

public:
  void add_historical_feed(std::string_view) override;
  bool handle_msg(FeedHandler::message &&) override;

private:
  void handle_system_event_(FeedHandler::message &&) override;
  void handle_order_action_(FeedHandler::message &&) override;
};
} // namespace nasdaq
} // namespace exchange

#endif // !__EXCHANGE__NASDAQ__NASDAQ_HPP__
