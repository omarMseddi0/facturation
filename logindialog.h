#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H


#include "qdebug.h"

#include <QDialog>
#include "databasehandler.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private slots:
    void on_loginButton_clicked();
    void on_signUpButton_clicked();

private:
    Ui::LoginDialog *ui;
    DatabaseHandler db;
};

#endif // LOGINDIALOG_H

