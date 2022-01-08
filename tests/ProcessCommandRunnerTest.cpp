#include "Catch2QtUtils.h"
#include "ProcessCommandRunner.h"

#include <catch2/catch.hpp>

static constexpr char JSON_DOC[] = "{\"a\":12}";

TEST_CASE("ProcessCommandRunner") {
    SECTION("run") {
        SECTION("success") {
            ProcessCommandRunner runner("echo", {});
            auto result = runner.run({JSON_DOC});
            auto* doc = std::get_if<QJsonDocument>(&result);
            REQUIRE(doc);
            REQUIRE(doc->object().value("a").toInt() == 12);
        }
        SECTION("failure_missing_command") {
            ProcessCommandRunner runner("not_a_real_command", {});
            auto result = runner.run({});
            auto* error = std::get_if<CommandError>(&result);
            REQUIRE(error);
            REQUIRE(error->code == CommandError::FailedToStart);
        }
        SECTION("failure_command_failed") {
            ProcessCommandRunner runner("false", {});
            auto result = runner.run({});
            auto* error = std::get_if<CommandError>(&result);
            REQUIRE(error);
            REQUIRE(error->code == CommandError::Failed);
        }
        SECTION("failure_invalid_json") {
            ProcessCommandRunner runner("echo", {});
            auto result = runner.run({"not_json"});
            auto* error = std::get_if<CommandError>(&result);
            REQUIRE(error);
            REQUIRE(error->code == CommandError::InvalidJson);
        }
    }
    SECTION("detachedRun") {
        SECTION("success") {
            ProcessCommandRunner runner("true", {});
            auto result = runner.detachedRun({});
            REQUIRE(!result.has_value());
        }
        SECTION("failure_missing_command") {
            ProcessCommandRunner runner("not_a_real_command", {});
            auto result = runner.detachedRun({});
            REQUIRE(result.has_value());
            auto error = result.value();
            REQUIRE(error.code == CommandError::FailedToStart);
        }
    }
}
