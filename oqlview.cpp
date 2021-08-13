#include "oglview.h"
#include "ui_oqlview.h"

#include <QFile>
#include <QMessageBox>
#include <QTime>


oqlview::oqlview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::oqlview)
{
    ui->setupUi(this);
    ui->openGLWidget->setImagePathPointer(&imagePath);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,ui->openGLWidget,&GLWidget::applyShader);
};

oqlview::~oqlview()
{
    delete timer;
    delete ui;
}

void oqlview::openButtonClicked()
{
    FileDialog dial(this, &imagePath, &shaderPath);
    auto openFileCode = dial.exec();
    if(openFileCode == QDialog::Rejected)
        return;


    if(imagePath != "")
        this->setWindowTitle(imagePath + " - OpenGl shader viewer" );
    else
        this->setWindowTitle("OpenGl shader viewer" );

    if(QImage(imagePath).isNull())
    {
        imagePath = ":/default.jpg";
        QMessageBox::warning(this,"Warning",imagePath + "is not an image!!!");
    }
    auto openGlMain = ui->openGLWidget;
    openGlMain->setImagePathPointer(&imagePath);
    openGlMain->setImage();
}

void oqlview::applyButtonClicked()
{
    ui->openGLWidget->setShader(ui->textEdit->toPlainText());
    ui->openGLWidget->applyShader();
    timer->start(1);
    update();
}

