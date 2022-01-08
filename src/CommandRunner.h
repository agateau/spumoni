#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include <QString>

#include <optional>
#include <variant>

class QStringList;
class QJsonDocument;

struct CommandError {
    enum Code { FailedToStart, Failed, Timeout, InvalidJson };
    const Code code;
    const QString details;

    CommandError(Code code, const QString& details = {}) : code(code), details(details) {
    }
};

class CommandRunner {
public:
    using RunResult = std::variant<QJsonDocument, CommandError>;

    virtual ~CommandRunner();

    virtual RunResult run(const QStringList& args) const = 0;
    virtual std::optional<CommandError> detachedRun(const QStringList& args) const = 0;
};

#endif /* COMMANDRUNNER_H */
