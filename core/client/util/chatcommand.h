#ifndef CHATCOMMAND_H
#define CHATCOMMAND_H

#include<QString>
#include<optional>
#include<QRegularExpression>

enum class ChatCommandType {
    DICEROLL,
    INVALID
};

class ChatCommand
{
    static const QString sCommand;
    static const QString sArguments;
    static const QRegularExpression sParserString;
    ChatCommandType mType;
    QString mCommandString;
    QString mArgumentString;
public:
    ChatCommand();
    ChatCommand(ChatCommandType type, const QString& commandString, const QString& argumentString);
    static std::optional<ChatCommand> parse(const QString& chatMessage);

    ChatCommandType type() const;
    const QString& commandString() const;
    const QString& argumentString() const;
};

#endif // CHATCOMMAND_H
