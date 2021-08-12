#include "filedialog.h"
#include "ui_filedialog.h"

#include <QFile>
#include <QMessageBox>


FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
}

FileDialog::FileDialog(QWidget *parent, QString *image, QString *shader) :
    QDialog(parent),
    ui(new Ui::FileDialog),
    image(image),
    shader(shader)
{
    ui->setupUi(this);
    ui->imagePath->setText(*image);
    ui->shaderPath->setText(*shader);
}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::checkImage()
{
    QString imagePath = ui->imagePath->text();
    QString shaderFile = ui->shaderPath->text();

    if(QFile::exists(imagePath))
    {
        *image = imagePath;
    }
    else
    {
        QMessageBox::warning(this,"Warning","This file does not exist");
        return;
    }
    if(QFile::exists(shaderFile))
    {
        *shader = shaderFile;
    }
    else
    {
        *shader = "";
    }
    accept();
}

