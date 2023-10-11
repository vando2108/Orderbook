#ifndef __LIMIT_CPP__
#define __LIMIT_CPP__

#include "../include/limit.hpp"
#include <ostream>

namespace Orderbook {
// move
Limit::Limit(const AddOrder &order)
    : size_(1), limit_(order.limit()), total_shares_(order.shares()),
      list_order_({order}), map_order_() {
  map_order_[order.id()] = --list_order_.end();
}

Limit::Limit(const Limit &other)
    : size_(other.size_), limit_(other.limit_),
      total_shares_(other.total_shares_), list_order_(other.list_order_),
      map_order_(other.map_order_) {}

Limit::Limit(Limit &&other) {
  std::swap(size_, other.size_);
  std::swap(limit_, other.limit_);
  std::swap(total_shares_, other.total_shares_);
  std::swap(list_order_, other.list_order_);
  std::swap(map_order_, other.map_order_);
}

void Limit::operator=(const Limit &other) {
  size_ = other.size_;
  limit_ = other.limit_;
  total_shares_ = other.total_shares_;
  list_order_ = other.list_order_;
  map_order_.insert(other.map_order_.begin(), other.map_order_.end());
}

void Limit::operator=(Limit &&other) {
  std::swap(size_, other.size_);
  std::swap(limit_, other.limit_);
  std::swap(total_shares_, other.total_shares_);
  std::swap(list_order_, other.list_order_);
  std::swap(map_order_, other.map_order_);
}

// public methods
bool Limit::add_order(AddOrder &&order) {
  // if (map_order_.find(order.id()) != map_order_.end()) {
  //   return false;
  // }

  ++size_;
  list_order_.push_back(std::forward<AddOrder>(order));
  // map_order_[list_order_.back().id()] = --list_order_.end();
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
