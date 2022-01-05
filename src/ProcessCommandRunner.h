#ifndef PROCESSCOMMANDRUNNER_H
#define PROCESSCOMMANDRUNNER_H

#include "CommandRunner.h"

#include <QString>

class ProcessCommandRunner : public CommandRunner {
public:
    explicit ProcessCommandRunner(const QString& cmd);
    QJsonDocument run(const QStringList& args) const override;

private:
    const QString mCommand;
};

#endif /* PROCESSCOMMANDRUNNER_H */
