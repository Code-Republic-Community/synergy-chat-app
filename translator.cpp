#include "translator.h"

Translator::Translator()
{
    qApp->installTranslator(&translator);
}

Translator &Translator::get()
{
    static Translator tr;
    return tr;
}

void Translator::set(QString lang)
{
    QString path = QString(":/i18n/Translator_%1.qm").arg(lang);
    if(!translator.load(path))
    {

    }
}
