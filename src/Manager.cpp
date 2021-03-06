#include "Manager.h"

#include "CommandRunner.h"

#include <QAction>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>

static constexpr char ID_PROPERTY[] = "id";
static constexpr char PRIMARY_REASON[] = "primary";
static constexpr char SECONDARY_REASON[] = "secondary";

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
    connect(&mIcon, &KStatusNotifierItem::activateRequested, this, [this] {
        onActivated(PRIMARY_REASON);
    });
    connect(&mIcon, &KStatusNotifierItem::secondaryActivateRequested, this, [this] {
        onActivated(SECONDARY_REASON);
    });
    connect(mIcon.contextMenu(), &QMenu::aboutToShow, this, &Manager::onAboutToShowMenu);
    connect(mIcon.contextMenu(), &QMenu::triggered, this, &Manager::onMenuTriggered);

    refresh();
}

Manager::~Manager() {
}

void Manager::refresh() {
    auto result = mRunner->run({"--status"});
    if (auto* error = std::get_if<CommandError>(&result)) {
        showCommandError(*error);
        return;
    }
    auto doc = std::get<QJsonDocument>(result);
    if (!doc.isObject()) {
        return;
    }
    auto root = doc.object();

    auto refreshMsec = int(root.value("refresh").toDouble() * 1000);
    if (refreshMsec > 0) {
        QTimer::singleShot(refreshMsec, this, &Manager::refresh);
    }

    auto iconName = root.value("iconName").toString();
    auto toolTipText = root.value("toolTipText").toString();

    if (iconName.isEmpty()) {
        mIcon.setStatus(KStatusNotifierItem::Passive);
        return;
    }

    auto icon = loadIcon(iconName);
    mIcon.setIconByPixmap(icon);
    mIcon.setToolTip(icon, toolTipText, {});

    iconName = root.value("overlayIconName").toString();
    if (iconName.isEmpty()) {
        mIcon.setOverlayIconByPixmap({});
    } else {
        mIcon.setOverlayIconByPixmap(loadIcon(iconName));
    }

    mPendingMenuDefinition = root.value("actions").toArray();

    mIcon.setStatus(KStatusNotifierItem::Active);
}

void Manager::onActivated(const QString& reason) {
    mRunner->detachedRun({"--activate", reason});
}

void Manager::onAboutToShowMenu() {
    if (!mPendingMenuDefinition.empty()) {
        updateMenu(mIcon.contextMenu(), mPendingMenuDefinition);
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

void Manager::showCommandError(const CommandError& error) {
    QString message;
    switch (error.code) {
    case CommandError::FailedToStart:
        message = tr("Command failed to start");
        break;
    case CommandError::Failed:
        message = tr("Command failed");
        break;
    case CommandError::Timeout:
        message = tr("Command ran for too long");
        break;
    case CommandError::InvalidJson:
        message = tr("Command returned invalid JSON");
        break;
    }
    mIcon.showMessage(message, error.details, "error");
}
