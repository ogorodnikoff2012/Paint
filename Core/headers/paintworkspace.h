#ifndef PAINTWORKSPACE_H
#define PAINTWORKSPACE_H

#include <QWidget>
#include <QVector>
class QImage;
class QPaintEvent;
class QMouseEvent;
class QColor;
class QBrush;
class QPaintDevice;
class QWheelEvent;
class WorkspaceShell;

class PaintWorkspace : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWorkspace(WorkspaceShell* shell, QWidget *parent = 0, const int width = 800, const int height = 600);
    explicit PaintWorkspace(WorkspaceShell* shell, const QString& filename);
    ~PaintWorkspace();
    enum NewLayerType {Transparent, White, CurrentColor};
    QImage* newChange();
    QImage* getChange()
    {
        return change;
    }
    void applyChange();
    bool isChanged()
    {
        return changed;
    }
    void setName(const QString& name);
    bool save(bool forced = false);
    static QString getFilter();
    double getScale()
    {
        return scale;
    }
    void setScale(double scale)
    {
        if (scale >= minScale && scale <= maxScale)
            this->scale = scale;
    }
    static const double minScale;
    static const double maxScale;
    void changeScale(int direction);

signals:
    void nameChanged(const QString&);

public slots:
    void addNewLayer(const NewLayerType type = Transparent, const QColor& color = QColor(Qt::white));
    void zoomIn();
    void zoomOut();
    void restoreZoom();

protected:
    void initWorkspace(const int width = 0, const int height = 0);
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void paint(QPaintDevice* dev, QPaintEvent* evt = 0);
    void exportQtp(const QString& filename);
    void open(const QString& filename);
    void importQtp(const QString& filename);
    QString name;
    bool hasFile, changed;
    int mouseBlocker;
    QVector<QImage *> layers;
    QStringList layerNames;
    double scale;
    int layerWidth, layerHeight, curLayer, layerCounter;
    QImage* change;
    WorkspaceShell* shell;
};

#endif // PAINTWORKSPACE_H