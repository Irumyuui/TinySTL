#pragma once

#include "Utility.hpp"
#include <memory>
#include <utility>
namespace tystl {

  class Any {
  private:
    struct Base {
      constexpr virtual ~Base() noexcept = default;

      constexpr virtual std::unique_ptr<Base> Clone() = 0;
    };

    template <typename T>
    struct Derive : Base {
      constexpr Derive(const T &value) : elem_(value) {}      

      [[nodiscard]]
      constexpr std::unique_ptr<Base> Clone() override {
        return std::make_unique<Derive>(elem_);
      }

      template <typename ...Args>
        requires requires (Args ...args) { T(args...); }
      [[nodiscard]]
      static constexpr std::unique_ptr<Base> MakeData(Args &&...args) {
        return std::make_unique<Derive>(std::forward<Args>(args)...);
      }

      T elem_;
    };

  public:
    constexpr Any() noexcept : data_(nullptr) {}

    template <typename T>
    constexpr Any(const T &value) : data_(Derive<T>::MakeData(value)) {}

    constexpr Any(const Any &other) {
      if (other.data_ != nullptr) {
        data_ = other.data_->Clone();
      } else {
        data_ = nullptr;
      }
    }

    constexpr Any(Any &&other) noexcept : data_(std::exchange(other.data_, nullptr)) {}

    constexpr Any& operator=(const Any &other) {
      Any tmp = other;
      tmp.Swap(*this);
      return *this;
    }

    constexpr Any& operator=(Any &&other) noexcept {
      data_ = std::exchange(other.data_, nullptr);
      return *this;
    }

    void Swap(Any &other) noexcept {
      tystl::Swap(data_, other.data_);
    }

    constexpr bool HasValue() const noexcept {
      return data_ != nullptr;
    }

    template <typename T, typename ...Args>
      requires requires (Args ...args) { T(args...); }
    constexpr void Emplace(Args &&...args) {
      data_ = Derive<T>::MakeData(std::forward<Args>(args)...);
    }

    constexpr void Reset() {
      data_ = nullptr;
    }

    template <typename T>
    [[nodiscard]]
    T* Cast() const {
      if (auto ptr = dynamic_cast<Derive<T>*>(data_.get())) {
        return std::addressof(ptr->elem_);
      }
      return nullptr;
    } 

  private:
    std::unique_ptr<Base> data_;
  };

  template <typename T, typename ...Args>
    requires requires (Args ...args) { T(args...); }
  [[nodiscard]]
  constexpr Any MakeAny(Args &&...args) {
    return Any(T(std::forward<Args>(args)...));
  }
}
