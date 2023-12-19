#include "zadaci.h"
#include <iostream>
#include <vector>

int main()
{
    // Zadatak 1.
    static_assert(!FPCont<X>);
    static_assert(!FPCont<Y>);
    static_assert(FPCont<Z>);
    static_assert(FPCont< std::vector<int> >);
    
    // Zadatak 2. 
    static_assert(Same<int, const int>);
    static_assert(Same<volatile int, int>);
    
    // Zadatak 3. 
    {
        int x = 2;
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }

     {
        const long x = 2;
        long double y = 2.0;
        std::cout << mult(x,y) << "\n"; 
    }

      {
        short x = 2;
        unsigned long int y = 2.;
        std::cout << mult(x,y) << "\n"; 
    }


    {
        char x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }

    {
        double x = 2.0;
        bool y = true;
        std::cout << mult(x,y) << "\n"; 
    }
 
    {
        unsigned char x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }
    {
        char8_t x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }
    {
        char16_t x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }
    {
        char32_t x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }
    {
        wchar_t x = '2';
        float y = 2.0f;
        std::cout << mult(x,y) << "\n"; 
    }

    
    // Zadatak 4. 
    int i=10;
    print(i);  // ispisuje vrijednost val
    print(&i); // ispisuje vrijednost *val
    int const * pi = &i;
    print(pi);// ispisuje vrijednost *val
    
    // Zadatak 5.
    f1(1, 2, 'v', 9, 'b'); 
   // f('a',9.0f,'b','c',5.1f);  // greška pri kompilaciji
    
    // Zadatak 6.
    std::span<double> sp;
    Engine1 e1;
    Wrapper<Engine1> w1(e1);
    w1.process(sp);
    w1.reset();

    Engine2 e2;
    Wrapper<Engine2> w2(e2);
    w2.process(sp);
    w2.reset();

    static_assert(!InPlaceEngine<Engine3>);
    
    return 0;
}
