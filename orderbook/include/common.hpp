#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include <cstdint>

namespace Orderbook {
typedef std::uint32_t id_t;
typedef std::uint64_t limit_t;
typedef std::uint16_t symbol_t;
typedef std::uint64_t shares_t;
typedef std::uint64_t timestamp_t;

enum class Side { BUY, SELL };
}; // namespace Orderbook

#endif
