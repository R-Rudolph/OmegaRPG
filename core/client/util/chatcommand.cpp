#include "chatcommand.h"

const QString ChatCommand::sCommand = "command";
const QString ChatCommand::sArguments = "arguments";
const QRegularExpression ChatCommand::sParserString =
    QRegularExpression("^"
                       "\\/(?<"+sCommand+">[a-zA-Z0-9_]+)([\\s\\t](?<"+sArguments+">.*))?"
                       "$");

ChatCommand::ChatCommand()
{
    mType = ChatCommandType::INVALID;
}

ChatCommand::ChatCommand(ChatCommandType type, const QString& commandString, const QString& argumentString) {
    mType = type;
    mCommandString = commandString;
    mArgumentString = argumentString;
}

std::optional<ChatCommand> ChatCommand::parse(const QString& chatMessage) {
    const QRegularExpressionMatch match = sParserString.match(chatMessage);

    if(match.hasMatch()) {
        ChatCommandType type = ChatCommandType::INVALID;
        QString commandString = match.captured(sCommand);
        if(commandString == "roll" || commandString == "dice" || commandString == "diceroll" || commandString == "r") {
            type = ChatCommandType::DICEROLL;
        }
        return ChatCommand(type, commandString, match.captured(sArguments));
    } else {
        return std::optional<ChatCommand>();
    }
}

ChatCommandType ChatCommand::type() const {
    return mType;
}

const QString& ChatCommand::commandString() const {
    return mCommandString;
}

const QString& ChatCommand::argumentString() const {
    return mArgumentString;
}
