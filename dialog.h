#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
class QLineEdit;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    QLineEdit* Edit;

private slots:
    void deletebtn();
    void enterbtn();
};
#endif // DIALOG_H
