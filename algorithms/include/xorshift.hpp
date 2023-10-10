#ifndef __XORSHIFT_HPP__
#define __XORSHIFT_HPP__

#include <cstdint>
namespace Algorithms {
class Xorshift {
public:
  Xorshift() : x_(21082001), y_(6061997), z_(29122022), w_(123123125) {}

  std::uint32_t next() {
    auto temp = x_ ^ (x_ << 11);

    x_ = y_, y_ = z_, z_ = w_;

    return w_ = w_ ^ (w_ >> 19) ^ temp ^ (temp >> 8);
  }

private:
  std::uint32_t x_, y_, z_, w_;
};
}; // namespace Algorithms

#endif // !__XORSHIFT_HPP__
