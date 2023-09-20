#ifndef __BOOK_HPP__
#define __BOOK_HPP__

#include "./avl.hpp"
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
  Book()
      : buy_tree_([](const Limit &a, const Limit &b) { return a < b; }),
        sell_tree_([](const Limit &a, const Limit &b) { return a > b; }),
        id_counter_(0) {}

public:
  void debug() {
    std::cout << "----------buy side------------\n";
    std::cout << "top limit: " << buy_tree_.top()->data.limit() << '\n';
    buy_tree_.debug([](const Limit &limit) {
      std::cout << "price level: " << limit.limit() << ' '
                << "number orders: " << limit.list_order().size() << ' '
                << "total shares: " << limit.total_shares() << '\n';

      std::cout << "id | shares | timestamp\n";
      for (const auto &order : limit.list_order()) {
        order.print();
      }
    });

    std::cout << "----------sell side------------\n";
    std::cout << "top limit: " << sell_tree_.top()->data.limit() << '\n';
    sell_tree_.debug([](const Limit &limit) {
      std::cout << "price level: " << limit.limit() << ' '
                << "number orders: " << limit.list_order().size() << ' '
                << "total shares: " << limit.total_shares() << '\n';

      std::cout << "id | shares | timestamp\n";
      for (const auto &order : limit.list_order()) {
        order.print();
      }
    });
  }

public:
  void process_order(std::derived_from<IOrder> auto &&);

private:
  void process_add_order_(AddOrder &&);

  timestamp_t get_time_from_midnight_();

  id_t id_counter_;
  Algorithms::Avl<Limit> buy_tree_, sell_tree_;
};

void Book::process_order(std::derived_from<IOrder> auto &&order) {
  order.id_ = ++id_counter_;
  order.timestamp_ = get_time_from_midnight_();

  if constexpr (std::is_same_v<std::remove_reference_t<decltype(order)>,
                               AddOrder>) {
    process_add_order_(std::forward<AddOrder>(order));
  } else {
  }
}
}; // namespace Orderbook

#endif // !__BOOK_HPP__
