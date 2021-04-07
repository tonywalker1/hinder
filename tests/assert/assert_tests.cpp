//
// Created by tony on 3/24/21.
//

#include <catch2/catch.hpp>
//#include <hinder/assert/assert.h>

#include <cassert>

SCENARIO("stuff", "[assert")
{
    int answer = 43;
    assert(answer == 42);
}
