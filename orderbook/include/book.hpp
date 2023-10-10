#ifndef __BOOK_HPP__
#define __BOOK_HPP__

#include "../../algorithms/include/avl.hpp"
#include "./limit.hpp"
#include "common.hpp"
#include "order.hpp"

#include <chrono>
#include <concepts>
#include <functional>
#include <string>
#include <utility>

namespace Orderbook {
class Book {
public:
  // constructor and destructor
  explicit Book(const symbol_t &symbol)
      : buy_tree_([](const Limit &a, const Limit &b) { return a < b; }),
        sell_tree_([](const Limit &a, const Limit &b) { return a > b; }),
        id_counter_(0), symbol_(symbol) {}

public:
  void debug() {
    LOG(INFO) << "----------buy side------------\n";
    if (buy_tree_.top()) {
      LOG(INFO) << "top limit: " << buy_tree_.top()->data.limit() << '\n';
    }
    buy_tree_.debug(buy_tree_.root(), [](const Limit &limit) {
      LOG(INFO) << "price level: " << limit.limit() << ' '
                << "number orders: " << limit.list_order().size() << ' '
                << "total shares: " << limit.total_shares() << '\n';

      LOG(INFO) << "id | shares | timestamp\n";
      for (const auto &order : limit.list_order()) {
        order.log();
      }
    });

    LOG(INFO) << "----------sell side------------\n";
    if (sell_tree_.top()) {
      LOG(INFO) << "top limit: " << sell_tree_.top()->data.limit() << '\n';
    }
    sell_tree_.debug(sell_tree_.root(), [](const Limit &limit) {
      LOG(INFO) << "price level: " << limit.limit() << ' '
                << "number orders: " << limit.list_order().size() << ' '
                << "total shares: " << limit.total_shares() << '\n';

      LOG(INFO) << "id | shares | timestamp\n";
      for (const auto &order : limit.list_order()) {
        order.log();
      }
    });
  }

public:
  void process_order(std::derived_from<IOrder> auto &&);
  inline const std::string_view symbol() const noexcept { return symbol_; };

private:
  void process_add_order_(AddOrder &&);
  // timestamp_t get_time_from_midnight_();

private:
  id_t id_counter_;
  symbol_t symbol_;
  Algorithms::Avl<Limit> buy_tree_, sell_tree_;
};

void Book::process_order(std::derived_from<IOrder> auto &&order) {
  if constexpr (std::is_same_v<std::remove_reference_t<decltype(order)>,
                               AddOrder>) {
    order.debug();
    process_add_order_(std::forward<AddOrder>(order));
  } else {
  }
}
}; // namespace Orderbook

#endif // !__BOOK_HPP__
