#ifndef OQLVIEW_H
#define OQLVIEW_H

#include <QWidget>
#include <QtOpenGL/qopenglwindow.h>
#include "filedialog.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class oqlview; }
QT_END_NAMESPACE

class oqlview : public QWidget
{
    Q_OBJECT

public:
    oqlview(QWidget *parent = nullptr);
    ~oqlview();
    FileDialog* dialog = nullptr;

public slots:
    void openButtonClicked();
    void applyButtonClicked();


private:
    QTimer* timer;
    Ui::oqlview *ui;
    bool hasImage = false;
    QString imagePath = ":/default.jpg";
    QString shaderPath = "";

};
#endif // OQLVIEW_H
