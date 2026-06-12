#pragma once
#include <cstdint>
#include <string>

namespace orderbook::foundation {

enum class Side : uint8_t { BUY, SELL };
enum class Type : uint8_t { LIMIT, MARKET };

struct Order {
  uint64_t order_id;
  Side side;
  Type type;
  double price;
  uint64_t quantity;
  uint64_t timestamp_ns;
};

} // namespace orderbook::foundation
