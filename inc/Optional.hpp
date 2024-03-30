#pragma once

#include <array>
#include <cstdint>

namespace tystl {

struct Nullopt {
  Nullopt() = default;
} inline none;

template <typename Ty>
class Optional {
private:
  using TDataByte = std::array<std::uint8_t, sizeof(Ty)>;
  using TValue = Ty;

public:
  constexpr Optional() noexcept : has_value_{false} {}

  constexpr Optional(Nullopt) noexcept : Optional() {}

  constexpr Optional(const Ty &init) : has_value_{true} {
    this->ConstructData(init);
  }

  constexpr Optional(Ty &&init) : has_value_{true} {
    this->ConstructData(std::move(init));
  }

  constexpr Optional(const Optional &other) : has_value_{other.has_value_} {
    if (other.has_value_)
      this->ConstructData(*other);
  }

  constexpr Optional(Optional &&other) noexcept
      : data_(std::move(other.data_)), has_value_{std::move(other.has_value_)} {
    other.has_value_ = false;
  }

  constexpr auto operator=(const Optional &other) -> Optional & {
    this->Reset();
    if (other.has_value_)
      this->ConstructData(*other);
    this->has_value_ = other.has_value_;
  }

  constexpr auto operator=(Optional &&other) noexcept {
    this->Reset();
    this->data_ = std::move(other.data_);
    this->has_value_ = other.has_value_;
    other.has_value_ = false;
  }

  constexpr ~Optional() noexcept {
    this->ReinterpretCastData()->~Ty();
    this->has_value_ = false;
  }

public:
  constexpr auto HasValue() const noexcept -> bool { return this->has_value_; }

  constexpr explicit operator bool() const noexcept { return this->HasValue(); }

public:
  constexpr auto operator->() const noexcept -> const Ty * {
    return this->ReinterpretCastData();
  }

  constexpr auto operator->() noexcept -> Ty * {
    return this->ReinterpretCastData();
  }

  constexpr auto operator*() const & noexcept -> const Ty & {
    return *this->ReinterpretCastData();
  }

  constexpr auto operator*() & noexcept -> Ty & {
    return *this->ReinterpretCastData();
  }

  constexpr auto operator*() const && noexcept -> const Ty && {
    return std::move(*this->ReinterpretCastData());
  }

  constexpr auto operator*() && noexcept -> Ty && {
    return std::move(*this->ReinterpretCastData());
  }

public:
  template <typename... Args> constexpr auto Emplace(Args &&...args) -> void {
    if (this->has_value_) {
      this->Reset();
    }
    this->ConstructData(std::forward<Args>(args)...);
  }

  constexpr auto Reset() noexcept -> void {
    if (this->has_value_) {
      this->ReinterpretCastData()->~Ty();
    }
  }

  constexpr auto Swap(Optional &other) noexcept -> void {
    std::swap(this->data_, other.data_);
    std::swap(this->has_value_, other.has_value_);
  }

  constexpr auto Value() const & -> const Ty & {
    if (this->has_value_) {
      return **this;
    }
    throw std::exception();
  }

  constexpr auto Value() const && -> const Ty && {
    if (this->has_value_) {
      return std::move(**this);
    }
    throw std::exception();
  }

  template <typename U>
  constexpr auto ValueOr(U &&default_value) const & -> Ty {
    if (this->has_value_) {
      return **this;
    }
    return static_cast<Ty>(default_value);
  }

  template <typename U> constexpr auto ValueOr(U &&default_value) && -> Ty {
    if (this->has_value_) {
      return std::move(**this);
    }
    return static_cast<Ty>(default_value);
  }

private:
  template <typename... Args>
    requires requires(Args... args) { new Ty(args...); }
  constexpr auto ConstructData(Args &&...args) -> void {
    new (this->data_.data()) Ty(std::forward<Args>(args)...);
  }

  constexpr auto ReinterpretCastData() noexcept -> Ty * {
    return reinterpret_cast<Ty *>(this->data_.data());
  }

  constexpr auto ReinterpretCastData() const noexcept -> const Ty * {
    return reinterpret_cast<const Ty *>(this->data_.data());
  }

private:
  TDataByte data_;
  bool has_value_;
};

} // namespace tystl
