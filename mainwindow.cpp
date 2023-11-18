#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "databasehandler.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
                                 "background-color: #3a3939;"
                                 "border: 1px solid #f2f2f2;"
                                 "color: #f2f2f2;"
                                 "}");

    // Apply the style to the UpdateBtn
    ui->UpdateBtn->setStyleSheet("QPushButton {"
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

    // Apply the style to the deleteBtn
    ui->deleteBtn->setStyleSheet("QPushButton {"
                                 "border-radius: 15px;"
                                 "background-color:#ff0000;" // Red color for delete button
                                 "padding : 6px;"
                                 "}"
                                 "QPushButton:hover {"
                                 "background-color:#b30000;" // Darker red color when hovered
                                 "}");

    generateConfigFile();
    populateComboBox();
    qDebug() <<  QSqlDatabase :: drivers();
}

MainWindow::~MainWindow()
{
    delete ui;
    dbHandler.closeConnection();
}


void MainWindow::resetForm() {
    QLineEdit* nameBox = ui->nameBox;
    QLineEdit* contactBox = ui->contactBox;
    QLineEdit* telBox = ui->telBox;
    QLineEdit* tel2Box = ui->tel2Box;
    QLineEdit* faxBox = ui->faxBox;
    QLineEdit* fax2Box = ui->fax2Box;
    QLineEdit* adressBox = ui->adressBox;
    QLineEdit* matBox = ui->matBox;
    QLineEdit* emailBox = ui->emailBox;
    nameBox->setText("");
    contactBox->setText("");
    telBox->setText("");
    tel2Box->setText("");
    faxBox->setText("");
    fax2Box->setText("");
    adressBox->setText("");
    matBox->setText("");
    emailBox->setText("");
}

void MainWindow::populateComboBox()
{
    resetForm();
    QComboBox* comboBox = ui->comboBox;
    comboBox->clear();
    comboBox->addItem("Ajouter une nouvelle société", -1);

    if (dbHandler.openConnection()) {
        QList<Company> companies = dbHandler.getCompanies();
        for (const Company& company : companies) {
            QString name = company.getName();
            QString id = company.getId();

            // Create a QVariant containing the company ID
            QVariant companyIdVariant(id);

            // Add the company name to the combobox
            ui->comboBox->addItem(name, companyIdVariant);
        }
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.\nAssurez-vous d'apporter les modifications appropriées au fichier sfmi.ini");
        ::exit(0);
    }
}

void MainWindow::on_comboBox_activated(int index)
{

    QComboBox* comboBox = ui->comboBox;
    QVariant itemData = comboBox->itemData(index);
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    if (itemData.isValid()) {
        int id = itemData.toInt();
        QLineEdit* nameBox = ui->nameBox;
        QLineEdit* contactBox = ui->contactBox;
        QLineEdit* telBox = ui->telBox;
        QLineEdit* tel2Box = ui->tel2Box;
        QLineEdit* faxBox = ui->faxBox;
        QLineEdit* fax2Box = ui->fax2Box;
        QLineEdit* adressBox = ui->adressBox;
        QLineEdit* matBox = ui->matBox;
        QLineEdit* emailBox = ui->emailBox;
        if (id >-1 ) {
            Company company = dbHandler.getCompanyById(id);
            nameBox->setText(company.getName());
            contactBox->setText(company.getContact());
            telBox->setText(company.getTel());
            tel2Box->setText(company.getTel2());
            faxBox->setText(company.getFax());
            fax2Box->setText(company.getFax2());
            adressBox->setText(company.getAdr());
            matBox->setText(company.getMat());
            emailBox->setText(company.getEmail());
        } else {

            nameBox->setText("");
            contactBox->setText("");
            telBox->setText("");
            tel2Box->setText("");
            faxBox->setText("");
            fax2Box->setText("");
            adressBox->setText("");
            matBox->setText("");
            emailBox->setText("");
        }



    }

}

void MainWindow::generateConfigFile()
{
    QString configFile = "sfmi.ini";


    if (QFileInfo::exists(configFile)) {
        return; // Configuration file already exists, no need to generate
    }

    QSettings settings(configFile, QSettings::IniFormat);
    // Set default values for the connection parameters
    settings.setValue("Database/HostName", "");
    settings.setValue("Database/DatabaseName", "");
    settings.setValue("Database/TableName", "");
    settings.setValue("Database/UserName", "");
    settings.setValue("Database/Password", "");

    // You can add additional settings as needed

    settings.sync(); // Save the configuration file
}


void MainWindow::on_refreshBtn_clicked()
{
    populateComboBox();
}


void MainWindow::on_createBtn_clicked()
{
    if (!dbHandler.openConnection()) {
        qDebug() << "Failed to open database connection.";
        return;
    }
    Company company;
    QLineEdit* nameBox = ui->nameBox;
    QLineEdit* contactBox = ui->contactBox;
    QLineEdit* telBox = ui->telBox;
    QLineEdit* tel2Box = ui->tel2Box;
    QLineEdit* faxBox = ui->faxBox;
    QLineEdit* fax2Box = ui->fax2Box;
    QLineEdit* adressBox = ui->adressBox;
    QLineEdit* matBox = ui->matBox;
    QLineEdit* emailBox = ui->emailBox;
    company.setName(nameBox->text());
    company.setContact(contactBox->text());
    company.setTel(telBox->text());
    company.setTel2(tel2Box->text());
    company.setFax(faxBox->text());
    company.setFax2(fax2Box->text());
    company.setAdr(adressBox->text());
    company.setMat(matBox->text());
    company.setEmail(emailBox->text());
    if (dbHandler.createCompany(company)) {
        QMessageBox::information(this, "Succès", "La société a été ajoutée avec succès.");
        populateComboBox();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout d'une nouvelle société.");
    }


}


void MainWindow::on_UpdateBtn_clicked()
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
            Company company;
            QLineEdit* nameBox = ui->nameBox;
            QLineEdit* contactBox = ui->contactBox;
            QLineEdit* telBox = ui->telBox;
            QLineEdit* tel2Box = ui->tel2Box;
            QLineEdit* faxBox = ui->faxBox;
            QLineEdit* fax2Box = ui->fax2Box;
            QLineEdit* adressBox = ui->adressBox;
            QLineEdit* matBox = ui->matBox;
            QLineEdit* emailBox = ui->emailBox;
            company.setId(QString::number(id));
            company.setName(nameBox->text());
            company.setContact(contactBox->text());
            company.setTel(telBox->text());
            company.setTel2(tel2Box->text());
            company.setFax(faxBox->text());
            company.setFax2(fax2Box->text());
            company.setAdr(adressBox->text());
            company.setMat(matBox->text());
            company.setEmail(emailBox->text());
            if (dbHandler.updateCompany(company)) {
                QMessageBox::information(this, "Succès", "La société a été mis à jour avec succès.");
                populateComboBox();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de mis à jour de société.");
            }
        }
    }
}


void MainWindow::on_deleteBtn_clicked()
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
        if (dbHandler.deleteCompanyById(id)) {
            QMessageBox::information(this, "Succès", "La société a été supprimé avec succès.");
            populateComboBox();
        }else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression d'une société.");
        }

    }
}

