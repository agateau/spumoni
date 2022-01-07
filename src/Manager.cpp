#include "Manager.h"

#include "CommandRunner.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

Manager::Manager(std::unique_ptr<CommandRunner> runner) : mRunner(std::move(runner)) {
    updateStatus();
    connect(&mIcon, &QSystemTrayIcon::activated, this, &Manager::onActivated);
}

Manager::~Manager() {
}

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

void Manager::updateStatus() {
    auto doc = mRunner->run({"--status"});
    if (!doc.isObject()) {
        return;
    }
    auto root = doc.object();

    auto refresh = root.value("refresh").toInt();
    if (refresh > 0) {
        QTimer::singleShot(refresh, this, &Manager::updateStatus);
    }

    auto iconName = root.value("iconName").toString();
    auto toolTipText = root.value("toolTipText").toString();

    if (iconName.isEmpty()) {
        mIcon.hide();
        return;
    }

    mIcon.setIcon(loadIcon(iconName));
    mIcon.setToolTip(toolTipText);

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
