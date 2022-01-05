#include "ProcessCommandRunner.h"

#include <QDebug>
#include <QJsonDocument>
#include <QProcess>

ProcessCommandRunner::ProcessCommandRunner(const QString& command) : mCommand(command) {
}

QJsonDocument ProcessCommandRunner::run(const QStringList& arguments) const {
    QProcess process;
    process.start(mCommand, arguments);
    auto ok = process.waitForFinished(1000);
    if (!ok) {
        qWarning() << "Process did not finish in time";
        return {};
    }
    auto output = process.readAllStandardOutput();

    return QJsonDocument::fromJson(output);
}
