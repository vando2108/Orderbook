#include "../algorithms/include/avl.hpp"
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

  // exchange::nasdaq::nasdaq nasdaq;
  // nasdaq.add_historical_feed("../data/12302019.NASDAQ_ITCH50");

  Orderbook::Book e_book("TVIX    ");
  std::vector<Orderbook::AddOrder> orders = {
      // Orderbook::AddOrder(1608, Orderbook::Side::SELL, 486000, 62,
      //                     14400004727759, "TVIX    "),
      // Orderbook::AddOrder(2720, Orderbook::Side::SELL, 486000, 500,
      //                     14400007966912, "TVIX    "),

      Orderbook::AddOrder(22056, Orderbook::Side::BUY, 80000000, 14,
                          14400117591630, "TVIX    "),
      Orderbook::AddOrder(30592, Orderbook::Side::BUY, 888888800, 19,
                          14400429603160, "TVIX    "),
      Orderbook::AddOrder(35880, Orderbook::Side::BUY, 1235900, 4000,
                          14400940892500, "TVIX    "),
      Orderbook::AddOrder(37768, Orderbook::Side::BUY, 49000000, 12,
                          14401036279133, "TVIX    "),
      Orderbook::AddOrder(59328, Orderbook::Side::BUY, 970000, 1000,
                          14403904789654, "TVIX    "),
      Orderbook::AddOrder(60108, Orderbook::Side::BUY, 1180000, 100,
                          14404109440043, "TVIX    "),
      Orderbook::AddOrder(68676, Orderbook::Side::BUY, 1180000, 60,
                          14408324990118, "TVIX    "),

      Orderbook::AddOrder(69052, Orderbook::Side::BUY, 1200000, 10,
                          14408979343620, "TVIX    "),
      // Orderbook::AddOrder(69060, Orderbook::Side::BUY, 1200000, 5,
      //                     14409004248630, "TVIX    "),

      // Orderbook::AddOrder(71124, Orderbook::Side::BUY, 850000, 20,
      //                     14414357951370, "TVIX    "),
  };

  for (auto &order : orders) {
    e_book.process_order(std::move(order));
    e_book.debug();
    LOG(INFO);
    LOG(INFO);
    LOG(INFO);
    LOG(INFO);
    LOG(INFO);
  }

  e_book.debug();
  //
  // Algorithms::Avl<int> avl([](int a, int b) { return a < b; });
  // avl.insert(1605800);
  // LOG(INFO) << avl.top()->data;
  //
  // avl.remove_top();
  // avl.insert(500);
  // avl.insert(800);
  // avl.insert(700);
  // avl.insert(900);
  // avl.insert(1000);

  // avl.debug(avl.root(), [](int a) { LOG(INFO) << a; });

  return 0;
}
