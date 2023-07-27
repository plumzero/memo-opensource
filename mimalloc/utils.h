
#include <chrono>
#include <type_traits>

template <typename TU,
          class = typename std::enable_if<std::is_same<TU, std::chrono::hours>::value ||
                                          std::is_same<TU, std::chrono::minutes>::value ||
                                          std::is_same<TU, std::chrono::seconds>::value ||
                                          std::is_same<TU, std::chrono::milliseconds>::value ||
                                          std::is_same<TU, std::chrono::microseconds>::value ||
                                          std::is_same<TU, std::chrono::nanoseconds>::value>::type,
          typename TP,
          class = typename std::enable_if<std::is_same<TP, std::chrono::time_point<std::chrono::system_clock>>::value ||
                                          std::is_same<TP, std::chrono::time_point<std::chrono::steady_clock>>::value>::type
         >
inline long timediff(TP t1, TP t2)
{
    return (long)std::chrono::duration_cast<TU>(t2 - t1).count();
}

