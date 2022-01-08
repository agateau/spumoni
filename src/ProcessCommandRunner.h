#ifndef PROCESSCOMMANDRUNNER_H
#define PROCESSCOMMANDRUNNER_H

#include "CommandRunner.h"

#include <QObject>
#include <QString>
#include <QStringList>

class ProcessCommandRunner : public QObject, public CommandRunner {
    Q_OBJECT
public:
    explicit ProcessCommandRunner(const QString& cmd, const QStringList& arguments);
    RunResult run(const QStringList& args) const override;
    std::optional<CommandError> detachedRun(const QStringList& args) const override;

private:
    const QString mCommand;
    const QStringList mArguments;
};

#endif /* PROCESSCOMMANDRUNNER_H */
