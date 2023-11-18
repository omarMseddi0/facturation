#include "signupdialog.h"
#include "ui_signupdialog.h"

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
    ui->buttonBox->setStyleSheet("QPushButton {"
                                 "border-radius: 15px;"
                                 "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                 "stop:0 #a6c1ee, stop: 1 #fbc2eb);"
                                 "padding: 6px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color: #3a3939;"
                                 "border: 1px solid #f2f2f2;"
                                 "color: #f2f2f2;"
                                 "}");
    ui->passwordEdit->setEchoMode(QLineEdit::Password);

}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

QString SignUpDialog::username() const
{
    return ui->usernameEdit->text();
}

QString SignUpDialog::password() const
{
    return ui->passwordEdit->text();
}

void SignUpDialog::on_buttonBox_accepted()
{
    if (username().isEmpty() || password().isEmpty()) {
        reject();
        return;
    }

    accept();
}

