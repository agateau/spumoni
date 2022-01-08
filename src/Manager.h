#ifndef MANAGER_H
#define MANAGER_H

#include <KStatusNotifierItem>

#include <QJsonArray>
#include <QMenu>
#include <QObject>

#include <memory>

class CommandRunner;

class QAction;

class Manager : public QObject {
public:
    explicit Manager(std::unique_ptr<CommandRunner> runner);
    ~Manager();

private:
    void refresh();
    void onActivated(const QString& reason);
    void onMenuTriggered(QAction* action);
    void onAboutToShowMenu();

    const std::unique_ptr<CommandRunner> mRunner;
    const QString mCommand;
    KStatusNotifierItem mIcon;
    QJsonArray mPendingMenuDefinition;
};

#endif /* MANAGER_H */
