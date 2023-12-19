**Zadatak**. Cilj ovog zadatka je pisanje koncepata (uvedenih u standardu C++20):

- koncept `FPCont` koji ispituje da li tip `T` ima metode `begin()` i `end()` i da li se povratna vrijednost od `begin()` može dereferncirati u nešto konvertibilno u tip `double`.

- koncept `Same<T,S>` koji je istina ako su tipovi `T` i `S` jednaki nakon zanemarivanja dekoracija `const` i `volatile`.

- koncept `Number` koji je istinit za sve cjelobrojne tipove i sve tipove s pokretnim zarezom, osim za tip `bool` i sve moguće `char` tipove. (`bool` i `char`-ovi su cjelobrojni tipovi). Ograničeni predložak `mult()` služi za testiranje koncepta.

- koncept `Pointer` koji predstavlja pointer. Pri tome je pointer (po našoj definiciji)  `void *` ili nešto što se dade dereferencirati.  Napravite predložak `print(auto const & val)` koji ispisuje vrijednost `val` te napravite preopterećenje predloška `print()` koje uzima pokazivač `val` i ispisuje `*val`. Ispravno preopterećenje postići koristeći koncept `Pointer`. 

- koncept `Two` koji proizvoljan niz argumenata zadovoljava ako svi argumenti pripadaju zadanom skupu od dva tipa: int i char. Da je koncept ispravan provjeriti pomoću predloška `f1()`.

- koncept `InPlaceEngine` koji provjerava da tip ima metodu `process()` koja kao argument uzima `std::span<double>`. Klasa `Wrapper` je parametrizirana tipom `Engine` koji mora imati metodu `process()`. Zamijenite opći predložak `Wrapper` s predloškom ograničenim na tipove koji zadovoljavaju koncept `InPlaceEngine`. Implementirajte metodu `Wrapper::reset()` tako da poziva `Engine::reset()` ako takva metoda postoji, a ne radi ništa ako metoda `reset()` ne postoji u klasi `Engine`. Pri tome koristiti samo `if constexpr` i `requires`!


