#include "Catch2QtUtils.h"

// Qt
#include <QAction>

// Catch2
#include <catch2/catch.hpp>

TEST_CASE("example") {
    SECTION("addition") {
        REQUIRE(2 + 2 == 4);
    }
    SECTION("substraction") {
        REQUIRE(2 - 2 == 0);
    }
}
