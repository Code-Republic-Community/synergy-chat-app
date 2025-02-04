#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QString>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMap>
#include <QLabel>

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(QWidget *parent = nullptr);

    void setLanguage();

signals:
    void goBackSignal();
    void languageChanged();
    void themeChanged();

private:
    QMap<QComboBox*, int> oldSettings;
    QMap<QComboBox*, int> newSettings;

private:
    QLabel *themeLabel;
    QLabel *languageLabel;
    QLabel *notificationLabel;

private:
    QString englishText;
    QString armenianText;
    QString russianText;

    QString lightText;
    QString darkText;
    QString autoText;

    QString enabledText;
    QString disabledText;

private:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QHBoxLayout *buttonsLayout;

    QComboBox *themeComboBox;
    QComboBox *languageComboBox;
    QComboBox *notificationComboBox;

    QPushButton *save;
    QPushButton *cancel;

    bool isEditing = false;

private:
    void init();
    void setup();
    void styleing();
    void connections();
};

#endif // SETTINGS_H
