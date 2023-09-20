#include "./orderbook/include/book.hpp"
#include "orderbook/include/common.hpp"
#include "orderbook/include/order.hpp"
#include <concepts>
#include <iostream>
#include <memory>
#include <vector>

int main() {
  Orderbook::Book book;

  std::vector<Orderbook::AddOrder> orders = {
      Orderbook::AddOrder(1, Orderbook::Side::BUY, 10, 10, 10, 10),
      Orderbook::AddOrder(1, Orderbook::Side::SELL, 10, 11, 10, 10),
      Orderbook::AddOrder(1, Orderbook::Side::SELL, 6, 6, 10, 10),
      // Orderbook::AddOrder(1, Orderbook::Side::SELL, 6, 6, 10, 10),
      // Orderbook::AddOrder(1, Orderbook::Side::BUY, 2, 10, 10, 10),
      // Orderbook::AddOrder(1, Orderbook::Side::BUY, 11, 10, 10, 10),
  };

  while (orders.size()) {
    auto &temp = orders.back();
    book.process_order(std::move(temp));
    orders.pop_back();
  }
  book.debug();
  std::cerr << '\n';

  return 0;
}
