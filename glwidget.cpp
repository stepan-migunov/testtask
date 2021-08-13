#include "glwidget.h"

#include <QException>
#include <QMessageBox>

QOpenGLShaderProgram* GLWidget::makeShader(const QString &vertexShaderSource, const QString &fragmentShaderSource)
{

    if(fragmentShaderSource.length()==0 && vertexShaderSource.length() == 0)
    {
        return makeShader(defaultUserVertexShader, defaultUserFragmentShader);
    }

    if(fragmentShaderSource.length()==0)
    {
        return makeShader(vertexShaderSource, defaultUserFragmentShader);
    }

    if(vertexShaderSource.length()==0)
    {
        return makeShader(defaultUserVertexShader, fragmentShaderSource);
    }

    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceCode(vertexShader);

    QOpenGLShader *userVertexShader = new QOpenGLShader(QOpenGLShader::Vertex,this);
    userVertexShader->compileSourceCode(vertexShaderSource);

    if(!userVertexShader->isCompiled())
    {
        delete vshader;
        auto e = std::logic_error(userVertexShader->log().toStdString());
        delete userVertexShader;
        throw e;
    }

    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceCode(fragmentShader);

    QOpenGLShader *userFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    userFragmentShader->compileSourceCode(fragmentShaderSource);
    if(!userFragmentShader->isCompiled())
    {
        delete vshader;
        delete fshader;
        delete userVertexShader;
        auto e = std::logic_error(userFragmentShader->log().toStdString());
        delete userFragmentShader;
        throw e;
    }

    QOpenGLShaderProgram* shaderProgram = new QOpenGLShaderProgram;
    shaderProgram->addShader(vshader);
    shaderProgram->addShader(fshader);
    shaderProgram->addShader(userVertexShader);
    shaderProgram->addShader(userFragmentShader);
    shaderProgram->link();
    delete vshader;
    delete fshader;
    delete userFragmentShader;
    delete userVertexShader;
    if(!shaderProgram->isLinked())
    {
        auto e = std::logic_error(shaderProgram->log().toStdString());
        delete shaderProgram;
        throw e;
    }
    return shaderProgram;
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

void GLWidget::setShader(const QString &vertexShaderSource, const QString &fragmentShaderSource)
{
    releaseShader();
    try {
        shader = makeShader(vertexShaderSource, fragmentShaderSource);
    }  catch (const std::logic_error& e) {
        shader = makeShader(defaultUserVertexShader,defaultUserFragmentShader);
        QMessageBox::warning(this,"Error occurred",e.what());
    }
    shader->bind();
    shader->setUniformValue("texture", 0);
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

    shader = makeShader(defaultUserVertexShader, defaultUserFragmentShader);
    shader->bind();
    shader->setUniformValue("texture", 0);
    shader->setUniformValue("currTime",(GLfloat)QTime().msecsSinceStartOfDay());
}

void GLWidget::paintGL()
{

    shader->bind();
    vbo.bind();
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    shader->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    shader->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    shader->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    shader->setUniformValue("currTime",(GLfloat)QTime::currentTime().msecsSinceStartOfDay());

    texture->bind();
    glDrawArrays(GL_TRIANGLE_FAN,0,4);
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
    //paintGL();
    update();
}

void GLWidget::setImage()
{
    releaseTexture();
    try{
        texture = new QOpenGLTexture(QImage(*imageFilePathPointer).mirrored());
        makeObject();
    }
    catch(std::exception& e)
    {
        delete texture;
        texture = new QOpenGLTexture(QImage(":default.jpg").mirrored());
        QMessageBox::warning(this,"", QString("Unable to load ") + *imageFilePathPointer + " as an texture\n" + e.what());
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
    for (int j = 0; j < 4; ++j)
    {
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


























