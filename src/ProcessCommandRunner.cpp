#include "ProcessCommandRunner.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QProcess>

ProcessCommandRunner::ProcessCommandRunner(const QString& command, const QStringList& argments)
        : mCommand(command), mArguments(argments) {
}

QJsonDocument ProcessCommandRunner::run(const QStringList& arguments_) const {
    QProcess process;

    auto arguments = mArguments + arguments_;

    // TODO: Check mCommand exists
    process.start(mCommand, arguments);
    if (!process.waitForStarted(500)) {
        qWarning() << "Could not start command" << mCommand << arguments;
        return {};
    }
    auto ok = process.waitForFinished(1000);
    if (!ok) {
        qWarning() << "Process did not finish in time";
        return {};
    }
    auto output = process.readAllStandardOutput();

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(output, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Invalid JSON received from" << arguments;
        qWarning() << "Error:" << error.errorString();
        qWarning() << "Content:" << output;
    }
    return doc;
}

void ProcessCommandRunner::detachedRun(const QStringList& arguments_) const {
    auto arguments = mArguments + arguments_;
    QProcess::startDetached(mCommand, arguments);
}
