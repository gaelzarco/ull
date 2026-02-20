// #include <unordered_set>
// #include <array>
import <unordered_set>;
import <array>;

// Standard approach
// Heavily utilizes pointer indirection (math efficient; not hardware eff)
bool is_duplicate_trade(int trade_id, std::unordered_set<int>& history) {
    // 1. Hashing overhead
    // 2. Potential cache miss looking up the bucket
    // 3. Potential cache miss traversing the list in bucket
    // 4. Memory alloc if a rehash is triggered
    auto result = history.insert(trade_id);
    return !result.second;
}

// HFT approach
// Flat structures are better
// Range of trade IDs known? -> bitset or direct lookup table.
// Need history buff? -> ring (circular) buff on stack or pre-alloc mem
// Strict compile time
constexpr const size_t MAX_TRADES = 100'000;

// Zero mem alloc during run-time
// Zero pointer chasing
// Compiles down to base addr calculation and bit toggle (consistent run-time)
class TradeFilter {
    // Contig mem block (no ptrs; no heap)
    // L1 cache friendly
    std::array<bool, MAX_TRADES> seen_flags_{};

public:
    // Mark consteval or constexpr to ensure full optimization
    // Run-time usage needed for live data
    bool is_duplicate_trade(std::size_t trade_id) {
        if (trade_id >= MAX_TRADES) [[unlikely]] {
            // Handle error path without throwing except
            return true;
        }

        if (seen_flags_[trade_id]) {
            return true;
        }

        seen_flags_[trade_id] = true;
        return false;
    }
};

int main(void) { return 0; }
