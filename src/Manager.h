#ifndef MANAGER_H
#define MANAGER_H

#include <KStatusNotifierItem>

#include <QJsonArray>
#include <QMenu>
#include <QObject>

#include <memory>

class CommandError;
class CommandRunner;

class QAction;

class Manager : public QObject {
    Q_OBJECT
public:
    explicit Manager(std::unique_ptr<CommandRunner> runner);
    ~Manager();

private:
    void refresh();
    void onActivated(const QString& reason);
    void onMenuTriggered(QAction* action);
    void onAboutToShowMenu();
    void showCommandError(const CommandError& error);

    const std::unique_ptr<CommandRunner> mRunner;
    const QString mCommand;
    KStatusNotifierItem mIcon;
    QJsonArray mPendingMenuDefinition;
};

#endif /* MANAGER_H */
