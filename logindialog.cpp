#include "logindialog.h"

#include "databasehandler.h"
#include "logindialog.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSettings>
#include <QFileInfo>
#include "adminwindow.h"
#include "ui_logindialog.h"
#include "userwindow.h"
#include "signupdialog.h"
#include <QMessageBox>
#include "qdebug.h"
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    ui->usernameEdit->setPlaceholderText("Username");
    ui->usernameEdit->setStyleSheet("QLineEdit {"
                                    "border: 1px solid gray;"
                                    "padding: 0 8px;"
                                    "selection-background-color: darkgray;"
                                    "}");

    ui->passwordEdit->setPlaceholderText("Password");
    ui->passwordEdit->setStyleSheet("QLineEdit {"
                                    "border: 1px solid gray;"
                                    "padding: 0 8px;"
                                    "selection-background-color: darkgray;"
                                    "}");
    this->setStyleSheet("QDialog {"
                        "background-color: #f2f2f2;"
                        "}");
    ui->loginButton->setStyleSheet("QPushButton {"
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
    this->setStyleSheet("QDialog {"
                        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                        "stop:0 #a6c1ee, stop: 1 #fbc2eb);"
                        "}");
    ui->signUpButton->setStyleSheet("QPushButton {"
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
    qDebug() <<  QSqlDatabase :: drivers();

}




LoginDialog::~LoginDialog()
{
    delete ui;
  db.closeConnection();
}

void LoginDialog::on_loginButton_clicked()
{
    // Retrieve entered username and password
    QString username = ui->usernameEdit->text();
    QString password = ui->passwordEdit->text();

    // Check credentials against database
    if (db.checkCredentials(username, password)) {
        // Check if user is an admin
        if (db.isAdmin(username)) {
            // Open admin window
            AdminWindow *adminWindow = new AdminWindow();
            adminWindow->show();
            close();
        } else {
            // Open user window
            UserWindow *userWindow = new UserWindow();
            userWindow->show();
            close();
        }
    } else {
        // Show error message
        QMessageBox::critical(this, "Error", "Invalid username or password.");
    }
}

void LoginDialog::on_signUpButton_clicked()
{
    // Open sign-up dialog
    SignUpDialog signUpDialog(this);
    if (signUpDialog.exec() == QDialog::Accepted) {
        // Retrieve entered username and password
        QString username = signUpDialog.username();
        QString password = signUpDialog.password();

        // Create new user account in database
        if (db.createUser(username, password)) {
            QMessageBox::information(this, "Success", "User account created successfully.");
        } else {
            QMessageBox::critical(this, "Error", "Failed to create user account.");
        }
    }
}

