#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QSystemTrayIcon>

#include <memory>

class CommandRunner;

class Manager : public QObject {
public:
    explicit Manager(std::unique_ptr<CommandRunner> runner);
    ~Manager();

private:
    void updateStatus();
    void onActivated(QSystemTrayIcon::ActivationReason reason);

    const std::unique_ptr<CommandRunner> mRunner;
    const QString mCommand;
    QSystemTrayIcon mIcon;
};

#endif /* MANAGER_H */
