#include "range/v3/all.hpp"  // to je dovoljno za range-v3.
                             // Pojedini elementi se mogu "includati" i pojedinačno. 
#include "fmt/format.h"
#include <iostream>
#include <string>
#include <optional>
#include <string>

using namespace ranges; // Nikako ne uključivati std!

struct Planet {
    std::string name;
    double gravity;
};

int main(){   

    {
     //1. Generirajte niz 1,2,2,3,3,3.......9,9,9,9,9,9,9,9,9.
    auto rng1 = views::for_each(views::ints(1, 10), [](int i) {
      return yield_from(views::repeat_n(i, i));});
    fmt::print("{}\n", fmt::join(rng1, ","));
    }   

    {
    // 2. Nađi planet s gravitacijom 11.15 m²/s. Rezultat vratiti (i ispisati)
    //    kao std::optional<std::string>.
    Planet const v[] = {{"Venus",8.87}, {"Mars",3.711}, {"Jupiter", 24.92},{"Neptun", 11.15},
                        {"Saturn", 10.44}, {"Uran",8.69}};
    auto rng2 = v | views::filter([](auto x) {return x.gravity == 11.15;}) 
                  | views::transform([](auto x) {return std::optional<std::string>{x.name};});
    
    if (rng2.begin() != rng2.end()) {
      std::optional<std::string> res = *(rng2.begin());
      fmt::print("Planet: {}\n", res.value());
    }
    else
      fmt::print("Planet ne postoji!\n");
    }

    {
    // 3. Aproksimirajte broj  π koristeći Gregory - Leibnizovu formulu 
    // π ≈ 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...). Ispišite rezultat s 30 članova.
    auto rng3 = views::iota(1, 61) | views::filter([](auto x){return x%2==1;})
                | views::transform([](auto x){
                  auto a = 4.0 / x;
                  return x % 4 != 1 ? -1 * a : a;
                });

    auto val = accumulate(rng3, 0.);
    fmt::print("π ≈ {}\n", val);
    }

    {
    // 4. Selektirati domenu u web adresi i konverirati ju u string. Ispisati. 
    auto const str = std::string{"https://en.test.org"}; // -> org
    auto rng4 = views::reverse(views::reverse(str) | views::take_while([](char c) {
      return c != '.';
    }));
    fmt::print("domena: {}\n", fmt::join(rng4, ""));
    }
    
    {
      //5.  Pretvoriti CAMELCASE string u SNAKE_CASE.
      std::string str{"ovoJeMojaCamelCaseVarijabla"};
      
      auto rng5 = str | views::transform([](char x) {
        return std::isupper(x) ? std::string("_") + (char)std::tolower(x) : std::string(1, x);
      });
      
      fmt::print("snake case: {}\n", fmt::join(rng5, "")); // ovo_je_moja_camel_case_varijabla
    }

    {
      //6.  Izračunati euklidsku udaljenost između dvije točke zadana tipom std::array<double,n>. 
      std::array<double,4> a{1,2,3,4}, b{4,3,2,1};
      auto rng6 = views::zip_with(std::minus{}, a, b) | views::transform([](auto x) {return x * x;});
      auto val = accumulate(rng6, 0.);
      fmt::print("d(a,b) = {}\n", std::sqrt(val));
    }
	{
    //7.  Provjeriti korektnost ISBN-13 broja (na primjer 9780306406157).
    //    Polazeći s lijeva na desno treba naizmjenično množiti znamenke s 1 odnosno 3.
    //    (Prvu znamenku množimo s 1, drugu s 3, treću s 1 itd. Broj ima 13 znamenaka.) 
	//    Zatim zbrojimo tako dobivenih 
    //    prvih 12 vrijednosti i izračunamo ostatak sume modulo 10. Ako je dobiveni broj nakon toga 
    //    različit od nule, oduzmemo ga od 10. Izračunata vrijednost mora biti jednaka 13. znamenci.
      long int n = 9780306406157;
      long int copy = n;
      int last_digit = n % 10;
      n /= 10;
      auto rng7 = views::iota(0, 12) | views::transform([&n] (int x) {
        int digit = n % 10;
        n /= 10;
        return x % 2 ? digit : 3 * digit;
      });

      auto val = accumulate(rng7, 0);
      int remainder = val % 10;

      if (remainder && 10 - remainder == last_digit)
        fmt::print("{} je korektan ISBN-13 broj\n", copy);
      else
        fmt::print("{} nije korektan ISBN-13 broj\n", copy);  
  }
      
     {
      //8.  Naći i ispisati sve troznamenkaste narcisoidne brojeve. Broj je narcisoidan ako je 
      //    jednak sumi svojih znameni dignutih na treću potenciju. Na primjer, 153 = 1³ + 5³ + 3³.
      auto rng8 = views::iota(100, 1000) | views::filter([](int x) {
        int copy = x;
        auto sub_rng = views::iota(0, 3) | views::transform([&x](int y) {
          int digit = x % 10;
          x /= 10;
          return std::pow(digit, 3);
        });
        return copy == accumulate(sub_rng, 0);
      }); 
      fmt::print("{}\n", fmt::join(rng8, ", "));
	   }
     return 0;
}
