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
				REQUIRE(v.size() == 1);
				REQUIRE(v.numberBits() == 8);
			}
        }
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
                REQUIRE(v1 == v2);
            }
        }
    }
}
