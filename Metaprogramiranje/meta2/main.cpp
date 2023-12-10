#include <fmt/format.h>
#include <type_traits>
#include <array>
#include <cassert>
#include <cstdint> // za (u)intptr_t
#include <vector>
#include <unordered_set>

template <typename T> 
struct TypeIdentity1{
    using type = T;
};

template <typename T> 
struct TypeIdentity2{
    using type = T&;
};

// 1. metafunkcija
template <typename T>
struct RemoveReference : TypeIdentity1<T> {};

template <typename T>
struct RemoveReference<T &> : TypeIdentity1<T> {};

template <typename T>
struct RemoveReference<T &&> : TypeIdentity1<T> {};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;

// 2. metafunkcija
template <typename T>
struct AddLValueReference : TypeIdentity2<T> {};

template <>
struct AddLValueReference<void> {
    using type = void;
};

template <typename T>
using AddLValueReference_t = typename AddLValueReference<T>::type;

// 3. metafunkcija
template <typename T>
struct IsArray {
    static constexpr bool value = false;
};

template <typename T>
struct IsArray<T[]> {
    static constexpr bool value = true;
};

template <typename T, std::size_t N>
struct IsArray<T[N]> {
    static constexpr bool value = true;
};

template <typename T>
inline constexpr bool IsArray_v = IsArray<T>::value;

// 4. metafunkcija (operator zbrajanja)
template <typename T, typename S, std::size_t N>
auto operator+(const std::array<T, N>& arr1, const std::array<S, N>& arr2) -> std::array<decltype(std::declval<T>() + std::declval<S>()), N> {
    std::array<decltype(std::declval<T>() + std::declval<S>()), N> result;
    for (std::size_t i = 0; i < N; ++i) {
        result[i] = arr1[i] + arr2[i];
    }
    return result;
}

// 5. metafunkcija
namespace Implementation {
    template <typename T>
    std::true_type test(const volatile T*);
    template<typename>
    std::false_type test(const volatile void*);

    template <typename...>
    using my_void_t = void;
}

template <typename B, typename D>
struct IsBaseOf {
    static constexpr bool value = std::is_class_v<B> && std::is_class_v<D> && decltype(Implementation::test<B>(std::declval<D*>()))::value;
};

// 6. metafunkcija
template <typename Container, typename = Implementation::my_void_t<> >
struct hasConstReverseIterator : std::false_type {};

template <typename Container>
struct hasConstReverseIterator<Container, Implementation::my_void_t<typename Container::const_reverse_iterator>> : std::true_type {};

template <typename Container>
constexpr inline bool hasConstReverseIterator_v = hasConstReverseIterator<Container>::value;

template <typename Container, bool = hasConstReverseIterator_v<Container> >
struct ConstReverseIteratorType {
  using type = typename Container::const_reverse_iterator;
};

template <typename Container>
struct ConstReverseIteratorType<Container, false> {};

// 7. metafunkcija
template <typename T, typename = Implementation::my_void_t<> >
struct HasMember_x : std::false_type {};

template <typename T>
struct HasMember_x<T, Implementation::my_void_t<decltype((void)T::x)>> : std::true_type {};

template <typename T>
constexpr bool hasMember_x = HasMember_x<T>::value;

// Klase za testiranje
class B {};
class D : public B {};
class C{};

class E : public B, public C {};

class Base{
    public:
    virtual ~Base() = default;
};
class Derived : public Base {};


class AAA{
    public:
        int x;
};
class BBB{
    public:
        void x() const {}
};
class CCC{
    int y;
};


// Ne mijenjati main!
int main(){
    // 1. 
    static_assert(std::is_same_v<RemoveReference_t<int &>, int>);
    static_assert(std::is_same_v<RemoveReference_t<int &&>, int>);
    static_assert(std::is_same_v<RemoveReference_t<int const &>, int const>);
    
    // 2. 
    static_assert(std::is_same_v<AddLValueReference_t<int>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<int&>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<int&&>, int&>);
    static_assert(std::is_same_v<AddLValueReference_t<void>, void>);
    
    // 3. 
    static_assert(IsArray_v<int> == false);
    static_assert(IsArray_v<int[]> == true);
    static_assert(IsArray_v<int[3]> == true);
    
    // 4.
    std::array<int,3> a{1,2,3};
    std::array<float,3> b{1.0f, 2.0f, 3.0f};
    auto c = a+b;
    assert(c[0] == 2);
    assert(c[1] == 4);
    assert(c[2] == 6);
    static_assert(std::is_same_v<std::decay_t<decltype(c[0])>, float>); 
    
    // 5. 
    static_assert( IsBaseOf<B,D>::value );
    static_assert( !IsBaseOf<B,C>::value );
    static_assert( IsBaseOf<B,B>::value );
    static_assert( !IsBaseOf<int,long>::value );
    static_assert( !IsBaseOf<void,void>::value );
    static_assert( !IsBaseOf<void,B>::value );

    static_assert( IsBaseOf<const B,D>::value );
    static_assert( IsBaseOf<B, volatile D>::value );
    
    static_assert( IsBaseOf<B,E>::value );
    static_assert( IsBaseOf<C,E>::value );
    
    // 6. 
    static_assert(hasConstReverseIterator_v<std::vector<int>>);
    static_assert(!hasConstReverseIterator_v<std::unordered_set<int>>);

    ConstReverseIteratorType<std::vector<int>>::type it;
//    ConstReverseIteratorType<std::unordered_set<int>>::type it1;  // ne kompilira se
    
    // 7. 
    static_assert(hasMember_x<AAA>);
    static_assert(!hasMember_x<BBB>);
    static_assert(!hasMember_x<CCC>);
    
   
    return 0;
}
