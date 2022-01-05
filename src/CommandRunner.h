#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

class QStringList;
class QJsonDocument;

class CommandRunner {
public:
    virtual ~CommandRunner();

    virtual QJsonDocument run(const QStringList& args) const = 0;
};

#endif /* COMMANDRUNNER_H */
