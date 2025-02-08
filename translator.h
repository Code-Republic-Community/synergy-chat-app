#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QCoreApplication>
#include <QString>
#include <QTranslator>

class Translator
{
    QTranslator translator;

private:
    Translator();
    Translator(const Translator &) = delete;
    Translator &operator=(const Translator &) = delete;

public:
    static Translator &get();
    void set(QString lang);
};

#endif // TRANSLATOR_H
