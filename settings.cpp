#include "settings.h"
#include "translator.h"
#include <QPushButton>


Settings::Settings(QWidget *parent) : QWidget(parent)
{

    this ->resize(400, 700);
    this -> setFixedSize(400, 700);
    Translator::get().set("en_US");
    init();

    setup();
    setLanguage();
    connections();
    styleing();
}

void Settings::setLanguage()
{
    save->setText(tr("Save"));
    cancel->setText(tr("Back"));

    englishText = tr("English");
    armenianText = tr("Armenian");
    russianText = tr("Russian");

    lightText = tr("Light");
    darkText = tr("Dark");
    autoText = tr("Auto");

    enabledText = tr("Enabled");
    disabledText = tr("Disabled");

    themeComboBox->setItemText(0, lightText);
    themeComboBox->setItemText(1, darkText);
    themeComboBox->setItemText(2, autoText);

    notificationComboBox->setItemText(0, enabledText);
    notificationComboBox->setItemText(1, disabledText);

    languageComboBox->setItemText(0, englishText);
    languageComboBox->setItemText(1, armenianText);
    languageComboBox->setItemText(2, russianText);

    themeLabel->setText(tr("Theme"));
    languageLabel->setText(tr("Language"));
    notificationLabel->setText(tr("Notifications"));

}

void Settings::init()
{
    englishText = tr("English");
    armenianText = tr("Armenian");
    russianText = tr("Russian");

    lightText = tr("Light");
    darkText = tr("Dark");
    autoText = tr("Auto");

    enabledText = tr("Enabled");
    disabledText = tr("Disabled");

    themeLabel = new QLabel(tr("Theme"));
    languageLabel = new QLabel(tr("Language"));
    notificationLabel = new QLabel(tr("Notifications"));

    mainLayout = new QVBoxLayout();
    formLayout = new QFormLayout();
    buttonsLayout = new QHBoxLayout();


    themeComboBox = new QComboBox();
    languageComboBox = new QComboBox();
    notificationComboBox = new QComboBox();

    save = new QPushButton();
    cancel = new QPushButton();
}

void Settings::setup()
{
    mainLayout->addLayout(formLayout);
    mainLayout -> addLayout(buttonsLayout);

    formLayout->addRow(themeLabel, themeComboBox);
    formLayout->addRow(languageLabel, languageComboBox);
    formLayout->addRow(notificationLabel, notificationComboBox);

    themeComboBox->addItem(lightText);
    themeComboBox->addItem(darkText);
    themeComboBox->addItem(autoText);

    notificationComboBox->addItem(enabledText);
    notificationComboBox->addItem(disabledText);

    languageComboBox->addItem(englishText, "en_US");
    languageComboBox->addItem(armenianText, "hy_AM");
    languageComboBox->addItem(russianText, "ru_RU");

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(cancel);
    buttonsLayout->addWidget(save);
    buttonsLayout->addStretch();

    oldSettings.insert(themeComboBox, 0);
    oldSettings.insert(languageComboBox, 0);
    oldSettings.insert(notificationComboBox, 0);
    newSettings = oldSettings;

    setLayout(mainLayout);
}

void Settings::styleing()
{
    const int fixedLabelWidth = 100;

    themeLabel->setFixedWidth(fixedLabelWidth);
    languageLabel->setFixedWidth(fixedLabelWidth);
    notificationLabel->setFixedWidth(fixedLabelWidth);

    QString comboBoxStyle = R"(
        QComboBox {
            color: #FFFFFF;
            font-size: 12px;
            border: 1px solid #420242;
            border-radius: 5px;
            padding: 5px;
            background-color: #2A002A;
        }
        QComboBox::drop-down {
            border: none;
            background-color: #420242;
            width: 25px;
        }
        QComboBox QAbstractItemView {
            background-color: #1E1E1E;
            border: 1px solid #420242;
            selection-background-color: #5A005A;
            selection-color: #FFFFFF;
            color: #FFFFFF;
        }
    )";


    QString buttonStyle = R"(
        QPushButton {
            border: 1px solid #420242;
            border-radius: 5px;
            padding: 8px 15px;
            background-color: #420242;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5A005A;
        }
        QPushButton:pressed {
            background-color: #2A002A;
        }
    )";

    QString formLabelStyle = R"(
        QLabel {
            color: #FFFFFF;
            font-size: 12px;
            border: 3px solid #420242;
            border-radius: 5px;
            padding: 3px;
        }
    )";

    themeComboBox->setStyleSheet(comboBoxStyle);
    languageComboBox->setStyleSheet(comboBoxStyle);
    notificationComboBox->setStyleSheet(comboBoxStyle);

    save->setStyleSheet(buttonStyle);
    cancel->setStyleSheet(buttonStyle);

    for (int i = 0; i < formLayout->rowCount(); ++i) {
        QWidget *labelWidget = formLayout->itemAt(i, QFormLayout::LabelRole)->widget();
        if (labelWidget) {
            labelWidget->setStyleSheet(formLabelStyle);
        }
    }

    this->setStyleSheet("background-color: #000000;");
}

void Settings::connections()
{
    connect(themeComboBox, &QComboBox::currentIndexChanged, this, [this](){
        isEditing = true;
        cancel->setText(tr("Cancel"));
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }
    });
    connect(languageComboBox, &QComboBox::currentIndexChanged, this, [this](){
        isEditing = true;
        cancel->setText(tr("Cancel"));
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }


    });
    connect(notificationComboBox, &QComboBox::currentIndexChanged, this, [this](){
        isEditing = true;
        cancel->setText(tr("Cancel"));
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }
    });
    connect(save, &QPushButton::clicked, this, [this]() {
        isEditing = false;
        cancel->setText(tr("Back"));
        QString newLang = languageComboBox->currentData().toString();
        QString oldLang = oldSettings.contains(languageComboBox) ?
                              languageComboBox->itemData(oldSettings[languageComboBox]).toString() : "";
        if (newLang != oldLang) {
            Translator::get().set(newLang);
            emit languageChanged();
        }
        oldSettings = newSettings;
    });
    connect(cancel, &QPushButton::clicked, this,[this](){
        if(!isEditing)
        {
            emit goBackSignal();
        }
        else
        {
            for (auto comboBox : oldSettings.keys()) {
                int oldIndex = oldSettings[comboBox];
                comboBox->setCurrentIndex(oldIndex);
            }
            newSettings = oldSettings;
            cancel->setText(tr("Back"));
            isEditing = false;
        }
    });
}
