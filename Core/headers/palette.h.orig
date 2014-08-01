#ifndef PALETTE_H
#define PALETTE_H

#include <QWidget>
#include <QVector>
#include <QColor>

class QPushButton;
class QLayout;

class Palette : public QWidget
{
    Q_OBJECT
public:
    explicit Palette(QWidget *parent = 0);
    QColor getFirstColor()
    {
        return firstColor;
    }

    QColor getSecondColor()
    {
        return secondColor;
    }

signals:

public slots:

protected slots:
    void changeCurrentColor();
    void updateIndicator();
    void swapColors();
    void defaultColors();
    void setColor();
    void moreColors();

protected:
    void initStandardColors();
    QColor firstColor, secondColor;
    QColor* currentColor;
    QLayout* createIndicator();
    QLayout* createPalette();
    QPushButton* firstColorButton;
    QPushButton* secondColorButton;
    QVector<QColor> standardColors;
};

#endif // PALETTE_H
