**Zadatak**. U ovom zadatku treba napisati niz metafunkcija. 

-  Napisati metafunkciju `RemoveReference` koja uklanja referencu 
   s danog tipa ( std to radi `remove_reference<>`). Dodati i `_t` alias template.
   Ne koristiti metafunkcije iz zaglavlja `<type_traits>`.
-  Napisati metafunkciju `AddLValueReference` koja na tip dodaje lijevu referencu.
   Za tip `void`  vraća `void` jer ne postoji    referenca na `void`. Dodati i `_t` alias template.
   Ne koristiti metafunkcije iz zaglavlja `<type_traits>`.
-  Napisati metafunkciju `IsArray` koja vraća `true` ako je tip polje, a inače vraća `false`. 
   Uključiti ograničena polja (tip T[N]) i neograničena polja (tip T[]). Dodati i predložak vatijable `_v`.
   Ne koristiti metafunkcije iz zaglavlja `<type_traits>`.
-  Definirati operator zbrajanja općenito različitih polja `std::array<T>` i
   `std::array<S>`. Definirati povratnu vrijednost operatora kao `std::array<V>` 
    tako da `V` bude egzaktno tip koji proizvodi zbrajanje objekta tipa `T` i `S`.
    Iz standardne biblioteke koristiti samo `std::declval<>`.
-  Potrebno je napisati metafunkciju `IsBaseOf<B,D>` (u STL-u `std::is_base_of<B,D>`)
   koja ispituje je li klasa `B` **javna bazna** klasa za `D`. 
    Temelj na kojem se bazira razlikovanje je preopterećenje funkcija. Ako imamo
    pokazivač na klasu `D` tada je njegova konverzija u pokazivač na baznu klasu 
    bolja od konverzije u pokazivač na `void`. Na taj način možemo razlikovati klasu 
    koja ima odgovarajuću bazu od one koja ju nema.  
    Iz `<type_traits>` koristiti samo `std::is_class<>` te `std::true_type` i `std::false_type`. 
    Ne kopirati rješenje iz cppreference.com (0 bodova) koje funkcionira i s privatnom bazom i 
    s ambivalentnom bazom!
-  Konstruirati predložak varijable `hasConstReverseIterator_v<Container>`
  koja daje `true` ako `Container` definira tip `const_reverse_iterator`. 
  Inače vraća `false`.  Zatim napisati metafunkciju (struct) `ConstReverseIteratorType<Container>`   koja kroz svoj tip `type` (alijas) eksportira tip `Container::const_reverse_iterator`
  ako `Container` eksportira tip `const_reverse_iterator`. Ako tip `Container::const_reverse_iterator`   ne postoji `ConstReverseIteratorType<Container>` ne definira alijas `type`. Iz standarne biblioteke koristiti samo  `std::true_type` i `std::false_type`. 
-  Treba napisati predložak varijable `hasMember_x<T>` koji za dani tip `T`
  daje `true` ako `T` ima varijablu članicu imena `x`. Kada nema, ili kada ima funkciju članicu imena `x`, vraća `false`. Iz standarne biblioteke koristiti samo  `std::true_type` i `std::false_type`.

Napomena. `std::declval` se može slobodno koristiti.
