#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Sort/Sort.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
    TEST_CLASS(SortUnitTests)
    {
    public:

        TEST_METHOD(SimpleTest)
        {
            // TODO: Your test code here
            Assert::AreEqual(Sort::foo(1), 1, L"yes");
        }

    };
}