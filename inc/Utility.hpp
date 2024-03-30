#pragma once

#include "TypeTraits.hpp"
#include <cstddef>
#include <memory>
#include <type_traits>

namespace tystl {

template <typename T>
[[nodiscard]]
constexpr RemoveReferenceType<T>&& Move(T&& value) noexcept {
  return static_cast<RemoveReferenceType<T>&&>(value);
}

template <typename T>
[[nodiscard]]
constexpr T&& Forward(RemoveReferenceType<T>& value) noexcept {
  return static_cast<T&&>(value);
}

template <typename T>
[[nodiscard]]
constexpr T&& Forward(RemoveReferenceType<T>&& value) noexcept {
  static_assert(!IsLValueReferenceValue<T>, "Forward must not be used to convert an rvalue to an lvalue");
  return static_cast<T&&>(value);
}

template <typename T>
constexpr void Swap(T &left, T &right)
  noexcept(std::is_nothrow_swappable_v<T> && std::is_nothrow_move_assignable_v<T>)  {
  
  T tmp = Move(left);
  left = Move(right);
  right = Move(tmp);
}

template <typename T, std::size_t N>
constexpr void Swap(T (&left)[N], T (&right)[N])
  noexcept(std::is_nothrow_swappable_v<T>) {
    
  T *first1 = left;
  T *last1 = first1 + N;
  T *first2 = right;
  for (; first1 != last1; ++ first1, ++ first2) {
    Swap(*first1, first2);
  }
}

template <typename T, typename Deleter>
constexpr void Swap(std::unique_ptr<T, Deleter> &left, std::unique_ptr<T, Deleter> &right) noexcept {
  left.swap(right);
}

}
