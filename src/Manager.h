#ifndef MANAGER_H
#define MANAGER_H

#include <QJsonArray>
#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

#include <memory>

class CommandRunner;

class QAction;

class Manager : public QObject {
public:
    explicit Manager(std::unique_ptr<CommandRunner> runner);
    ~Manager();

private:
    void refresh();
    void onActivated(QSystemTrayIcon::ActivationReason reason);
    void onMenuTriggered(QAction* action);
    void onAboutToShowMenu();

    const std::unique_ptr<CommandRunner> mRunner;
    const QString mCommand;
    QSystemTrayIcon mIcon;
    QMenu mMenu;
    QJsonArray mPendingMenuDefinition;
};

#endif /* MANAGER_H */
