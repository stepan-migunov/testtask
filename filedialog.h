#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QDialog>

namespace Ui {
class FileDialog;
}

class FileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDialog(QWidget *parent = nullptr);
    FileDialog(QWidget *parent = nullptr, QString* image = nullptr, QString* shader = nullptr);
    ~FileDialog();

public slots:
    void checkImage();

private:
    Ui::FileDialog *ui;
    QString * image;
    QString * shader;
};

#endif // FILEDIALOG_H
