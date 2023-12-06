#include <iostream>
#include <cassert>
#include <type_traits>
#include <cmath>
#include <cstring>

// Zadatak 1.
// a. prva verzija koja koristi rekurziju i specijalizaciju predloška.
template <unsigned int d, unsigned int k>
struct Dim{
    static constexpr unsigned int value = (Dim<d, k-1>::value) + (Dim<d-1, k>::value); 
};

template<unsigned int d>
struct Dim<d, 0> {
    static constexpr unsigned int value = 1;
};

template<unsigned int d>
struct Dim<0, d> {
    static constexpr unsigned int value = 1;
};

// b. druga verzija koja koristi c++11 constexpr funkciju. 
//    Funkcija se sastoji od samo jedne return naredbe, 
constexpr unsigned int dim(unsigned int d, unsigned int k){
    return ( (d == 0 || k == 0) ? 1 : dim(d, k - 1) + dim(d - 1, k) );
}

// c. treća verzija koja koristi c++14 constexpr funkciju i ne koristi
//  rekurziju. 
constexpr unsigned int dim14(unsigned int d, unsigned int k){
    unsigned int value = 1;

    if (d == 0 || k == 0)
        return value;

    std::size_t i = 1;
    for (i = 1; i <= k; i++)
        value = (value * (d + k + 1 - i) / i);
    
    return value;
}

// Zadatak 2. 
// a. C++11 verzija 

constexpr int64_t hash11(const char * s) {
    constexpr int p = 31;
    constexpr int m = 1e9 + 9;
    return ((*s != '\0') ? (*s -'a' +1 + hash11(s+1)*p) % m : 0);
}

// b. C++14 verzija 
constexpr int64_t power(int base, int exp, int64_t m) {
    int64_t result = 1;
    for (int i = 0; i < exp; i++) {
        result %= m;
        result *= base;
    }
    return result;
}

constexpr int64_t hash14(const char * s) 
{
    int64_t m = 1000000009;
    int p = 31;
    int64_t hash_s = 0;
    int i = 0;

    for (i = 0; i < std::strlen(s); i++)
        hash_s += ( (int64_t)( ((int64_t)s[i] - 96) * power(p, i, m) ) ) % m;

    return hash_s % m;
}

int main(){
        // Ovaj kod ne brisati! Možete dodavati svoje testove.
        // Zadatak 1.
        // 1. 
        static_assert(Dim<0,3>::value == 1);
        static_assert(Dim<3,0>::value == 1);
        static_assert(Dim<3,1>::value == 4);
        static_assert(Dim<3,2>::value == 10);
        static_assert(Dim<3,3>::value == 20);
        static_assert(Dim<5,3>::value == 56);
        // 2. 
        static_assert(dim(0,3) == 1);
        static_assert(dim(3,0) == 1);
        static_assert(dim(3,1) == 4);
        static_assert(dim(3,2) == 10);
        static_assert(dim(3,3) == 20);
        static_assert(dim(5,3) == 56);    
        //3.
        static_assert(dim14(0,3) == 1);
        static_assert(dim14(3,0) == 1);
        static_assert(dim14(3,1) == 4);
        static_assert(dim14(3,2) == 10);
        static_assert(dim14(3,3) == 20);
        static_assert(dim14(5,3) == 56);  
        
        // Zadatak 2.
        constexpr const char * s1="abcder";
        constexpr const char * s2="ertamabcdernn";
        constexpr const char * s3="afgtbbnjiocbfrewqdeloikjuhbvcr";
        constexpr const char * s4="utdtbbnjiocbzrewddegoimjeh"; 
        /* std::cout << hash11(s1) << "\n";
        std::cout << hash11(s2) << "\n";
        std::cout << hash11(s3) << "\n";
        std::cout << hash11(s4) << "\n"; */
        
        static_assert(hash11(s1) == 520064433);
        static_assert(hash11(s2) == 202527068);
        static_assert(hash11(s3) == 615720090);
        static_assert(hash11(s4) == 814096530);

        static_assert(hash14(s1)==hash11(s1));
        static_assert(hash14("ertamabcdernn")==hash11("ertamabcdernn"));
        static_assert(hash14(s3)==hash11(s3));
        static_assert(hash14(s4)==hash11(s4));
        
    return 0;
}
