#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = nullptr);
    ~SignUpDialog();
    QString username() const;
    QString password() const;




private:
    Ui::SignUpDialog *ui;
private slots:
    void on_buttonBox_accepted();

};

#endif // SIGNUPDIALOG_H
