#ifndef OQLVIEW_H
#define OQLVIEW_H

#include <QWidget>
#include <QtOpenGL/qopenglwindow.h>
#include "filedialog.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class oqlview; }
QT_END_NAMESPACE

class oglview : public QWidget
{
    Q_OBJECT

public:
    oglview(QWidget *parent = nullptr);
    ~oglview();
    FileDialog* dialog = nullptr;

public slots:
    void openButtonClicked();
    void applyButtonClicked();
    void radioPressed();
    void textChanged();


private:
    QTimer* timer;
    Ui::oqlview *ui;
    bool hasImage = false;
    QString imagePath = ":/default.jpg";
    QString vertexShaderSource  =
            "vec4 vertexShader(vec4 vertex, float currTime)\n"
            "{\n"
            "   return vec4(vertex.xyz,1.0);\n"
            "}\n";
    QString fragmentShaderSource =
            "vec4 fragmentShader(vec4 aPos, vec2 texCoord, sampler2D texture, float currTime)\n"
            "{\n"
            "    return texture2D(texture, texCoord);\n"
            "}\n";

};
#endif // OQLVIEW_H
