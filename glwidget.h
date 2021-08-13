#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTime>


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    using QOpenGLWidget::QOpenGLWidget;
    ~GLWidget();
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void setImagePathPointer(QString* fileName);
    void setShader(const QString& vertexShaderSource);

public slots:
    void applyShader();
    void setImage();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
private:
    QOpenGLShaderProgram *makeShader(const QString &vertexShaderSource);
    void releaseTexture();
    void releaseShader();
    QColor clearColor = Qt::black;
    void makeObject();
    QOpenGLTexture *texture = nullptr;
    QOpenGLShaderProgram *shader = nullptr;
    QOpenGLBuffer vbo;

    QString* imageFilePathPointer;
    QTime time;

    const QString vertexShader =
            "attribute vec4 vertex;\n"
            "attribute vec2 texCoord;\n"
            "varying highp vec2 texc;\n"
            "varying vec4 aPos;\n"
            "void main(void)\n"
            "{\n"
            "    aPos = vec4(vertex.x, vertex.y, vertex.z, 1);\n"
            "    gl_Position = aPos;\n"
            "    texc = texCoord;\n"
            "}\n";

    const QString fragmentShader =
            "uniform sampler2D texture;\n"
            "uniform float currTime;\n"
            "varying highp vec2 texc;\n"
            "varying vec4 aPos;\n"
            "vec4 shaderMain(vec4,vec2,sampler2D, float);\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = shaderMain( aPos, texc, texture, currTime );\n"
            "}\n";

    const QString defaultShader =
            "vec4 shaderMain(vec4 aPos, vec2 texCoord, sampler2D texture, float currTime)\n"
            "{\n"
            "    return texture2D(texture, texCoord);\n"
            "}\n";
};

#endif // GLWIDGET_H
