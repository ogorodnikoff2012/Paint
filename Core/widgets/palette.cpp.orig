#include "headers\palette.h"
#include "headers/mainwindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

Palette::Palette(QWidget *parent) :
    QWidget(parent),
    firstColor(Qt::black),
    secondColor(Qt::white),
    currentColor(&firstColor)
{
    setMinimumHeight(70);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(10);
    initStandardColors();

    setStyleSheet("QPushButton#colorIndicator {"
                  "border-color: white;"
                  "border-style: solid;"
                  "border-width: 1px;"
                  "}"
                  "QPushButton#colorIndicator:pressed {"
                  "border-color: black;"
                  "}"
                  "QPushButton#colorChooser {"
                  "border-color: black;"
                  "border-style: solid;"
                  "border-width: 0px;"
                  "}"
                  "QPushButton#colorChooser:hover {"
                  "border-color: black;"
                  "border-width: 1px;"
                  "}");

    QLayout *indicatorLayout = createIndicator();
    QLayout* paletteLayout = createPalette();
    layout->addLayout(indicatorLayout);
    layout->addLayout(paletteLayout);

    QPushButton* moreColorsButton = new QPushButton(tr("More colors"));
    connect(moreColorsButton, SIGNAL(clicked()), SLOT(moreColors()));
    layout->addWidget(moreColorsButton);
    layout->addStretch();

    setLayout(layout);
}

void Palette::changeCurrentColor()
{
    QPushButton* button = qobject_cast<QPushButton *>(sender());
    if(button == firstColorButton)
        currentColor = &firstColor;
    else if(button == secondColorButton)
        currentColor = &secondColor;
}

void Palette::updateIndicator()
{
    firstColorButton->setStyleSheet("QPushButton {background-color: " + firstColor.name() + "}");
    secondColorButton->setStyleSheet("QPushButton {background-color: " + secondColor.name() + "}");
}

void Palette::swapColors()
{
    QColor t = firstColor;
    firstColor = secondColor;
    secondColor = t;
    updateIndicator();
}

void Palette::defaultColors()
{
    firstColor = Qt::black;
    secondColor = Qt::white;
    updateIndicator();
}

void Palette::setColor()
{
     QPushButton* button = qobject_cast<QPushButton *>(sender());
     if(button && button->objectName() == "colorChooser")
     {
         QPalette pal = button->palette();
         QBrush brush = pal.background();
         *currentColor = brush.color();
         updateIndicator();
     }
}

void Palette::moreColors()
{
    QColor col = QColorDialog::getColor(*currentColor, MainWindow::window, tr("Choose color"), QColorDialog::ShowAlphaChannel);
    if(col.isValid())
        *currentColor = col;
    updateIndicator();
}

void Palette::initStandardColors()
{
    standardColors
            << QColor(0x00, 0x00, 0x00)
            << QColor(0x40, 0x40, 0x40)
            << QColor(0xff, 0x00, 0x00)
            << QColor(0xff, 0x6a, 0x00)
            << QColor(0xff, 0xd8, 0x00)
            << QColor(0xb6, 0xff, 0x00)
            << QColor(0x4c, 0xff, 0x00)
            << QColor(0x00, 0xff, 0x21)
            << QColor(0x00, 0xff, 0x90)
            << QColor(0x00, 0xff, 0xff)
            << QColor(0x00, 0x94, 0xff)
            << QColor(0x00, 0x26, 0xff)
            << QColor(0x48, 0x00, 0xff)
            << QColor(0xb2, 0x00, 0xff)
            << QColor(0xff, 0x00, 0xdc)
            << QColor(0xff, 0x00, 0x6e)
            << QColor(0xff, 0xff, 0xff)
            << QColor(0x80, 0x80, 0x80)
            << QColor(0x7f, 0x00, 0x00)
            << QColor(0x7f, 0x33, 0x00)
            << QColor(0x7f, 0x6a, 0x00)
            << QColor(0x5b, 0x7f, 0x00)
            << QColor(0x26, 0x7f, 0x00)
            << QColor(0x00, 0x7f, 0x0e)
            << QColor(0x00, 0x7f, 0x46)
            << QColor(0x00, 0x7f, 0x7f)
            << QColor(0x00, 0x4a, 0x7f)
            << QColor(0x00, 0x13, 0x7f)
            << QColor(0x21, 0x00, 0x7f)
            << QColor(0x57, 0x00, 0x7f)
            << QColor(0x7f, 0x00, 0x6e)
            << QColor(0x7f, 0x00, 0x37);
}

QLayout *Palette::createIndicator()
{
    firstColorButton = new QPushButton;
    QFrame* firstColorFrame = new QFrame;
    secondColorButton = new QPushButton;
    QFrame* secondColorFrame = new QFrame;

    QGridLayout* indicatorLayout = new QGridLayout;

    indicatorLayout->setSpacing(0);

    secondColorFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    secondColorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    secondColorFrame->setLineWidth(1);
    QPalette pal = secondColorFrame->palette();
    pal.setBrush(QPalette::Background, MainWindow::window->background);
    secondColorFrame->setPalette(pal);

    secondColorButton->setMinimumSize(30, 30);
    secondColorButton->setMaximumSize(30, 30);
    secondColorButton->setObjectName("colorIndicator");

    connect(secondColorButton, SIGNAL(clicked()), SLOT(changeCurrentColor()));

    indicatorLayout->addWidget(secondColorFrame, 1, 1, 2, 2);
    QHBoxLayout* secondColorLayout = new QHBoxLayout;
    secondColorLayout->setMargin(0);
    secondColorFrame->setLayout(secondColorLayout);
    secondColorLayout->addWidget(secondColorButton);

    //----------------------------------------------------

    firstColorFrame->setFrameStyle(QFrame::Box | QFrame::Plain);
    firstColorFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    firstColorFrame->setLineWidth(1);
    pal = firstColorFrame->palette();
    pal.setBrush(QPalette::Background, MainWindow::window->background);
    firstColorFrame->setPalette(pal);


    firstColorButton->setMinimumSize(30, 30);
    firstColorButton->setMaximumSize(30, 30);
    firstColorButton->setObjectName("colorIndicator");

    connect(firstColorButton, SIGNAL(clicked()), SLOT(changeCurrentColor()));

    indicatorLayout->addWidget(firstColorFrame, 0, 0, 2, 2);
    QHBoxLayout* firstColorLayout = new QHBoxLayout;
    firstColorLayout->setMargin(0);
    firstColorFrame->setLayout(firstColorLayout);
    firstColorLayout->addWidget(firstColorButton);

    //----------------------------------------------------

    QPushButton* swap = new QPushButton;
    swap->setIcon(QIcon(":icons/swap.png"));
    swap->setMinimumSize(16, 16);
    swap->setMaximumSize(16, 16);
    swap->setFlat(true);

    connect(swap, SIGNAL(clicked()), SLOT(swapColors()));

    indicatorLayout->addWidget(swap, 0, 2);

    QPushButton* defaultColors = new QPushButton;
    defaultColors->setIcon(QIcon(":icons/default_colors.png"));
    defaultColors->setMinimumSize(16, 16);
    defaultColors->setMaximumSize(16, 16);
    defaultColors->setFlat(true);

    connect(defaultColors, SIGNAL(clicked()), SLOT(defaultColors()));

    indicatorLayout->addWidget(defaultColors, 2, 0);

    updateIndicator();

    QVBoxLayout* globalLayout = new QVBoxLayout;
    globalLayout->addStretch();
    globalLayout->addLayout(indicatorLayout);
    globalLayout->addStretch();
    globalLayout->setSpacing(0);
    return globalLayout;
}

QLayout *Palette::createPalette()
{
    QGridLayout* layout = new QGridLayout;
    layout->setVerticalSpacing(0);
    layout->setHorizontalSpacing(0);
    int numOfColors = standardColors.size();
    for(int i = 0; i < numOfColors; ++i)
    {
        QPushButton* button = new QPushButton;
        button->setMinimumSize(15, 15);
        button->setMaximumSize(15, 15);
        button->setObjectName("colorChooser");
        button->setStyleSheet("QPushButton {background-color: " + standardColors[i].name() + "}");
        QPalette pal = button->palette();
        pal.setColor(QPalette::Background, standardColors[i]);
        button->setPalette(pal);
        connect(button, SIGNAL(clicked()), SLOT(setColor()));
        layout->addWidget(button, (i*2) / numOfColors, (i) % (numOfColors / 2));
    }
    QVBoxLayout* globalLayout = new QVBoxLayout;
    globalLayout->addStretch();
    globalLayout->addLayout(layout);
    globalLayout->addStretch();
    globalLayout->setSpacing(0);
    return globalLayout;
}
