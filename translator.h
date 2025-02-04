#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QCoreApplication>
#include <QTranslator>
#include <QString>

class Translator
{
    QTranslator translator;
private:
    Translator();
    Translator(const Translator&) = delete;
    Translator& operator=(const Translator&) = delete;
public:
    static Translator& get();
    void set(QString lang);
};

#endif // TRANSLATOR_H
