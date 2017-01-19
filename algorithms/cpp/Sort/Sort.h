#pragma once

#ifdef BUILD_DLL
    #define ALGO_API __declspec(dllexport)
#else  
    #define ALGO_API __declspec(dllimport)
#endif  

#include<map>
#include<string>
#include<vector>

namespace Sort
{
    ALGO_API int foo(int n);
}