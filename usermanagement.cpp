#include "usermanagement.h"
#include "ui_usermanagement.h"
#include "user.h"
#include <Windows.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSettings>
#include <QFileInfo>
#include "qdebug.h"
#include <QtSql>
#include<QtCore>
#include"databasehandler.h"
#include <QMessageBox>


#include"QApplication"
userManagement::userManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::userManagement)
{
    ui->setupUi(this);
    // Apply the style to the refreshBtn
    ui->refreshBtn->setStyleSheet("QPushButton {"
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

    // Apply the style to the createBtn
    ui->createBtn->setStyleSheet("QPushButton {"
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

    // Apply the style to the UpdateBtn
    ui->UpdateBtn->setStyleSheet("QPushButton {"
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

    // Apply the style to the deleteBtn
    ui->deleteBtn->setStyleSheet("QPushButton {"
                                 "border-radius : 15px;"
                                 "background-color:#ff0000;" // Red color for delete button
                                 "padding : 6px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color:#b30000;" // Darker red color when hovered
                                 "}");

    populateComboBox();
}

userManagement::~userManagement()
{
    delete ui;
}
void userManagement::resetForm()
{
    QLineEdit* username = ui->username;
    QLineEdit* password = ui->password;

    username->setText("");
    password->setText("");

}

void userManagement::populateComboBox(){
    resetForm();
    QComboBox* comboBox = ui->comboBox;
    comboBox->clear();
    comboBox->addItem("Ajouter un user", -1);

    if (dbHandler.openConnection()) {
        QList<User> companies = dbHandler.getUsers();
        for (const User & company : companies) {
            QString name = company.getUsername();
            QString id = company.getId();


            // Create a QVariant containing the company ID
            QVariant companyIdVariant(id);

            // Add the company name to the combobox
            ui->comboBox->addItem(name, companyIdVariant);
        }
    } else {

        ::exit(0);
    }
}

void userManagement::on_comboBox_activated(int index)
{
    QComboBox* comboBox = ui->comboBox;
    QVariant itemData = comboBox->itemData(index);
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    if (itemData.isValid()) {
        int id = itemData.toInt();
        QLineEdit* nameBox = ui->username;
        QLineEdit* contactBox = ui->password;

        if (id >-1 ) {
            User company = dbHandler.getUserById(id);
            nameBox->setText(company.getUsername());
            contactBox->setText(company.getPassword());

        } else {

            nameBox->setText("");
            contactBox->setText("");

        }



    }

}

void userManagement::on_refreshBtn_clicked()
{
    populateComboBox();
}

void userManagement::on_createBtn_clicked()
{
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    User company;
    QLineEdit* nameBox = ui->username;
    QLineEdit* contactBox = ui->password;

    company.setUsername(nameBox->text());
    company.setPassword(contactBox->text());

    if (dbHandler.createeUser(company)) {
        QMessageBox::information(this, "Succès", "Le user a été ajoutée avec succès.");
        populateComboBox();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout le  nouveau user.");
    }


}

void userManagement::on_UpdateBtn_clicked()
{
    QComboBox* comboBox = ui->comboBox;
    int currentIndex = comboBox->currentIndex();
    QVariant itemData = comboBox->itemData(currentIndex);
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    if (itemData.isValid()) {
        int id = itemData.toInt();
        if (id >-1 ) {
            User company;
            QLineEdit* nameBox = ui->username;
            QLineEdit* contactBox = ui->password;

            company.setId(QString::number(id));
            company.setUsername(nameBox->text());
            company.setPassword(contactBox->text());

            if (dbHandler.updateUser(company)) {
                QMessageBox::information(this, "Succès", "Le user a été mis à jour avec succès.");
                populateComboBox();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de mis à jour de user.");
            }
        }
    }
}

void userManagement::on_deleteBtn_clicked()
{
    QComboBox* comboBox = ui->comboBox;
    int currentIndex = comboBox->currentIndex();
    QVariant itemData = comboBox->itemData(currentIndex);
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    if (itemData.isValid()) {
        int id = itemData.toInt();
        if (dbHandler.deleteUserById(id)) {
            QMessageBox::information(this, "Succès", "La société a été supprimé avec succès.");
            populateComboBox();
        }else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression d'une société.");
        }

    }
}
