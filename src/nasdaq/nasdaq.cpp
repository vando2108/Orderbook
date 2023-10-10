#include "../../include/nasdaq/nasdaq.hpp"
#include "../../feed_handler/include/nasdaq_itch50/nasdaq_itch50.hpp"
#include "../../include/nasdaq/enum.hpp"

#include <cstdio>
#include <cstring>
#include <glog/logging.h>

#include <memory>
#include <string>
#include <thread>
#include <utility>

using nasdaq = exchange::nasdaq::nasdaq;

void nasdaq::add_historical_feed(std::string_view path) {
  boost::lockfree::queue<FeedHandler::message, boost::lockfree::capacity<1000>>
      msg_queue;
  std::unique_ptr<FeedHandler::IFeedHandler> feed_handler(
      std::make_unique<FeedHandler::NasdaqITCH50::NasdaqITCH50>(msg_queue));

  /* Spawn a thread for receive data from data feed */
  std::thread producer([&] { feed_handler->handle_historical_data(path); });

  /* Spawn a thread to consume and handle messages from message_queue */
  std::thread consumer([&] {
    while (true) {
      FeedHandler::message msg;
      if (msg_queue.pop(msg)) {
        if (!handle_msg(std::move(msg))) {
          break;
        }
      }
    }
  });

  consumer.join();
  producer.join();
}

bool nasdaq::handle_msg(FeedHandler::message &&msg) {
  if (msg.type == 'S') {
    handle_system_event_(std::forward<FeedHandler::message>(msg));
  } else {
    handle_order_action_(std::forward<FeedHandler::message>(msg));
  }

  return true;
}

void nasdaq::handle_system_event_(FeedHandler::message &&msg) {
  FeedHandler::NasdaqITCH50::SystemEventMessage parsed_msg(msg.buffer);
  delete[] msg.buffer;
  auto temp = static_cast<SystemEvent>(parsed_msg.event_code);
  LOG(INFO) << parsed_msg.timestamp << ' ' << system_event_to_string(temp);
}

void nasdaq::handle_order_action_(FeedHandler::message &&msg) {
  if (msg.type == 'A') {
    FeedHandler::NasdaqITCH50::AddOrderMessage parsed_msg(msg.buffer);

    Orderbook::AddOrder order(
        parsed_msg.tracking_number,
        (parsed_msg.is_buy_side ? Orderbook::Side::BUY : Orderbook::Side::SELL),
        parsed_msg.price, parsed_msg.shares, parsed_msg.timestamp,
        parsed_msg.symbol());

    // if (order.symbol() == "E       ") {
    execute_order_(std::move(order));
    // }
  }
}
