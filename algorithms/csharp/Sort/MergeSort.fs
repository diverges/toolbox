namespace Algorithms

module Sort =
    let rec fib = function
        | n when n=0I -> 0I
        | n when n=1I -> 1I
        | n -> fib(n-1I) + fib(n-2I)
