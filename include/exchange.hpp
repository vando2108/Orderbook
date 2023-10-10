#ifndef __EXCHANGE_HPP__
#define __EXCHANGE_HPP__

#include "../feed_handler/include/enum.hpp"
#include "../feed_handler/include/feed_handler.hpp"
#include "../feed_handler/include/message.hpp"
#include "../orderbook/include/book.hpp"
#include "../orderbook/include/common.hpp"
#include <memory>

namespace exchange {
class iexchange {
public:
  iexchange() : book_map_() {}

public:
  /* public methods */
  virtual void add_historical_feed(std::string_view) = 0;
  /* This function will return false when this is the last message */
  virtual bool handle_msg(FeedHandler::message &&) = 0;

private:
  virtual void handle_system_event_(FeedHandler::message &&) = 0;
  virtual void handle_order_action_(FeedHandler::message &&) = 0;

protected:
  void execute_order_(std::derived_from<Orderbook::IOrder> auto &&order) {
    auto it = book_map_.find(order.symbol());
    if (it == book_map_.end()) {
      book_map_.insert(
          {order.symbol(), std::make_unique<Orderbook::Book>(order.symbol())});
    }
    book_map_[order.symbol()]->process_order(std::move(order));
    // book_map_[order.symbol()]->debug();
  }

private:
  /* each book will has unique order queue */
  std::unordered_map<Orderbook::symbol_t, std::unique_ptr<Orderbook::Book>>
      book_map_;
  std::unordered_map<Orderbook::symbol_t, FeedHandler::msg_queue_t> queue_map_;
};
}; // namespace exchange

#endif // __EXCHANGE_HPP__
