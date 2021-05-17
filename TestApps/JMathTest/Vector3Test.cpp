#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Klink::JMath;

namespace JMathTest
{		
	TEST_CLASS(Vector3Test)
	{
	public:
		
		TEST_METHOD(TestConstructor)
		{
			// TODO: Your test code here

			Vector3 v0;
			Assert::AreEqual(v0.x, 0.0f);
			Assert::AreEqual(v0.y, 0.0f);
			Assert::AreEqual(v0.z, 0.0f);

			Vector3 v1(1.0f);
			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 1.0f);
			Assert::AreEqual(v1.z, 1.0f);

			Vector3 v2(1.0f, 2.0f, 3.0f);
			Assert::AreEqual(v2.x, 1.0f);
			Assert::AreEqual(v2.y, 2.0f);
			Assert::AreEqual(v2.z, 3.0f);
		}

		TEST_METHOD(TestAddition)
		{
			Vector3 v0(1.0f);
			Vector3 v1(1.0f, 2.0f, 3.0f);
			Vector3 v3 = v0 + v1;

			Assert::AreEqual(v0.x, 1.0f);
			Assert::AreEqual(v0.y, 1.0f);
			Assert::AreEqual(v0.z, 1.0f);

			Assert::AreEqual(v1.x, 1.0f);
			Assert::AreEqual(v1.y, 2.0f);
			Assert::AreEqual(v1.z, 3.0f);

			Assert::AreEqual(v3.x, 2.0f);
			Assert::AreEqual(v3.y, 3.0f);
			Assert::AreEqual(v3.z, 4.0f);
		}

	};
}