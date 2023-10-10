#include "../feed_handler/include/feed_handler.hpp"
#include "../include/nasdaq/nasdaq.hpp"

#include <chrono>
#include <glog/logging.h>
#include <sys/resource.h>
#include <thread>
#include <vector>

int main(int argc, char *argv[]) {
  const rlim_t kStackSize = 16 * 1024 * 1024; // min stack size = 16 MB
  struct rlimit rl;
  int result;

  result = getrlimit(RLIMIT_STACK, &rl);
  if (result == 0) {
    if (rl.rlim_cur < kStackSize) {
      rl.rlim_cur = kStackSize;
      result = setrlimit(RLIMIT_STACK, &rl);
      if (result != 0) {
        fprintf(stderr, "setrlimit returned result = %d\n", result);
      }
    }
  }

  FLAGS_logtostderr = 1;
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging(argv[0]);

  exchange::nasdaq::nasdaq nasdaq;
  nasdaq.add_historical_feed("../data/12302019.NASDAQ_ITCH50");

  // Orderbook::Book e_book("E");
  // std::vector<Orderbook::AddOrder> orders = {
  //     Orderbook::AddOrder(0, Orderbook::Side::SELL, 310300, 3000,
  //                         14400022387518, "E"),
  //     Orderbook::AddOrder(0, Orderbook::Side::BUY, 312500, 3000,
  //     14400024536615,
  //                         "E"),
  //     Orderbook::AddOrder(0, Orderbook::Side::SELL, 310800, 2000,
  //                         14400025603453, "E"),
  //     Orderbook::AddOrder(0, Orderbook::Side::SELL, 310700, 1000,
  //                         14400026646003, "E"),
  //     Orderbook::AddOrder(0, Orderbook::Side::BUY, 311900, 2000,
  //     14400027862872,
  //                         "E"),
  //     Orderbook::AddOrder(0, Orderbook::Side::BUY, 312000, 2000,
  //     14400028858496,
  //                         "E"),
  //
  // };
  //
  // for (auto &order : orders) {
  //   e_book.process_order(std::move(order));
  //   e_book.debug();
  //   LOG(INFO);
  //   LOG(INFO);
  //   LOG(INFO);
  //   LOG(INFO);
  //   LOG(INFO);
  // }
  //
  // e_book.debug();

  return 0;
}
