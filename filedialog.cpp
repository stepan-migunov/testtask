#include "filedialog.h"
#include "ui_filedialog.h"

#include <QFile>
#include <QFileInfo>
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
}

FileDialog::~FileDialog()
{
    delete ui;
}

void FileDialog::checkImage()
{
    QString imagePath = ui->imagePath->text();

    if(QFile::exists(imagePath) && QFileInfo(imagePath).isFile())
    {
        *image = imagePath;
    }
    else
    {
        QMessageBox::warning(this,"Warning","Could not open the file " + imagePath);
        return;
    }
    accept();
}

