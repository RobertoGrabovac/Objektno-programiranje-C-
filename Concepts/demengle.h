#include <cxxabi.h>
#include <string>
#include <stdexcept>

// Dekodiranje imena koje daje typeid operator. 
template <typename T>
std::string demengle_type_name(T&& t){
//    vidi https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
//    char* abi::__cxa_demangle(const char * mangled_name,
//		                        char *  	output_buffer,
//		                        size_t *  	length,
//		                        int *  	status	 
//	                           )
   int status = 0;
   char* dem_name = abi::__cxa_demangle(typeid(t).name(), NULL, NULL, &status);
   if(status <0){
       switch(status){
           case -1: throw std::runtime_error("A memory allocation failiure occurred.");
           case -2: throw 
                    std::runtime_error("Mangled_name is not a valid name under"
                                       " the C++ ABI mangling rules.");
           case -3: throw std::runtime_error("One of the arguments is invalid.");
           default: throw std::runtime_error("Unknown error.");
       }
   } 
   std::string res = dem_name;
   std::free(dem_name);
   return res;
}
