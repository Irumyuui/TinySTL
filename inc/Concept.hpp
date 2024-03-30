#pragma once

#include "TypeTraits.hpp"
#include <type_traits>

namespace tystl {

template <typename T1, typename T2>
concept SameAs = IsSameValue<T1, T2>;

template <typename T>
concept NothrowMoveAssign = std::is_nothrow_move_assignable_v<T>;

}
