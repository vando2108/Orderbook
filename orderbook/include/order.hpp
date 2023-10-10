#ifndef __ORDER_HPP__
#define __ORDER_HPP__

#include "./common.hpp"

#include <glog/logging.h>
#include <iostream>
#include <list>
#include <ostream>
#include <string_view>
#include <unordered_map>

namespace Orderbook {
class IOrder {
  friend class Book;

public:
  IOrder(id_t id, timestamp_t timestamp, const symbol_t &symbol)
      : id_(id), timestamp_(timestamp), symbol_(symbol) {}

  inline const id_t &id() const noexcept { return id_; }
  inline const timestamp_t &timestamp() const noexcept { return timestamp_; };
  inline const std::string &symbol() const noexcept { return symbol_; };

private:
  id_t id_;
  timestamp_t timestamp_;
  symbol_t symbol_;
};

class AddOrder : public IOrder {
public:
  AddOrder(id_t id, Side side, limit_t limit, shares_t shares,
           timestamp_t timestamp, const symbol_t &symbol)
      : side_(side), limit_(limit), shares_(shares),
        IOrder(id, timestamp, symbol) {}

  inline const Side &side() const noexcept { return side_; }
  inline const limit_t &limit() const noexcept { return limit_; };
  inline const shares_t &shares() const noexcept { return shares_; }

  void log() const noexcept {
    LOG(INFO) << id() << " | " << limit_ << " | " << shares_ << " | "
              << timestamp() << " | " << symbol() << '\n';
  }

  void debug() const noexcept {
    auto temp =
        side() == Side::SELL ? "Orderbook::Side::SELL" : "Orderbook::Side::BUY";
    LOG(INFO) << id() << ", " << temp << ", " << limit() << ", " << shares()
              << ", " << timestamp() << ", " << symbol();
  };

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
};
}; // namespace Orderbook

#endif // !__ORDER_HPP__
