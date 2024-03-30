#pragma once

#include "Utility.hpp"
#include <type_traits>
#include <utility>
namespace tystl {

template <typename T>
struct DefaultDeleter {
  constexpr DefaultDeleter() noexcept = default;

  template <typename T2>
    requires std::is_convertible_v<T2*, T*>
  constexpr DefaultDeleter(const DefaultDeleter<T2> &) noexcept {}

  constexpr void operator()(T *value) const noexcept {
    delete value;
  }
};

template <typename T>
struct DefaultDeleter<T[]> {
  constexpr DefaultDeleter() noexcept = default;

  template <typename T2>
    requires std::is_convertible_v<T2(*)[], T(*)[]>
  constexpr DefaultDeleter(const DefaultDeleter<T2[]> &) noexcept {}

  template <typename T2>
    requires std::is_convertible_v<T2(*)[], T(*)[]>
  constexpr void operator()(T2* ptr) const noexcept {
    delete[] ptr;
  }

};

template <typename T, typename Deleter = DefaultDeleter<T>>
  requires std::is_invocable_v<Deleter, T*>
class UniquePtr {
public:
  using Pointer     = T*;
  using ElementType = T;
  using DeleterType = Deleter;

public:
  constexpr UniquePtr() noexcept : ptr_(nullptr), deleter_() {}

  constexpr explicit UniquePtr(T* ptr) noexcept : ptr_(ptr), deleter_() {}

  constexpr explicit UniquePtr(T* ptr, Deleter &&deleter) noexcept : ptr_(ptr), deleter_(std::move(deleter)) {}

  constexpr explicit UniquePtr(T* ptr, const Deleter &deleter) noexcept : ptr_(ptr), deleter_(deleter) {}

  constexpr UniquePtr(const UniquePtr&) = delete;

  constexpr UniquePtr(UniquePtr &&other) noexcept 
    : ptr_(std::exchange(other.ptr_, nullptr)),
      deleter_(std::exchange(other.deleter_, {})) {}

  constexpr UniquePtr& operator=(UniquePtr other) noexcept {
    this->Swap(other);
    return *this;
  }

  constexpr ~UniquePtr() {
    if (ptr_) {
      deleter_(ptr_);
    }
  }

  constexpr void Swap(UniquePtr &other) noexcept {
    tystl::Swap(ptr_, other.ptr_);
    tystl::Swap(deleter_, other.deleter_);
  }

  constexpr void Reset(Pointer ptr = nullptr) noexcept {
    if (ptr_) {
      deleter_(ptr_);
    }
    ptr_ = ptr;
  }

  [[nodiscard]]
  constexpr Pointer Release() noexcept {
    return std::exchange(ptr_, nullptr);
  }

  [[nodiscard]]
  constexpr Pointer Get() const noexcept {
    return ptr_;
  }

  [[nodiscard]]
  constexpr Deleter& GetDeleter() noexcept {
    return deleter_;
  }
  
  [[nodiscard]]
  constexpr const Deleter& GetDeleter() const noexcept {
    return deleter_;
  }

  constexpr explicit operator bool() const noexcept {
    return ptr_ != nullptr;
  }

  [[nodiscard]]
  constexpr T& operator*() const noexcept(noexcept(*std::declval<Pointer>())) {
    return *ptr_;
  }

  [[nodiscard]]
  constexpr Pointer operator->() const noexcept {
    return ptr_;
  }

private:
  Pointer ptr_;
  Deleter deleter_;
};

template <typename T, typename ...Args>
  requires std::is_constructible_v<T, Args...>
[[nodiscard]]
constexpr UniquePtr<T> MakeUnique(Args &&...args) {
  return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

}
