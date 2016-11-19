#ifndef _FRAMEWORK_TUPLE_ALGORITHMS_H_
#define _FRAMEWORK_TUPLE_ALGORITHMS_H_

namespace utils {

/**
 * Create tuple from homongenous flat container
 */
template <typename T, size_t... I>
decltype(auto) make_tuple(const T& a, std::index_sequence<I...>) {
  return std::make_tuple(a[I]...);
}

namespace impl {
// Tuple for_each
template <typename Tuple, typename F, std::size_t... Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
  using swallow = int[];
  (void)swallow{
      1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...};
}
}  // end namespace impl

/**
 * Apply functor to each element of a tuple.
 */
template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
  constexpr std::size_t N =
      std::tuple_size<std::remove_reference_t<Tuple>>::value;
  impl::for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                      std::make_index_sequence<N>{});
}

}  // end of namespace utils

/**
 * Tuple cout functionality.
 */
template <typename... T>
std::ostream& operator<<(std::ostream& stream, const std::tuple<T...>& tuple) {
  utils::for_each(tuple, [&stream](const auto& val) { stream << val << " "; });
  return stream;
}

#endif  //_FRAMEWORK_TUPLE_ALGORITHMS_H_
