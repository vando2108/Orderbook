#ifndef __LIMIT_CPP__
#define __LIMIT_CPP__

#include "../include/limit.hpp"
#include <ostream>

namespace Orderbook {

// public methods
bool Limit::add_order(AddOrder &&order) {
  if (map_order_.find(order.id()) != map_order_.end()) {
    return false;
  }

  ++size_;
  list_order_.push_back(std::forward<AddOrder>(order));
  map_order_[list_order_.back().limit()] = --list_order_.end();
  total_shares_ += order.shares();

  return true;
}

void Limit::match_order(AddOrder &order) {
  while (order.shares() && list_order_.size()) {
    auto &matching_order = list_order_.front();
    auto matched_shares = std::min(order.shares(), matching_order.shares());

    order.match(matched_shares);
    matching_order.match(matched_shares);
    total_shares_ -= matched_shares;

    if (matching_order.shares() == 0) {
      pop_front_();
    }
  }
}

// private methods
void Limit::pop_front_() {
  map_order_.erase(list_order_.front().id());
  total_shares_ -= list_order_.front().shares();
  --size_;

  list_order_.pop_front();
}
} // namespace Orderbook

#endif // !__LIMIT_CPP__
