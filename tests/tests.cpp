#define CATCH_CONFIG_RUNNER
#include <QApplication>
#include <QtTest>

#include <catch2/catch.hpp>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QTEST_SET_MAIN_SOURCE_PATH
    return Catch::Session().run(argc, argv);
}
