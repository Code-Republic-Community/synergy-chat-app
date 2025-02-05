#include "globals.h"

Globals& Globals::getInstance()
{
    static Globals instance;
    return instance;
}

const QString& Globals::getUserId()
{
    return USER_ID;
}

void Globals::setUserID(const QString& id)
{
    USER_ID = id;
}

Globals::Globals()
{
    comboBoxStyle = { "background-color", "white" };
    buttonStyle = { "color", "black" };
    formLabelStyle = { "font-size", "14px" };
    labelStyle = { "font-weight", "bold" };
    checkBoxStyle = { "border", "1px solid black" };

    USER_ID = "";
}
