#ifndef __ALGORITHMS_ATOMIC_QUEUE_HPP__
#define __ALGORITHMS_ATOMIC_QUEUE_HPP__

#include <atomic>
#include <cstddef>
#include <glog/logging.h>
#include <memory>
#include <utility>

namespace Algorithms {
template <typename T> class LockFreeQueue {
private:
  struct Node {
    T data;
    std::atomic<Node *> next;

    explicit Node(T &&d) : data(std::move(d)), next(nullptr) {}
  };

  std::atomic<Node *> head_;
  std::atomic<Node *> tail_;
  std::atomic<size_t> size_;

public:
  LockFreeQueue() : head_(nullptr), tail_(nullptr) {}

  ~LockFreeQueue() {
    tail_.store(nullptr);
    while (Node *node = head_.load()) {
      head_.store(node->next);
      delete node;
    }
  }

  void push(T &&data) {
    Node *const new_node = new Node(std::forward<T>(data));

    while (true) {
      Node *tail_node = tail_.load();
      if (!tail_node) {
        // The queue is empty
        if (tail_.compare_exchange_weak(tail_node, new_node)) {
          head_.store(new_node);
          size_.fetch_add(1);
          return;
        }
      } else {
        LOG(INFO) << "Queue size: " << size_ << '\n';
        Node *temp = nullptr;
        tail_node = tail_.load();
        if (tail_node->next.compare_exchange_weak(temp, new_node)) {
          LOG(INFO) << "if passed" << '\n';
          tail_.compare_exchange_weak(tail_node, new_node);
          size_.fetch_add(1);
          return;
        }
      }
    }
  }

  bool pop(T &data) {
    while (true) {
      Node *head_node = head_.load(), *tail_node = tail_.load();
      if (head_node == tail_node) {
        if (head_node) {
          // The queue just has one element
          if (tail_.compare_exchange_weak(tail_node, nullptr)) {
            head_.store(nullptr);
            data = head_node->data;
            delete head_node;
            size_.fetch_sub(1);

            return true;
          }
        } else {
          // The queue is empty
          return false;
        }
      } else {
        if (head_.compare_exchange_weak(head_node, head_node->next)) {
          data = head_node->data;
          delete head_node;
          size_.fetch_sub(1);

          return true;
        }
      }
    }
  }
};
}; // namespace Algorithms

#endif // __ALGORITHMS_ATOMIC_QUEUE_HPP__
