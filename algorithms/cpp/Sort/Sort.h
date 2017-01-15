#pragma once

#ifdef BUILD_DLL
    #define ALGO_API __declspec(dllexport)
#else  
    #define ALGO_API __declspec(dllimport)
#endif  

namespace Sort
{
    ALGO_API int foo(int n);
}