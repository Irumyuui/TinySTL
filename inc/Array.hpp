#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <type_traits>

#include "Utility.hpp"

namespace tystl {

template <typename T>
concept ArrayType = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>; 

template <ArrayType T, std::size_t N>
struct Array {
public:
  using value_type      = T;
  using size_type       = std::size_t;
  using difference_type = ptrdiff_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;
  
public:
  constexpr void Fill(const T &value) {
    std::fill_n(elems_, N, value);
  }

  constexpr void Swap(Array &other) noexcept ( std::is_nothrow_swappable_v<T> ) {
    tystl::Swap(elems_, other.elems_);
  }

  // constexpr auto begin();

  // constexpr auto end();

  // constexpr auto cbegin();

  // constexpr auto cend();

  constexpr size_type Size() const noexcept {
    return N;
  }

  constexpr size_type MaxSize() const noexcept {
    return N;
  }

  constexpr bool Empty() const noexcept {
    return false;
  }

  [[nodiscard]]
  constexpr reference At(size_type pos) {
    if (N <= pos) {

    }
    return elems_[pos];
  }

  [[nodiscard]]
  constexpr const_reference At(size_type pos) const {
    if (N <= pos) {

    }
    return elems_[pos];
  }

  [[nodiscard]]
  constexpr reference operator[](size_type pos) noexcept {
    return elems_[pos];
  }

  [[nodiscard]]
  constexpr const_reference operator[](size_type pos) const noexcept {
    return elems_[pos];
  }

  [[nodiscard]]
  constexpr reference Front() noexcept {
    return elems_[0];
  }

  [[nodiscard]]
  constexpr const_reference Front() const noexcept {
    return elems_[0];
  }

  [[nodiscard]]
  constexpr reference Back() noexcept {
    return elems_[N - 1];
  }

  [[nodiscard]]
  constexpr const_reference Back() const noexcept {
    return elems_[N - 1];
  }

  [[nodiscard]]
  constexpr pointer Data() noexcept {
    return elems_;
  }

  [[nodiscard]]
  constexpr const_pointer Data() const noexcept {
    return elems_;
  }

  constexpr auto operator<=>(const Array &right) const {
    for (size_type i = 0; i < N; i ++) {
      if (auto result = elems_[i] <=> right[i]; result == 0) {
        continue;
      } else {
        return result;
      }
    }
    return std::strong_ordering::equal;
  }

public:
  T elems_[N];
};

template <typename T>
struct Array<T, 0> {
public:
  using value_type      = T;
  using size_type       = std::size_t;
  using difference_type = ptrdiff_t;
  using pointer         = T*;
  using const_pointer   = const T*;
  using reference       = T&;
  using const_reference = const T&;

public:
  constexpr void Fill(const T&) {}

  constexpr void Swap(Array&) noexcept {}

  constexpr size_type Size() const noexcept {
    return 0;
  }

  constexpr size_type MaxSize() const noexcept {
    return 0;
  }

  constexpr bool Empty() const noexcept {
    return true;
  }

  [[noreturn]]
  reference At(size_type) {}

  [[noreturn]]
  const_reference At() const {}

  constexpr reference operator[](size_type) noexcept {
    return *Data();
  }

  constexpr const_reference operator[](size_type) const noexcept {
    return *Data();
  }

  constexpr reference Front() noexcept {
    return *Data();
  }

  constexpr const_reference Front() const noexcept {
    return *Data();
  }

  constexpr reference Back() noexcept {
    return *Data();
  }

  constexpr const_reference Back() const noexcept {
    return *Data();
  }
  
  constexpr pointer Data() noexcept {
    return nullptr;
  }

  constexpr const_pointer Data() const noexcept {
    return nullptr;
  }
};

template <typename T, typename ...Ts>
Array(T, Ts...) -> Array<T, 1 + sizeof...(Ts)>;

} // namespace tystl
