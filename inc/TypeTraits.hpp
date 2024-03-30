#pragma once

#include <type_traits>

namespace tystl {

template <typename T>
struct RemoveReference {
  using Type = T;
};

template <typename T>
struct RemoveReference<T&> {
  using Type = T&;
};

template <typename T>
struct RemoveReference<T&&> {
  using Type = T&&;
};

template <typename T>
using RemoveReferenceType = RemoveReference<T>::Type;

template <typename T1, typename T2>
inline constexpr bool IsSameValue = false;

template <typename T>
inline constexpr bool IsSameValue<T, T> = true;

template <typename T>
inline constexpr bool IsLValueReferenceValue = false;

template <typename T>
inline constexpr bool IsLValueReferenceValue<T&> = true;

template <typename T, typename ...Args>
inline constexpr bool IsConstructibleValue = std::is_constructible_v<T, Args...>;

}
