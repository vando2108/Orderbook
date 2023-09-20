#ifndef __ORDER_HPP__
#define __ORDER_HPP__

#include "./common.hpp"

#include <iostream>
#include <list>
#include <ostream>
#include <unordered_map>

namespace Orderbook {
class IOrder {
  friend class Book;

public:
  IOrder(id_t id, timestamp_t timestamp) : id_(id), timestamp_(timestamp) {}

  inline const id_t &id() const noexcept { return id_; }
  inline const timestamp_t &timestamp() const noexcept { return timestamp_; };

private:
  id_t id_;
  timestamp_t timestamp_;
};

class AddOrder : public IOrder {
public:
  AddOrder(id_t id, Side side, limit_t limit, shares_t shares, symbol_t symbol,
           timestamp_t timestamp)
      : side_(side), limit_(limit), shares_(shares), symbol_(symbol),
        IOrder(id, timestamp) {}

  inline const Side &side() const noexcept { return side_; }
  inline const limit_t &limit() const noexcept { return limit_; };
  inline const shares_t &shares() const noexcept { return shares_; }

  void print() const {
    printf("shares: %lld | id: %d | timestamp: %lld\n", shares(), id(),
           timestamp());
  }

  bool match(shares_t shares) noexcept {
    if (shares_ < shares) {
      return false;
    }
    shares_ -= shares;
    return true;
  }

private:
  Side side_;
  limit_t limit_;
  shares_t shares_;
  symbol_t symbol_;
};
}; // namespace Orderbook

#endif // !__ORDER_HPP__
