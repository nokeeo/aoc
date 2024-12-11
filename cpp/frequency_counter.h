#ifndef FREQUENCY_COUNTER_H_
#define FREQUENCY_COUNTER_H_
#include <unordered_map>

namespace aoc {
  template<typename T>
  class FrequencyCounter {
    static constexpr int64_t kNotFound = 0;
    public:
    void Increment(const T& v, int64_t amount = 1) {
      int64_t count = 0;
      auto itr = counts_.find(v);
      if (itr != counts_.end()) {
        count = itr->second;
      }
      counts_.insert_or_assign(v, count + amount);
    }

    void Set(const T& k, int64_t v) { counts_.insert_or_assign(k, v); }

    int64_t GetCount(const T& k) const {
      auto itr = counts_.find(k);
      if (itr == counts_.end()) {
        return kNotFound;
      }
      return itr->second;
    }

    typename std::unordered_map<T, int64_t>::const_iterator max() const {
      if (unique_size() == 0) {
        return end();
      }

      typename std::unordered_map<T, int64_t>::const_iterator max = begin(); 
      for (auto itr = begin(); itr != end(); ++itr) {
        if (max->second < itr->second) {
          max = itr;
        }
      }
      return max;
    }

    typename std::unordered_map<T, int64_t>::const_iterator begin() const { return counts_.begin(); }
    typename std::unordered_map<T, int64_t>::const_iterator end() const { return counts_.end(); }
    size_t unique_size() const { return counts_.size(); }

    private:
    std::unordered_map<T, int64_t> counts_;
  };
}
#endif  // FREQUENCY_COUNTER_H_
