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

class Settings : public QWidget
{
    Q_OBJECT
public:
    Settings(QWidget *parent = nullptr);

signals:
    void goBackSignal();

private slots:
    void handle_back_btn();

private:
    QMap<QComboBox*, int> oldSettings;
    QMap<QComboBox*, int> newSettings;

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
