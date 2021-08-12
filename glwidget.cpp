#include "glwidget.h"

#include <QException>
#include <QMessageBox>


QOpenGLShaderProgram* GLWidget::makeShader(const QString& vertexShaderSource, const QString& fragmentShaderSource)
{
    releaseShader();
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceCode(vertexShaderSource);
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceCode(fragmentShaderSource);


    QOpenGLShaderProgram* shader = new QOpenGLShaderProgram;
    shader->addShader(vshader);
    shader->addShader(fshader);
    shader->link();
    if(!shader->isLinked())
        throw std::logic_error("bad shader program!!!");
    return shader;
}


GLWidget::~GLWidget()
{
    makeCurrent();
    releaseTexture();
    releaseShader();
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(300,300);
}

QSize GLWidget::sizeHint() const
{
    return QSize(300,300);
}

void GLWidget::setImagePathPointer(QString *fileName)
{
    imageFilePathPointer = fileName;
}

void GLWidget::setShader(const QString &vertexShaderSource)
{
    releaseShader();
    shader = makeShader(vertexShaderSource,fragmentShader);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    setImage();
    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1


    const char *vsrc =
        "attribute vec4 vertex;\n"
        "attribute vec2 texCoord;\n"
        "varying highp vec2 texc;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = vec4(vertex.x, vertex.y, vertex.z, 1);\n"
        "    texc = texCoord;\n"
        "}\n";

    shader = makeShader(vsrc,fragmentShader);
    shader->bind();
    shader->setUniformValue("texture", 0);
}

void GLWidget::paintGL()
{
    vbo.bind();
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    shader->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    shader->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    shader->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN,0,6);
    //glDrawArrays(GL_TRIANGLE_STRIP,0,4);
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::releaseTexture()
{
    vbo.destroy();
    delete texture;
    texture = nullptr;
}

void GLWidget::releaseShader()
{
    delete shader;
    shader = nullptr;
}

void GLWidget::applyShader()
{
    paintGL();
    update();
}

void GLWidget::setImage()
{
    releaseTexture();
    try{
        QImage im(*imageFilePathPointer);
        if(im.isNull())
            throw;
        texture = new QOpenGLTexture(im.mirrored());
    }
    catch(...)
    {
        QMessageBox::warning(this,"", QString("Unable to load ") + *imageFilePathPointer + " as an texture");
    }
}

void GLWidget::makeObject()
{
    static const GLfloat coords[4][3] = {
        { +1, +1, 0 }, { -1, +1, 0 }, { -1, -1, 0 }, { +1, -1, 0 }
    };
    GLfloat height_ = texture->height();
    GLfloat width_ = texture->width();
    float x_scale = height_ >= width_ ? width_ / height_ : 1;
    float y_scale = height_ >= width_ ? 1 : height_ / width_;
    QList<GLfloat> vertData;
    for (int j = 0; j < 4; ++j) {
        // vertex position
        vertData.append(0.7f * x_scale * coords[j][0]);
        vertData.append(0.7f * y_scale * coords[j][1]);
        vertData.append(0.5f * coords[j][2]);
        // texture coordinate
        vertData.append(j == 0 || j == 3);
        vertData.append(j == 0 || j == 1);
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}


























