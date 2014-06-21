#include "headers\workspaceshell.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int WorkspaceShell::untitledCounter = 0;

WorkspaceShell::WorkspaceShell(QWidget *parent, const int width, const int height, const QString &str) :
    QWidget(parent)
{
    initWorkspace(width, height, str);
    setAttribute(Qt::WA_DeleteOnClose);
}

void WorkspaceShell::initWorkspace(const int width, const int height, const QString &str)
{
    if (str.isEmpty())
        wsp = new PaintWorkspace(this, 0, width, height);
    else
        wsp = new PaintWorkspace(this, str);
    QFrame* f = new QFrame;
    QScrollArea* a = new QScrollArea;

    f->setFrameStyle(QFrame::Box | QFrame::Plain);
    f->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    f->setLineWidth(1);

    QPalette pal;
    pal.setColor(QPalette::Background, QColor(Qt::lightGray));
    a->setPalette(pal);
    a->setAutoFillBackground(true);

    QBoxLayout* scrollAreaLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    scrollAreaLayout->setMargin(0);
    scrollAreaLayout->addWidget(wsp);
    f->setLayout(scrollAreaLayout);
    a->setWidget(f);
    a->setAlignment(Qt::AlignCenter);
    connect(wsp, SIGNAL(nameChanged(QString)), SLOT(setWindowTitle(QString)));
    if (str.isEmpty())
        wsp->setName(tr("Untitled ") + QString().setNum(nextUntitled()));
    else
        wsp->setName(str);

    vRuler = new VerticalRuler(this);
    connect(a->verticalScrollBar(), SIGNAL(valueChanged(int)), vRuler, SLOT(update()));
    hRuler = new HorizontalRuler(this);
    connect(a->horizontalScrollBar(), SIGNAL(valueChanged(int)), hRuler, SLOT(update()));

    QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout* layout1 = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout* layout2 = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->addLayout(layout1);
    layout->addLayout(layout2, 1);

    QLabel* lbl = new QLabel("<big><b>px");
    lbl->setMinimumSize(RULER_SIZE, RULER_SIZE);
    lbl->setAlignment(Qt::AlignCenter);
    layout1->addWidget(lbl);
    layout1->addWidget(hRuler, 1);
    layout1->setSizeConstraint(QLayout::SetMinimumSize);

    layout2->addWidget(vRuler);
    layout2->addWidget(a, 1);
    layout2->setSizeConstraint(QLayout::SetMinimumSize);

    layout->setMargin(0);
    layout->setSpacing(0);
    layout1->setMargin(0);
    layout1->setSpacing(0);
    layout2->setMargin(0);
    layout2->setSpacing(0);

    setLayout(layout);

}

void WorkspaceShell::closeEvent(QCloseEvent *evt)
{
    if (wsp->isChanged()) {
        QString title = windowTitle().remove(0, 1);
        int ans = QMessageBox::warning(0, tr("Warning!"), tr("Do you want to save \"") + title + tr("\" before closing?"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        switch (ans) {
        case QMessageBox::Yes:
            if (wsp->save())
                evt->accept();
            else
                evt->ignore();
            break;
        case QMessageBox::No:
            evt->accept();
            break;
        default:
            evt->ignore();
            break;
        }
    } else
        evt->accept();
}
