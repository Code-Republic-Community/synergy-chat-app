#include "settings.h"
#include <QPushButton>


Settings::Settings(QWidget *parent) : QWidget(parent) {

    this ->resize(400, 700);
    this -> setFixedSize(400, 700);
    init();
    setup();
    connections();
    styleing();
}

void Settings::init()
{
    mainLayout = new QVBoxLayout();
    formLayout = new QFormLayout();
    buttonsLayout = new QHBoxLayout();


    themeComboBox = new QComboBox();
    languageComboBox = new QComboBox();
    notificationComboBox = new QComboBox();

    save = new QPushButton("Save");
    cancel = new QPushButton("Back");
}

void Settings::setup()
{
    mainLayout->addLayout(formLayout);
    mainLayout -> addLayout(buttonsLayout);

    themeComboBox->addItems({"Light", "Dark", "Auto"});
    languageComboBox->addItems({"English", "Armenian", "Russian"});
    notificationComboBox->addItems({"Enabled", "Disabled"});

    formLayout->addRow("Theme", themeComboBox);
    formLayout->addRow("Language", languageComboBox);
    formLayout->addRow("Notifications", notificationComboBox);

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
        cancel->setText("Cancel");
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }
    });
    connect(languageComboBox, &QComboBox::currentIndexChanged, this, [this](){
        isEditing = true;
        cancel->setText("Cancel");
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }
    });
    connect(notificationComboBox, &QComboBox::currentIndexChanged, this, [this](){
        isEditing = true;
        cancel->setText("Cancel");
        for (auto comboBox : newSettings.keys())
        {
            newSettings[comboBox] = comboBox->currentIndex();
        }
    });
    connect(save, &QPushButton::clicked, this,[this](){
        isEditing = false;
        oldSettings = newSettings;
        cancel ->setText("Back");
    });
    connect(cancel, &QPushButton::clicked, this,[this](){
        if(!isEditing)
        {
            qDebug()<< "go Back Signal";
            emit goBackSignal();
        }
        else
        {
            for (auto comboBox : oldSettings.keys()) {
                int oldIndex = oldSettings[comboBox];
                comboBox->setCurrentIndex(oldIndex);
            }
            newSettings = oldSettings;
            cancel->setText("Back");
            isEditing = false;
        }
    });
}
