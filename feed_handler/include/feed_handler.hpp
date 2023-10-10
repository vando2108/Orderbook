#ifndef __FEED_HANDLER_HPP__
#define __FEED_HANDLER_HPP__

#include "./message.hpp"

namespace FeedHandler {
class IFeedHandler {
public:
  IFeedHandler(msg_queue_t &queue) : msg_queue_(queue) {}
  virtual ~IFeedHandler() = default;

public:
  virtual void handle_historical_data(std::string_view) = 0;
  void emit(char type, char *body) { msg_queue_.push(message{type, body}); }

private:
  msg_queue_t &msg_queue_;
};
}; // namespace FeedHandler

#endif // !__FEED_HANDLER_HPP__
