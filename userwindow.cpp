#include "userwindow.h"
#include "ui_userwindow.h"
#include "previousfacture.h"
#include "previousfactureobject.h"

#include "facturemanagement.h"

UserWindow::UserWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    // Apply the style to the objectFacture button
    ui->objectFacture->setStyleSheet("QPushButton {"
                                     "border-radius: 15px;"
                                     "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                     "stop:0 #a6c1ee, stop: 1 #fbc2eb);"
                                     "padding: 6px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color:#3a3939;"
                                     "border : 1px solid #f2f2f2;"
                                     "color :#f2f2f2;"
                                     "}");

    // Apply the style to the tableauFacture button
    ui->tableauFacture->setStyleSheet("QPushButton {"
                                      "border-radius: 15px;"
                                      "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                      "stop:0 #a6c1ee, stop: 1 #fbc2eb);"
                                      "padding : 6px;"
                                      "}"
                                      "QPushButton:hover {"
                                      "background-color:#3a3939;"
                                      "border : 1px solid #f2f2f2;"
                                      "color :#f2f2f2;"
                                      "}");

    // Apply the style to the FactureManage button
    ui->FactureManage->setStyleSheet("QPushButton {"
                                     "border-radius : 15px;"
                                     "background-color:#a6c1ee;" // Blue color for FactureManage button
                                     "padding : 6px;"
                                     "}"
                                     "QPushButton:hover {"
                                     "background-color:#3a3939;" // Darker color when hovered
                                     "}");

}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::on_objectFacture_clicked()
{
    previousFactureObject *previousfactureobject = new previousFactureObject();
    previousfactureobject->show();
}


void UserWindow::on_tableauFacture_clicked()
{
    PreviousFacture *p= new PreviousFacture();
    p->show();
}

void UserWindow ::on_FactureManage_clicked(){

    factureManagement *z=new factureManagement();
    z->show();
}
