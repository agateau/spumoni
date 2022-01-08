#ifndef PROCESSCOMMANDRUNNER_H
#define PROCESSCOMMANDRUNNER_H

#include "CommandRunner.h"

#include <QString>
#include <QStringList>

class ProcessCommandRunner : public CommandRunner {
public:
    explicit ProcessCommandRunner(const QString& cmd, const QStringList& arguments);
    QJsonDocument run(const QStringList& args) const override;
    void detachedRun(const QStringList& args) const override;

private:
    const QString mCommand;
    const QStringList mArguments;
};

#endif /* PROCESSCOMMANDRUNNER_H */
