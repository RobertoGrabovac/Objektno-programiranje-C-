**Strukturirano vezanje za korisničke klase**. Ukoliko želite da instance vaše 
klase podržavaju strukturirano vezanje trebate klasi dodati `std::tuple<>` 
API, kao što standardna biblioteka to čini za `std::pair<>`, `std::array<>` i 
`std::tuple<>`. Taj se API sastoji od predloška strukture `std::tuple_size<>`, 
predloška strukture `std::tuple_element<>` i predloška funkcije `get<>`. 

Predlošci  `std::tuple_size<>` i `std::tuple_element<>` definirani su standardnoj 
biblioteci (u imeniku `std`) i mi ih moramo samo specijalizirati za našu klasu.
Recimo da naša klasa ima oblik

```
class MyCl{
    std::string name;
    int         number;
public:
   // ....
};
```

Tada moramo definirati 

```
// Ovo je specijalizacija predloška funkcije iz `<utility>` zaglavlja.
template <>
struct std::tuple_size<MyCl>{
    static constexpr int value =  2;
};
```
`std::tuple_size<>` daje broj članova u struktriranom vezanju kroz statičku `constexpr`
varijablu `value`. U našem slučaju imamo dvije varijable članice koje će obje sudjelovati
u strukturiranom vezanju pa smo definirali  `value` kao 2. 


Predložak strukture `std::tuple_element<>` je također definiran u zaglavlju `<utility>` i uzima dva 
parametra predloška. Prvi parametar predloška je indeks (tipa `std::size_t`) elementa u vezanju, a 
drugi je klasa za koju se vezanje implementira. Predložak treba  kroz varijablu `type`  dati tip 
indeksiranog elementa u vezanju. U našem slučaju definiramo dvije potpune specijalizacije tog 
predloška:

```
// Prvi element je string, a drugi int.
template <>
struct std::tuple_element<0, MyCl>{
     using type = std::string;
};

template <>
struct std::tuple_element<1, MyCl>{
     using type = int;
};
```
Prvi element u strukturiranom vezanju (indeks 0) je `name` i njegov tip je `std::string`; drugi element
(indeks 1) je `number` i njegov tip je `int`. 

Sada su nam potrebna još dva elementa da bi strukturirano vezanje zaživjelo. 
Prvo, klasa mora imati **get metode** koje će vratiti vrijednosti privatnih varijabli budući da se cijela 
konstrukcija oslanja na javno sučelje klase. Mi želimo da kroz strukturirano vezanje možemo mijenjati
našu ciljanu klasu i stoga  _get metode_ moraju vraćati referencu. Kao i uvijek koristimo preopterećenje
po konstantnosti: nekonstantna metoda vraća
običnu (lijevu) referencu, a konstantna metoda vraća konstantnu referencu. Tako bismo imali:


```
class MyCl{
    std::string name;
    int         number;
public:
    std::string       & getName() { return name; }
    std::string const & getName() { return name; }
    int       & getNumber() { return number; }
    int const & getNumber() { return number; }

   // ....
};
```
Posljednji korak je konstrukcija `get<>` predloška funkcija.  `get<I>(MyCl)` funkcija
ima jedan parametar predloška  koji predstavlja indeks elementa u strukturiranom vezanju 
(tip `std::size_t`). Kao argument uzima objekt klase za koju implementiramo strukturirano vezanje,
a vraća vrijednost varijable na poziciji `I` u strukturiranom vezanju. Taj posao obavlja zvanjem _get metoda_
iz klase `MyCl`.  Kako se radi o funkciji, a ne o strukturi, 
mi ne specijaliziramo predloške funkcije `std::get<>` koji već postoje u standardnoj biblioteci (imeniku `std`)
već definiramo nove predloške koji _preopterećuju_ već postojeće.

U implementaciji je važno preopteretiti tri verzije funkcije  `get<>`: onu koja uzima referencu na klasu `MyCl`, 
konstantnu referencu i desnu referencu. Bez sve tri verzije možemo dobiti nefunkcionalni kod. Povratna vrijednost,
s druge strane, mora biti deducirana. `get<>` funkcije uzimaju kao argument privremeni objekt koji prevodilac 
konstruira u memoriji. Taj objekt može biti desna vrijednost ako ne koristimo referencu u strukturiranom vezanju,
ili lijeva vrijednost (referenca na originalni objekt) ako koristimo referencu.  Funkcije  `get<>()` će stoga 
zvati _get metodu_ koja vraća konstantnu referencu ili običnu referencu i njena povratna vrijednost mora biti ili 
objekt (poziv po vrijednosti) ili referenca na objekt. Stoga za deklaraciju povratne vrijednosti koristimo
**decltype(auto)**.

Pogledajmo konačno kod. Trebamo _deklarirati_ tri funkcijska predloška:


```
template <std::size_t N> decltype(auto) get(MyCl const &);
template <std::size_t N> decltype(auto) get(MyCl &);
template <std::size_t N> decltype(auto) get(MyCl &&);
```

Konačno, implementacija funkcija je vrlo jednostavna. Trebamo samo napisati specijalizacije za 
sve indekse. Ako specijalizacije  dolaze u datoteku zaglavlja moraju biti deklarirane `inline`.


```
template <>
inline decltype(auto) get<0>(MyCl & objekt){
    return objekt.getName();
}
template <>
inline decltype(auto) get<1>(MyCl & objekt){
    return objekt.getNumber();
}
// itd...
```

U glavnom programu sada možemo imati:
```
MyCl obj{std::string("Pero"), 333};
auto [ime, broj] = obj;
auto & [refime, refbroj] = obj;
```


**Zadatak**. Zadana je klasa `Log` koja predstavlja logiranu poruku.

```
class Log{
    std::string _sender; 
    int         _priority; 
    std::array<char,3> _code;                            
    std::string _message; 
public:
    Log(std::string sender,  int priority, std::array<char,3> code, 
        std::string message); 
    
   // ostale potrebne metode. 
};
```

Potrebno je dopuniti klasu `Log` i dodati ostale elemente potrebne za strukturirano vezanje. 
U direktoriju `test` se nalaze testovi koji moraju biti zadovoljeni.

Ograničenje: Izvan klase `Log` može se mijenjati samo prioritet poruke. 

