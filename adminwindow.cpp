#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "mainwindow.h"
#include "usermanagement.h"

AdminWindow::AdminWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminWindow)
{
    ui->setupUi(this);
    ui->manageCompaniesButton->setStyleSheet("QPushButton {"
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
    ui->manageUsersButton->setStyleSheet("QPushButton {"
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

}

AdminWindow::~AdminWindow()
{
    delete ui;
}
void AdminWindow::on_manageCompaniesButton_clicked()
{
    // Open company management window
    MainWindow *mainWindow = new MainWindow();
    mainWindow->show();
}

void AdminWindow::on_manageUsersButton_clicked()
{
    // Open user management window
    userManagement *userManagementWindow = new userManagement();
    userManagementWindow->show();
}
