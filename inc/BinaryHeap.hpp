#include "Utility.hpp"
#include <vector>

namespace tystl {

template <typename Ty, typename Compare, typename Container = std::vector<Ty>>
  requires requires(Compare comp, Ty a, Ty b) {
    { comp(a, b) } -> std::same_as<bool>;
  } &&
           requires(Container cont, Ty value) {
             { cont[0] } -> std::same_as<Ty &>;
             { cont.front() } -> std::same_as<Ty &>;
             { cont.back() } -> std::same_as<Ty &>;
             cont.push_back(value);
             cont.emplace_back(value);
             cont.emplace_back(std::move(value));
             cont.pop_back();
           } && requires(const Container cont) {
             { cont[0] } -> std::same_as<const Ty &>;
             { cont.front() } -> std::same_as<const Ty &>;
             { cont.back() } -> std::same_as<const Ty &>;
             { cont.size() } -> std::same_as<std::size_t>;
             { cont.empty() } -> std::same_as<bool>;
           }
class BinaryHeap {
public:
  BinaryHeap() : cont_(), comp_() {}
 
  explicit BinaryHeap(const Compare &comp) : cont_(), comp_(comp) {}
 
  explicit BinaryHeap(Compare comp) : cont_(), comp_(tystl::Move(comp)) {}
 
  BinaryHeap(const BinaryHeap &other)
      : cont_(other.cont_), comp_(other.comp_) {}
 
  BinaryHeap(BinaryHeap &&other) noexcept
      : cont_(tystl::Move(other.cont_)), comp_(tystl::Move(other.comp_)) {}
 
  template <typename Comp, typename Cont>
  explicit BinaryHeap(Comp &&comp, Cont &&cont)
      : cont_(tystl::Forward<Cont>(cont)), comp_(tystl::Forward<Comp>(comp)) {
    for (int i = static_cast<int>(this->cont_.size()) - 1; i >= 0; i--) {
      this->Down(i);
    }
  }
 
  auto operator=(BinaryHeap other) -> BinaryHeap & {
    this->Swap(other);
    return *this;
  }
 
  ~BinaryHeap() noexcept = default;
 
  auto Swap(BinaryHeap &other) noexcept -> void {
    tystl::Swap(this->cont_, other.cont_);
    tystl::Swap(this->comp_, other.comp_);
  }
 
public:
  auto Size() const noexcept -> std::size_t { return this->cont_.size(); }
 
  auto Empty() const noexcept -> bool { return this->cont_.empty(); }
 
  auto Push(Ty value) -> void {
    this->cont_.emplace_back(std::move(value));
    this->Up(this->Size() - 1);
  }
 
  template <typename... Ts>
    requires requires(Ts &&...args) { Ty(std::forward<Ts>(args)...); }
  auto Emplace(Ts &&...args) -> void {
    this->cont_.emplace_back(std::forward<Ts>(args)...);
    this->Up(this->Size() - 1);
  }
 
  auto Top() const -> const Ty & { return this->cont_[0]; }
 
  auto Top() -> Ty & { return this->cont_[0]; }
 
  auto Pop() -> void {
    tystl::Swap(this->cont_.front(), this->cont_.back());
    this->cont_.pop_back();
    this->Down(0);
  }
 
private:
  auto Up(std::size_t idx) -> void {
    while (idx > 0 &&
           this->comp_(this->cont_[idx], this->cont_[(idx - 1) / 2])) {
      tystl::Swap(this->cont_[(idx - 1) / 2], this->cont_[idx]);
      idx = (idx - 1) / 2;
    }
  }
 
  auto Down(std::size_t idx) -> void {
    while (idx * 2 + 1 < this->Size()) {
      auto t = idx * 2 + 1;
      if (t + 1 < this->Size() &&
          this->comp_(this->cont_[t + 1], this->cont_[t])) {
        t += 1;
      }
      if (this->comp_(this->cont_[idx], this->cont_[t])) {
        break;
      }
      tystl::Swap(this->cont_[idx], this->cont_[t]);
      idx = t;
    }
  }
 
private:
  Container cont_;
  Compare comp_;
};
 
template <typename Comp, typename Cont>
BinaryHeap(Comp &&, Cont &&)
    -> BinaryHeap<typename Cont::value_type, Comp, Cont>;
    
}

