#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <cstdint>
#include <string_view>

namespace Orderbook {
using id_t = std::uint32_t;
using limit_t = std::uint64_t;
using symbol_t = std::string;
using shares_t = std::uint64_t;
using timestamp_t = std::uint64_t;

enum class Side { BUY, SELL };
}; // namespace Orderbook

#endif
