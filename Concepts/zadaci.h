#include "demengle.h"

#include <iostream>
#include <concepts>
#include <type_traits>
#include <span>

/* Zadatak 1. Napisati koncept FPCont koji ispituje da li tip T 
 *            ima metode begin() i end(), da li se povratna vrijednost od 
 *            begin() dade dereferncirati te da li daje nešto konvertibilno u 
 *            tip double. 
 */

template <typename T>
concept FPCont = requires (T obj) {
    obj.begin();
    obj.end();
    {*(obj.begin())} -> std::convertible_to<double>;
};

// Klase X,Y i Z služe za testiranje Zadatka 1.
class X{};

struct Y{
    void begin(){}
    void end(){}
};

struct Z{
    float * begin(){ return nullptr;}
    float * end(){ return nullptr; }
};

// Zadatak 2.  Napraviti koncept Same<T,S> koji je istina ako su 
//             tipovi T i S jednaki nakon zanemarivanja dekoracija const i
//             volatile. 

template <typename T, typename S>
concept Same = std::is_same_v<std::remove_const_t<std::remove_volatile_t<T>>, std::remove_const_t<std::remove_volatile_t<S>> >;


// Zadatak 3. Napisati koncept Number koji je istinit za sve cjelobrojne 
//          tipove i sve tipove s pokretnim zarezom, osim za tip bool
//          i sve moguće char tipove. (bool i char-ovi su cjelobrojni tipovi).
//          Ograničeni predložak mult() služi za testiranje koncepta.
//

// Neograničeni predložak funkcije mult() - samo kako ne bi imali grešku pri kompilaciji.
// UPUTA: Ako demengle_type_name(x) ne radi na vašem sustavu, zamijenite 
//        ga s typeid(x).name(). Isto za y.

template <typename T>
concept IsCharacterType = std::is_same_v<T, char> ||
                          std::is_same_v<T, unsigned char> ||
                          std::is_same_v<T, wchar_t> ||
                          std::is_same_v<T, char8_t> ||
                          std::is_same_v<T, char16_t> ||
                          std::is_same_v<T, char32_t>;

template <typename T>
concept Number = (std::integral<T> || std::floating_point<T>) && !IsCharacterType<T> && !std::is_same_v<T, bool>;

auto mult(auto x, auto y){
    std::cerr << "Greška. Tip od x = " << demengle_type_name(x)
              << ". Tip od y = " << demengle_type_name(y) << "\n";
    return -1;
}

auto mult(Number auto x, Number auto y) {
    std::cerr << "Ograniceni predlozak. Tip od x = " << demengle_type_name(x)
              << ". Tip od y = " << demengle_type_name(y) << "\n";
    return x * y;
} 


// ZADATAK 4. Napraviti koncept Pointer koji predstavlja pointer. Pri tome je pointer (po
//          našoj definiciji)  "void *" ili nešto što se dade dereferencirati. 
//          Napravite predložak "print(auto const & val)" koji ispisuje vrijednost val 
//          te napravite preopterećenje predloška print() koje uzima pokazivač val i ispisuje *val. 
//          Ispravno preopterećenje postići koristeći koncept Pointer. 

template <typename T>
concept Pointer = std::is_same_v<T, void*> || requires (T p) {
    *p;
};

void print(auto const &val) {
    std::cout << "Predlozak print(): " << val << std::endl;
}

void print (Pointer auto const &val) {
    std::cout << "Preopterecenje predloska print(): " << *val << std::endl;
}

// ZADATAK 5.
// Treba napisati koncept Two koji proizvoljan niz argumenata zadovoljava ako svi argumenti 
// pripadaju zadanom skupu od dva tipa int i char.
// 

template <typename... Args>
concept Two = (... && (std::is_same_v<Args, int> || std::is_same_v<Args, char>));

// Funkcija koja može uzeti proizvoljan broj parametara ali samo 
// tipa int i char (u bilo kojem poretku).
template<typename... Args>
requires Two<int, char, Args...>
void f1(Args... args){
	std::cout << "OK\n";
}

/* Zadatak 6. Napisati koncept InPlaceEngine koji provjerava da tip
 *          ima metodu process() koja kao argument uzima std::span<double>.
 *          Klasa Wrapper je parametrizirana s tipom koji mora imati 
 *          metodu process(). Zamijenite opći predložak Wrapper s 
 *          predloškom ograničenim na tipove koji zadovoljavaju 
 *          koncept InPlaceEngine. Implementirajte metodu Wrapper::reset()
 *          tako da poziva Engine::reset() ako takva metoda postoji,
 *          a ne radi ništa ako ne postoji. Pri tome koristiti
 *          samo if constexpr i requires!
 */

template <typename T>
concept InPlaceEngine = requires (T obj) {
    obj.process(std::span<double> {});
};

// Polazni kod klase Wrapper - neograničeni predložak
template <InPlaceEngine Engine>
class Wrapper{
    public:
        Wrapper(Engine e) : engine_(std::move(e)) {}
        void process(std::span<double> buf){
            engine_.process(buf);
        }
        void reset(){
            if constexpr (requires (Engine obj) {
                obj.reset();
            }) engine_.reset();
            else std::cout << "reset ne postoji\n";
        }
    private:
        Engine engine_;
};

// Engine klase za testiranje.
struct Engine1{
    void process(std::span<double> sp){ std::cout << "process\n"; }
};
struct Engine2{
    void process(std::span<double> sp){ std::cout << "process\n"; }
    void reset() const { std::cout << "resret\n"; }
};
struct Engine3{
};