#ifndef __LIMIT_HPP__
#define __LIMIT_HPP__

#include "./common.hpp"
#include "./order.hpp"

#include <cstddef>
#include <functional>
#include <list>
#include <ostream>
#include <type_traits>
#include <unordered_map>

namespace Orderbook {
class Limit {
public:
  explicit Limit(const AddOrder &);
  Limit(const Limit &);

  Limit(Limit &&);
  void operator=(const Limit &);
  void operator=(Limit &&);

  // get methods
  inline const limit_t &limit() const noexcept { return limit_; }

  // public methods
  bool add_order(AddOrder &&);
  void match_order(AddOrder &);

  inline size_t size() const noexcept { return size_; }
  inline shares_t total_shares() const noexcept { return total_shares_; }
  inline const std::list<AddOrder> &list_order() const noexcept {
    return list_order_;
  }

  // operator
  constexpr bool operator==(const Limit &other) const noexcept {
    return limit_ == other.limit_;
  }

  constexpr bool operator<(const Limit &other) const noexcept {
    return limit_ < other.limit_;
  }

  constexpr bool operator>(const Limit &other) const noexcept {
    return limit_ > other.limit_;
  }

private:
  void pop_front_();

private:
  size_t size_;
  limit_t limit_;
  shares_t total_shares_;

  std::list<AddOrder> list_order_;
  std::unordered_map<id_t, typename std::list<AddOrder>::iterator> map_order_;
};
} // namespace Orderbook

template <> struct std::hash<Orderbook::Limit> {
  std::size_t operator()(const Orderbook::Limit &limit) const {
    return std::hash<Orderbook::limit_t>{}(limit.limit());
  }
};

#endif // !__LIMIT_HPP__
