#include "Manager.h"

#include "CommandRunner.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

Manager::Manager(std::unique_ptr<CommandRunner> runner) : mRunner(std::move(runner)) {
    updateStatus();
    mIcon.show();
}

Manager::~Manager() {
}

void Manager::updateStatus() {
    auto doc = mRunner->run({"--status"});
    if (!doc.isObject()) {
        return;
    }
    auto root = doc.object();
    auto iconName = root.value("iconName").toString();
    auto toolTipText = root.value("toolTipText").toString();
    auto refresh = root.value("refresh").toInt();

    qDebug() << iconName << toolTipText << refresh;

    auto icon = QIcon::fromTheme(iconName);

    mIcon.setIcon(icon);
    mIcon.setToolTip(toolTipText);

    if (refresh > 0) {
        QTimer::singleShot(refresh, this, &Manager::updateStatus);
    }
}
