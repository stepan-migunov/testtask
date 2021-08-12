#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>


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
    QOpenGLShaderProgram *makeShader(const QString &vertexShaderSource, const QString &fragmentShaderSource);
    void releaseTexture();
    void releaseShader();
    QColor clearColor = Qt::black;
    void makeObject();
    QOpenGLTexture *texture = nullptr;
    QOpenGLShaderProgram *shader = nullptr;
    QOpenGLBuffer vbo;
    QString* imageFilePathPointer;
    const QString fragmentShader = "uniform sampler2D texture;\n"
            "varying highp vec2 texc;\n"
            "void main(void)\n"
            "{\n"
            "    gl_FragColor = texture2D(texture, texc);\n"
            //"    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
            "}\n";
};

#endif // GLWIDGET_H
