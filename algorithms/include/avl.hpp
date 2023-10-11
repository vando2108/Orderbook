#ifndef __ALGORITHMS_AVL_HPP__
#define __ALGORITHMS_AVL_HPP__

#include <algorithm>
#include <functional>
#include <glog/logging.h>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

namespace Algorithms {
template <typename T> class Avl {
public:
  using compare_t = std::function<bool(const T &, const T &)>;

private:
  struct Node {
    T data;
    uint32_t height;
    std::shared_ptr<Node> left, right, parent;

    // universal reference
    template <typename X = T>
    Node(X &&d, std::shared_ptr<Node> &&l, std::shared_ptr<Node> &&r,
         const std::shared_ptr<Node> &p, uint32_t h = 0)
        : data(std::forward<X>(d)), left(std::move(l)), right(std::move(r)),
          parent(p), height(h) {}
  };

public:
  // constructor & destructor
  explicit Avl(compare_t &&comp)
      : root_(nullptr), size_(0), comp_(std::move(comp)), top_(nullptr) {}
  explicit Avl(const Avl &);
  // explicit Avl(Avl &&);

public:
  // public methods
  void insert(T &&);
  bool remove(const T &);
  void remove_top();

  std::shared_ptr<Node> get_node(const T &);

  void debug(std::shared_ptr<Node>, const std::function<void(const T &)> &);
  std::vector<T> inorder() const;

public:
  inline const std::shared_ptr<Node> &top() const { return top_; }
  inline const size_t &size() const { return size_; }
  inline const std::shared_ptr<Node> &root() const { return root_; }

private:
  // private methods
  void insert_util_(T &&, std::shared_ptr<Node> &,
                    const std::shared_ptr<Node> &);
  std::shared_ptr<Node> clone_(const std::shared_ptr<Node> &) const;
  void inorder_util_(const std::shared_ptr<Node> &, std::vector<T> &) const;

  // remove utils
  void remove_util_(std::shared_ptr<Node> &, const std::shared_ptr<Node> &);
  void remove_leaf_node_(std::shared_ptr<Node> &);
  std::shared_ptr<Node> min_value_node_(const std::shared_ptr<Node> &node) {
    auto temp = node;
    while (temp->left) {
      temp = temp->left;
    }
    return temp;
  }

  // rebalance utils
  void rebalance_(std::shared_ptr<Node> &);
  void left_rotate_(std::shared_ptr<Node> &);
  void right_rotate_(std::shared_ptr<Node> &);
  inline uint32_t node_height_(const std::shared_ptr<Node> &) const;

private:
  compare_t comp_;
  std::size_t size_;
  std::shared_ptr<Node> root_;
  std::unordered_map<T, std::shared_ptr<Node>> node_map_;
  std::shared_ptr<Node> top_;
};

template <typename T>
void Avl<T>::debug(std::shared_ptr<Node> cur,
                   const std::function<void(const T &)> &func) {
  if (cur == nullptr)
    return;
  debug(cur->left, func);
  func(cur->data);
  debug(cur->right, func);
}

// constructor & destructor
template <typename T>
Avl<T>::Avl(const Avl &other)
    : root_(clone_(other.root_)), size_(other.size_), comp_(other.comp_),
      top_(nullptr), node_map_(other.node_map_) {}

// template <typename T> Avl<T>::Avl(Avl &&other) : Avl() {
//   std::swap(root_, other);
//   std::swap(top_, other.top_);
//   std::swap(size_, other.size_);
//   std::swap(comp_, other.comp_);
//   std::swap(node_map_, other.node_map_);
// }

// public methods
template <typename T> void Avl<T>::insert(T &&data) {
  insert_util_(std::forward<T>(data), root_, nullptr);
  ++size_;
}

template <typename T> void Avl<T>::remove_top() {
  /*
   * Depend on the type of tree (sell or buy) top will be equal to highest or
   * lowest node
   *
   * Note that highest or lowest node just have no more than one child
   *
   * Example: with lowest node, this node just can be has one right child
   * cause if it has left child, left child will be lowest
   *
   * Based on the remove operator, if a node just has one child, this child will
   * replace current node. If not current node will be replace by parent
   * */
  auto remove_data = top_->data;
  top_ = (top_->left || top_->right) ? top_ : top_->parent;
  remove(remove_data);
}

template <typename T>
void Avl<T>::remove_leaf_node_(std::shared_ptr<Node> &node) {
  node_map_.erase(node->data);
  if (node->parent) {
    if (node->parent->left == node)
      node->parent->left = nullptr;
    else
      node->parent->right = nullptr;

    node->parent = nullptr;
  }
  node = nullptr;
}

template <typename T> bool Avl<T>::remove(const T &data) {
  auto it = node_map_.find(data);
  if (it == node_map_.end()) {
    return false;
  }

  auto node = it->second;
  if (node == root_) {
    remove_util_(root_, nullptr);
  } else {
    remove_util_(node, root_);
  }

  --size_;

  return true;
}

template <typename T>
void Avl<T>::remove_util_(std::shared_ptr<Node> &node,
                          const std::shared_ptr<Node> &upper) {
  if (node->left == nullptr || node->right == nullptr) {
    auto temp = node->left ? node->left : node->right;
    if (temp == nullptr) {
      /* no child <=> leaf node */
      remove_leaf_node_(node);
    } else {
      /* has one child => swap data of current node with child node (temp) */
      std::swap(node->data, temp->data);
      std::swap(node_map_[node->data], node_map_[temp->data]);
      remove_leaf_node_(temp);
    }
  } else {
    /* has two child => swap current node value with min_value_node */
    auto temp = min_value_node_(node->right);

    std::swap(node->data, temp->data);
    std::swap(node_map_[node->data], node_map_[temp->data]);

    remove_util_(temp, node);
  }

  if (node == nullptr) {
    return;
  }

  node->height =
      1 + std::max(node_height_(node->left), node_height_(node->right));

  while (node != upper && node != nullptr) {
    rebalance_(node);
    node = node->parent;
  }
}

template <typename T>
std::shared_ptr<typename Avl<T>::Node> Avl<T>::get_node(const T &data) {
  if (node_map_.find(data) != node_map_.end()) {
    return node_map_[data];
  }
  return nullptr;
}

template <typename T> std::vector<T> Avl<T>::inorder() const {
  std::vector<T> res;
  inorder_util_(root_, res);
  return res;
}

// private methods
template <typename T>
std::shared_ptr<typename Avl<T>::Node>
Avl<T>::clone_(const std::shared_ptr<Node> &node) const {
  if (!node) {
    return nullptr;
  }

  return std::make_shared<Node>(node->data, clone_(node->left),
                                clone_(node->right), node->height);
}

// public method utils
template <typename T>
void Avl<T>::inorder_util_(const std::shared_ptr<Node> &cur_node,
                           std::vector<T> &res) const {
  if (!cur_node) {
    return;
  }
  inorder_util_(cur_node->left, res);
  res.push_back(cur_node->data);
  inorder_util_(cur_node->right, res);
}

template <typename T>
void Avl<T>::insert_util_(T &&data, std::shared_ptr<Node> &cur_node,
                          const std::shared_ptr<Node> &parent) {
  if (!cur_node) {
    cur_node =
        std::make_shared<Node>(std::forward<T>(data), nullptr, nullptr, parent);
    node_map_[cur_node->data] = cur_node;

    if (top_) {
      if (comp_(top_->data, cur_node->data)) {
        top_ = cur_node;
      }
    } else {
      top_ = cur_node;
    }
  } else if (data < cur_node->data) {
    // left node
    insert_util_(std::forward<T>(data), cur_node->left, cur_node);
  } else {
    // right node
    insert_util_(std::forward<T>(data), cur_node->right, cur_node);
  }

  rebalance_(cur_node);
}

// rebalance implementation
template <typename T> void Avl<T>::rebalance_(std::shared_ptr<Node> &cur_node) {
  if (cur_node == nullptr) {
    return;
  }

  int balance = node_height_(cur_node->left) - node_height_(cur_node->right);

  if (balance > 1) {
    if (node_height_(cur_node->left->left) >=
        node_height_(cur_node->left->right)) {
      /*
           T1, T2, T3 and T4 are subtrees.
              z                                      y
             / \                                   /   \
            y   T4      Right Rotate (z)          x      z
           / \          - - - - - - - - ->      /  \    /  \
          x   T3                               T1  T2  T3  T4
         / \
       T1   T2
      */
      right_rotate_(cur_node);
    } else {
      /*
            z                               z                           x
           / \                            /   \                        /  \
          y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
         / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
        T1   x                          y    T3                    T1  T2 T3 T4
           / \                        / \
         T2   T3                    T1   T2
      */
      std::shared_ptr<Node> temp = cur_node->left;
      left_rotate_(temp);
      right_rotate_(cur_node);
    }
  } else if (balance < -1) {
    if (node_height_(cur_node->right->left) <=
        node_height_(cur_node->right->right)) {
      /*
         z                                y
        /  \                            /   \
       T1   y     Left Rotate(z)       z      x
           /  \   - - - - - - - ->    / \    / \
          T2   x                     T1  T2 T3  T4
              / \
            T3  T4
      */
      left_rotate_(cur_node);
    } else {
      /*
          z                            z                            x
         / \                          / \                          /  \
       T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
           / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
          x   T4                      T2   y                  T1  T2  T3  T4
         / \                              /  \
       T2   T3                           T3   T4
      */
      std::shared_ptr<Node> temp = cur_node->right;
      right_rotate_(temp);
      left_rotate_(cur_node);
    }
  }

  cur_node->height =
      1 + std::max(node_height_(cur_node->left), node_height_(cur_node->right));
}

// rotate methods implementation
//     y                               x
//    / \     Right Rotation          /  \
//   x   T3   - - - - - - - >        T1   y
//  / \       < - - - - - - -            / \
// T1  T2     Left Rotation            T2  T3
template <typename T>
inline uint32_t Avl<T>::node_height_(const std::shared_ptr<Node> &node) const {
  return (node != nullptr ? node->height : 0);
}

template <typename T> void Avl<T>::right_rotate_(std::shared_ptr<Node> &y) {
  bool flag = (y == root_);
  auto parent = y->parent;

  auto x = std::move(y->left);

  if (parent) {
    if (parent->left == y) {
      parent->left = x;
    } else {
      parent->right = x;
    }
  }

  if (x->right) {
    x->right->parent = y;
  }
  y->left = std::move(x->right);

  y->height = 1 + std::max(node_height_(y->left), node_height_(y->right));
  x->height = 1 + std::max(node_height_(x->left), node_height_(y));

  y->parent = x;
  x->parent = parent;
  x->right = std::move(y);

  y = std::move(x);

  if (flag) {
    root_ = y;
  }
}

template <typename T> void Avl<T>::left_rotate_(std::shared_ptr<Node> &x) {
  bool flag = (x == root_);
  auto parent = x->parent;
  auto y = std::move(x->right);

  if (parent) {
    if (parent->left == x) {
      parent->left = y;
    } else {
      parent->right = y;
    }
  }

  if (y->left) {
    y->left->parent = x;
  }
  x->right = std::move(y->left);

  x->height = 1 + std::max(node_height_(x->left), node_height_(x->right));
  y->height = 1 + std::max(node_height_(x), node_height_(y->right));

  y->parent = parent;
  x->parent = y;

  y->left = std::move(x);
  x = std::move(y);

  if (flag) {
    root_ = x;
  }
}

}; // namespace Algorithms

#endif // __ALGORITHMS_AVL_HPP__
