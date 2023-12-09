#include <unordered_map>

namespace aoc {
  template<typename T>
  class FrequencyCounter {
    static constexpr int32_t kNotFound = -1;
    public:
    void Increment(const T& v) {
      int32_t count = 0;
      auto itr = counts_.find(v);
      if (itr != counts_.end()) {
        count = itr->second;
      }
      counts_.insert_or_assign(v, ++count);
    }

    void Set(const T& k, int32_t v) { counts_.insert_or_assign(k, v); }

    int32_t GetCount(const T& k) const {
      auto itr = counts_.find(k);
      if (itr == counts_.end()) {
        return kNotFound;
      }
      return itr->second;
    }

    typename std::unordered_map<T, int32_t>::const_iterator max() const {
      if (unique_size() == 0) {
        return end();
      }

      typename std::unordered_map<T, int32_t>::const_iterator max = begin(); 
      for (auto itr = begin(); itr != end(); ++itr) {
        if (max->second < itr->second) {
          max = itr;
        }
      }
      return max;
    }

    typename std::unordered_map<T, int32_t>::const_iterator begin() const { return counts_.begin(); }
    typename std::unordered_map<T, int32_t>::const_iterator end() const { return counts_.end(); }
    size_t unique_size() const { return counts_.size(); }

    private:
    std::unordered_map<T, int32_t> counts_;
  };
}
