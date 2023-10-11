#ifndef __FEED_HANDLER_MESSAGE_HPP__
#define __FEED_HANDLER_MESSAGE_HPP__

#include "boost/lockfree/policies.hpp"
#include <boost/lockfree/queue.hpp>
#include <string_view>

namespace FeedHandler {
struct message {
  /* This message will be emit by an data feed.
   * Order book will consume and handle these messages via message queue.
   * Cause message contain char* body, which is will cast to message struct
   * Once orderbook finished handle a message, orderbook need to delete body
   * buffer
   * */
  char type;
  char *buffer;
};

using msg_queue_t =
    boost::lockfree::queue<message, boost::lockfree::capacity<100>>;
}; // namespace FeedHandler

#endif // __FEED_HANDLER_MESSAGE_HPP__
