#include "BuildConfig.h"
#include "Manager.h"
#include "ProcessCommandRunner.h"
#include "Resources.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QLocale>
#include <QTranslator>

#include <memory>

static void loadTranslations(QObject* parent) {
    std::optional<QString> translationsDir = Resources::findDir("translations");
    if (!translationsDir.has_value()) {
        return;
    }
    auto translator = new QTranslator(parent);
    QLocale locale;
    if (translator->load(locale, APP_NAME, "_", translationsDir.value())) {
        QCoreApplication::installTranslator(translator);
    }
}

/**
 * Initialize QIcon so that QIcon::fromTheme() finds our icons on Windows and
 * macOS
 */
static void initFallbackIcons() {
#if defined(Q_OS_WINDOWS) || defined(Q_OS_MACOS)
    // A theme name must be defined othewise QIcon::fromTheme won't look in
    // fallbackSearchPaths
    QIcon::setThemeName(APP_NAME);
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":/icons");
#endif
}

struct Arguments {
    QString command;
};

static Arguments parseArguments(const QStringList& arguments) {
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("command", "Command to run");
    parser.process(arguments);

    if (parser.positionalArguments().length() != 1) {
        parser.showHelp(1);
    }

    Arguments args;
    args.command = parser.positionalArguments().first();
    return args;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setApplicationName(APP_NAME);
    app.setApplicationVersion(APP_VERSION);
    auto iconName = QString(":/appicon/sc-apps-%1.svg").arg(APP_NAME);
    app.setWindowIcon(QIcon(iconName));
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    initFallbackIcons();

    loadTranslations(&app);

    auto args = parseArguments(app.arguments());
    auto runner = std::make_unique<ProcessCommandRunner>(args.command);

    Manager manager(std::move(runner));

    return app.exec();
}
