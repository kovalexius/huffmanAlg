#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <iostream>
#include "vector_bb.h"

SCENARIO("Test size calculation of vector", "[vector]")
{
    GIVEN("A vector with some items")
    {
        containers::vector_bb v = { true, false, true, true, true, false };
        
		REQUIRE(v.numberBits() == 6);
        WHEN("Adding elements without increasing") 
        {
            v.push_back(true);
            v.push_back(true);

			THEN("the size in bytes not changed")
			{
				std::string strBits = v;
				REQUIRE(strBits == "10111011");
				REQUIRE(v.size() == 1);
				REQUIRE(v.numberBits() == 8);
			}
        }
        WHEN("Adding elements with increasing")
        {
            v.push_back(false);
            v.push_back(true);
            v.push_back(true);

			THEN("the size increased")
			{
				std::string strBits = v;
				REQUIRE(strBits == "101110011");
				REQUIRE(v.size() == 2);
				REQUIRE(v.numberBits() == 9);
			}
        }
        WHEN("Adding elements and remove. size not changed")
        {
            v.push_back(false);
            v.push_back(true);
            v.push_back(true);
            REQUIRE(v.pop_back() == true);

			THEN("the size increased")
			{
				std::string strBits = v;
				REQUIRE(strBits == "10111001");
				REQUIRE(v.size() == 1);
				REQUIRE(v.numberBits() == 8);
			}
        }
    }
}

SCENARIO("Test integrity", "[vector]")
{
    GIVEN("Vector with byte with some bits")
    {
        containers::vector_bb v = {true, true, true, true, false, false, false, false};
		std::string strBits = v;
		REQUIRE(strBits == "11110000");
        REQUIRE(v.size() == 1);
        REQUIRE(v.numberBits() == 8);
        REQUIRE(v.pop_back() == false);
        REQUIRE(v.pop_back() == false);
        REQUIRE(v.pop_back() == false);
        REQUIRE(v.pop_back() == false);
        REQUIRE(v.pop_back() == true);
        REQUIRE(v.pop_back() == true);
        REQUIRE(v.pop_back() == true);
        REQUIRE(v.pop_back() == true);
    }
}

SCENARIO("Test equal operators", "[vector]")
{
    GIVEN("A two vectors with some items")
    {
        containers::vector_bb v1 = { true, false, true, true, true, false, false, true, true };
        REQUIRE(v1.size() == 2);
		REQUIRE(v1.numberBits() == 9);
        containers::vector_bb v2 = { true, false, true, true, true, false };
        REQUIRE(v2.size() == 1);
		REQUIRE(v2.numberBits() == 6);
 
        WHEN("v2 increasing")
        {
            v2.push_back(false);
            v2.push_back(true);
            v2.push_back(true);

            THEN("the size of both == 2")
            {
				std::string strBits1 = v1;
				REQUIRE(strBits1 == "101110011");
				std::string strBits2 = v2;
				REQUIRE(strBits2 == "101110011");
                REQUIRE(v1.size() == 2);
                REQUIRE(v2.size() == 2);
                REQUIRE(v1 == v2);
				REQUIRE(v1.numberBits() == 9);
				REQUIRE(v2.numberBits() == 9);
            }
        }
        WHEN("v2 increasing, v1 decreasing")
        {
            REQUIRE(v1.pop_back() == true);
            v2.push_back(false);
            v2.push_back(true);

            THEN("the size of both == 1")
            {
                REQUIRE(v1.size() == 1);
				REQUIRE(v1.numberBits() == 8);
                REQUIRE(v2.size() == 1);
				REQUIRE(v2.numberBits() == 8);
                REQUIRE(v1 == v2);
            }
        }
        WHEN("v1 decreasing")
        {
            REQUIRE(v1.pop_back() == true);
            REQUIRE(v1.pop_back() == true);
            REQUIRE(v1.pop_back() == false);

            THEN("the size decreased")
            {
                REQUIRE(v1.size() == 1);
				REQUIRE(v1.numberBits() == 6);
                REQUIRE(v2.size() == 1);
				REQUIRE(v2.numberBits() == 6);
                REQUIRE(v1.size() == v2.size());
				REQUIRE(v1 == v2);
            }
        }
    }
}

SCENARIO("Test math operators", "[vector]")
{
	GIVEN("A two vectors with some items")
	{
		WHEN("")
		{
			containers::vector_bb v1 = { true, false, true, true, true, false, false };
			REQUIRE(v1.size() == 1);
			REQUIRE(v1.numberBits() == 7);
			containers::vector_bb v2 = { true };
			REQUIRE(v2.size() == 1);
			REQUIRE(v2.numberBits() == 1);

			v1 += v2;
			THEN("")
			{
				std::string strBits1 = v1;
				REQUIRE(strBits1 == "10111001");
				REQUIRE(v1.numberBits() == 8);
			}
		}

		WHEN("")
		{
			containers::vector_bb v1 = { true, false, true, true, true };
			REQUIRE(v1.size() == 1);
			REQUIRE(v1.numberBits() == 5);
			containers::vector_bb v2 = { true, false, true, false, false, true };
			REQUIRE(v2.size() == 1);
			REQUIRE(v2.numberBits() == 6);

			v1 += v2;
			THEN("")
			{
				std::string strBits1 = v1;
				REQUIRE(strBits1 == "10111101001");
				REQUIRE(v1.numberBits() == 11);
			}
		}
	}
}
