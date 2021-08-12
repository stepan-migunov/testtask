#include "oglview.h"
#include "ui_oqlview.h"

#include <QFile>


oqlview::oqlview(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::oqlview)
{
    ui->setupUi(this);
    ui->openGLWidget->setImagePathPointer(&imagePath);
};

oqlview::~oqlview()
{
    delete ui;
}

void oqlview::textChanged()
{

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

    auto openGlMain = ui->openGLWidget;
    openGlMain->setImagePathPointer(&imagePath);
    openGlMain->setImage();
}

void oqlview::applyButtonClicked()
{
    if(ui->textEdit->toPlainText().length()==0)
        ui->openGLWidget->setShader(defaultShader);
    else
        ui->openGLWidget->setShader(ui->textEdit->toPlainText());
    ui->openGLWidget->applyShader();
}

