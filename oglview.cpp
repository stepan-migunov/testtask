#include "oglview.h"
#include "ui_oqlview.h"

#include <QFile>
#include <QMessageBox>
#include <QTime>


oglview::oglview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::oqlview)
{
    ui->setupUi(this);
    ui->openGLWidget->setImagePathPointer(&imagePath);
    ui->textEdit->setText(vertexShaderSource);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,ui->openGLWidget,&GLWidget::applyShader);
};

oglview::~oglview()
{
    delete timer;
    delete ui;
}

void oglview::openButtonClicked()
{
    FileDialog dial(this, &imagePath);
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

void oglview::applyButtonClicked()
{
    ui->openGLWidget->setShader(vertexShaderSource, fragmentShaderSource);
    ui->openGLWidget->applyShader();
    timer->start(10);
}

void oglview::radioPressed()
{
    if(ui->fragmentRadio->isChecked())
        ui->textEdit->setText(fragmentShaderSource);
    if(ui->vertexRadio->isChecked())
        ui->textEdit->setText(vertexShaderSource);
}

void oglview::textChanged()
{
    if(ui->fragmentRadio->isChecked())
        fragmentShaderSource = ui->textEdit->toPlainText();
    if(ui->vertexRadio->isChecked())
        vertexShaderSource = ui->textEdit->toPlainText();
}

