#include "ProcessCommandRunner.h"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QProcess>

ProcessCommandRunner::ProcessCommandRunner(const QString& command, const QStringList& argments)
        : mCommand(command), mArguments(argments) {
}

CommandRunner::RunResult ProcessCommandRunner::run(const QStringList& arguments_) const {
    QProcess process;

    auto arguments = mArguments + arguments_;

    process.start(mCommand, arguments);
    if (!process.waitForStarted(500)) {
        return CommandError::FailedToStart;
    }
    auto ok = process.waitForFinished(1000);
    if (!ok) {
        return CommandError::Timeout;
    }
    auto output = process.readAllStandardOutput();
    if (process.exitCode() != 0) {
        auto out = QString::fromUtf8(output);
        auto err = QString::fromUtf8(process.readAllStandardError());
        auto details = tr("Command failed with exit code %1."
                          "\nout:\n%2"
                          "\nerr:\n%3")
                           .arg(process.exitCode())
                           .arg(out)
                           .arg(err);
        return CommandError(CommandError::Failed, details);
    }

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(output, &error);
    if (error.error != QJsonParseError::NoError) {
        auto details = QString("Invalid JSON: %1.\nOutput:\n%2")
                           .arg(error.errorString(), QString::fromUtf8(output));
        return CommandError(CommandError::InvalidJson, details);
    }
    return doc;
}

std::optional<CommandError> ProcessCommandRunner::detachedRun(const QStringList& arguments_) const {
    auto arguments = mArguments + arguments_;
    if (!QProcess::startDetached(mCommand, arguments)) {
        return CommandError::FailedToStart;
    }
    return {};
}
