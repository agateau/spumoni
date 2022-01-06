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
    QIcon icon;
    if (QIcon::hasThemeIcon(iconName)) {
        icon = QIcon::fromTheme(iconName);
    } else {
        QPixmap pixmap(iconName);
        if (pixmap.isNull()) {
            qWarning() << "Could not decode icon" << iconName;
            // TODO: use fallback icon
        } else {
            icon = QIcon(pixmap);
        }
    }
    mIcon.setIcon(icon);
    mIcon.setToolTip(toolTipText);
    mIcon.show();
}

void Manager::onActivated(QSystemTrayIcon::ActivationReason reason) {
    QString reasonStr;
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::DoubleClick:
        reasonStr = "main";
        break;
    case QSystemTrayIcon::MiddleClick:
        reasonStr = "middle";
        break;
    case QSystemTrayIcon::Context:
        reasonStr = "context";
        break;
    }

    mRunner->detachedRun({"--activate", reasonStr});
}
