#ifndef __BOOK_CPP__
#define __BOOK_CPP__

#include "../include/book.hpp"
#include <algorithm>
#include <functional>

namespace Orderbook {
void Book::process_add_order_(AddOrder &&order) {
  Algorithms::Avl<Limit> &op_limit_tree =
      order.side() == Side::BUY ? sell_tree_ : buy_tree_; // opposite limit tree

  auto comp = order.side() == Side::BUY
                  ? [](limit_t a, limit_t b) { return a <= b; }
                  : [](limit_t a, limit_t b) { return a >= b; };

  while (order.shares() && op_limit_tree.size() &&
         comp(op_limit_tree.top()->data.limit(), order.limit())) {
    auto top_limit = op_limit_tree.top();
    top_limit->data.match_order(order);

    if (top_limit->data.size() == 0) {
      op_limit_tree.remove_top();
    }
  }

  if (order.shares() > 0) {
    Limit limit(order);
    Algorithms::Avl<Limit> &limit_tree =
        order.side() == Side::BUY ? buy_tree_ : sell_tree_;
    auto node = limit_tree.get_node(limit);

    if (node) {
      if (!node->data.add_order(std::move(order))) {
        LOG(WARNING) << "Failed to add new order to an existsed limit";
      }
    } else {
      limit_tree.insert(std::move(limit));
    }
  }
}

// timestamp_t Book::get_time_from_midnight_() {
//   auto now = std::chrono::system_clock::now();
//   auto today = std::chrono::floor<std::chrono::days>(now);
//   return std::chrono::duration_cast<std::chrono::nanoseconds>(now - today)
//       .count();
// }
} // namespace Orderbook

#endif // __BOOK_CPP__
