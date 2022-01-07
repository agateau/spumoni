#include "Manager.h"

#include "CommandRunner.h"

#include <QAction>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

static constexpr char ID_PROPERTY[] = "id";

static QIcon loadIcon(const QString& iconName) {
    if (QIcon::hasThemeIcon(iconName)) {
        return QIcon::fromTheme(iconName);
    }
    QPixmap pixmap(iconName);
    if (pixmap.isNull()) {
        qWarning() << "Could not decode icon" << iconName;
        // TODO: use fallback icon
        return {};
    }
    return QIcon(pixmap);
}

static void updateMenu(QMenu* menu, const QJsonArray& actions) {
    menu->clear();

    for (const auto& value : actions) {
        if (!value.isObject()) {
            qWarning() << "Invalid menu entry: not an object, skipping it";
            continue;
        }
        auto object = value.toObject();

        auto text = object.value("text").toString();
        auto action = menu->addAction(text);

        auto id = object.value("id").toString();
        action->setProperty(ID_PROPERTY, id);

        auto iconName = object.value("iconName").toString();
        if (!iconName.isEmpty()) {
            action->setIcon(loadIcon(iconName));
        }

        menu->addAction(action);
    }
}

Manager::Manager(std::unique_ptr<CommandRunner> runner) : mRunner(std::move(runner)) {
    mIcon.setContextMenu(&mMenu);

    connect(&mIcon, &QSystemTrayIcon::activated, this, &Manager::onActivated);
    connect(&mMenu, &QMenu::aboutToShow, this, &Manager::onAboutToShowMenu);
    connect(&mMenu, &QMenu::triggered, this, &Manager::onMenuTriggered);

    refresh();
}

Manager::~Manager() {
}

void Manager::refresh() {
    auto doc = mRunner->run({"--status"});
    if (!doc.isObject()) {
        return;
    }
    auto root = doc.object();

    auto refreshInterval = root.value("refresh").toInt();
    if (refreshInterval > 0) {
        QTimer::singleShot(refreshInterval, this, &Manager::refresh);
    }

    auto iconName = root.value("iconName").toString();
    auto toolTipText = root.value("toolTipText").toString();

    if (iconName.isEmpty()) {
        mIcon.hide();
        return;
    }

    mIcon.setIcon(loadIcon(iconName));
    mIcon.setToolTip(toolTipText);

    mPendingMenuDefinition = root.value("actions").toArray();

    mIcon.show();
}

void Manager::onActivated(QSystemTrayIcon::ActivationReason reason) {
    QString reasonStr;
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::DoubleClick:
        reasonStr = "primary";
        break;
    case QSystemTrayIcon::MiddleClick:
        reasonStr = "secondary";
        break;
    case QSystemTrayIcon::Context:
        return;
    }

    mRunner->detachedRun({"--activate", reasonStr});
}

void Manager::onAboutToShowMenu() {
    if (!mPendingMenuDefinition.empty()) {
        updateMenu(&mMenu, mPendingMenuDefinition);
        mPendingMenuDefinition = {};
    }
}

void Manager::onMenuTriggered(QAction* action) {
    auto id = action->property(ID_PROPERTY).toString();
    if (id.isEmpty()) {
        qWarning() << "Cannot activate action, its id is empty";
        return;
    }
    mRunner->detachedRun({"--activate-action", id});
}
