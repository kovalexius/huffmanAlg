#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <iostream>
#include "data_types.h"

SCENARIO("Test size calculation of vector", "[vector]")
{
    GIVEN("A vector with some items")
    {
        containers::vector_bb v = { true, false, true, true, true, false };
        
        REQUIRE(v.size() == 1);
        WHEN("Adding elements without increasing") 
        {
            v.push_back(true);
            v.push_back(true);

            THEN("the size not changed") 
                REQUIRE(v.size() == 1);
        }
        WHEN("Adding elements with increasing")
        {
            v.push_back(false);
            v.push_back(true);
            v.push_back(true);

            THEN("the size increased")
                REQUIRE(v.size() == 2);
        }
        WHEN("Adding elements and remove. size not changed")
        {
            v.push_back(false);
            v.push_back(true);
            v.push_back(true);
            v.pop_back();

            THEN("the size increased")
                REQUIRE(v.size() == 1);
        }
    }
}

SCENARIO("Test equal operators", "[vector]")
{
    GIVEN("A two vectors with some items")
    {
        containers::vector_bb v1 = { true, false, true, true, true, false, false, true, true };
        REQUIRE(v1.size() == 2);
        containers::vector_bb v2 = { true, false, true, true, true, false };
        REQUIRE(v2.size() == 1);
 
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
            }
        }
        WHEN("v2 increasing, v1 decreasing")
        {
            v1.pop_back();
            v2.push_back(false);
            v2.push_back(true);

            THEN("the size of both == 1")
            {
                REQUIRE(v1.size() == 1);
                REQUIRE(v2.size() == 1);
                REQUIRE(v1 == v2);
            }
        }
        WHEN("v1 decreasing")
        {
            v1.pop_back();
            v1.pop_back();
            v1.pop_back();

            THEN("the size decreased")
            {
                REQUIRE(v1.size() == 1);
                REQUIRE(v2.size() == 1);
                REQUIRE(v1 == v2);
            }
        }
    }
}
