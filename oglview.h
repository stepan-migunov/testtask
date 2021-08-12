#ifndef OQLVIEW_H
#define OQLVIEW_H

#include <QWidget>
#include <QtOpenGL/qopenglwindow.h>
#include "filedialog.h"

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
    void textChanged();
    void openButtonClicked();
    void applyButtonClicked();


private:
    Ui::oqlview *ui;
    bool hasImage = false;
    QString imagePath = ":/default.jpg";
    QString shaderPath = "";
    const QString defaultShader = "attribute vec4 vertex;\n"
        "attribute vec2 texCoord;\n"
        "varying highp vec2 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1);\n"
        "    texc = texCoord;\n"
        "}\n";
};
#endif // OQLVIEW_H
