using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace Tests
{
    [TestClass]
    public class SortTests
    {
        [TestMethod]
        public void WhenEmptyInput_ExpectEmptyReturn()
        {
            Console.WriteLine(Algorithms.Sort.fib(10));
        }
    }
}
