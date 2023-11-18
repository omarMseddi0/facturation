#include "facturemanagement.h"
#include "ui_facturemanagement.h"
#include <QSqlQuery>
#include <QSqlError>
#include "databasehandler.h"

#include "facturemanagement.h"
#include "ui_facturemanagement.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "databasehandler.h"

factureManagement::factureManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::factureManagement)
{
    ui->setupUi(this);

    // Apply the style to the deleteButton
    ui->deleteButton->setStyleSheet("QPushButton {"
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

    db.openConnection();

    // Fill the first combobox with unique company names
    QSqlQuery query(db.m_database);
    query.prepare("SELECT DISTINCT company FROM facture");
    if (query.exec()) {
        while(query.next()) {
            ui->comboBox->addItem(query.value(0).toString());
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

factureManagement::~factureManagement()
{
    delete ui;
    db.closeConnection();
}

void factureManagement::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->comboBox_2->clear();
    QSqlQuery query(db.m_database);
    query.prepare("SELECT id FROM facture WHERE company = :company");
    query.bindValue(":company", arg1);
    if(query.exec()) {
        while(query.next()) {
            ui->comboBox_2->addItem(query.value(0).toString());
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

void factureManagement::on_comboBox_2_currentIndexChanged(int index)
{
    QString id = ui->comboBox_2->itemText(index);
    QSqlQuery query(db.m_database);
    query.prepare("SELECT date FROM facture WHERE id = :id");
    query.bindValue(":id", id);
    if(query.exec()) {
        if(query.next()) {
            ui->lineEdit->setText(query.value(0).toString());
        }
    } else {
        qDebug() << query.lastError().text();
    }
}

void factureManagement::on_deleteButton_clicked()
{
    QString id = ui->comboBox_2->currentText();
    QSqlQuery query(db.m_database);
    query.prepare("DELETE FROM facture WHERE id = :id");
    query.bindValue(":id", id);
    if(query.exec()) {
        QMessageBox::information(this, "Success", "Facture deleted successfully!");
        // Refresh company list
        QString currentCompany = ui->comboBox->currentText();
        ui->comboBox->clear();
        QSqlQuery query(db.m_database);
        query.prepare("SELECT DISTINCT company FROM facture");
        if (query.exec()) {
            while(query.next()) {
                ui->comboBox->addItem(query.value(0).toString());
            }
            int index = ui->comboBox->findText(currentCompany);
            if (index != -1) {
                // If the current company still exists, set it as the current item
                ui->comboBox->setCurrentIndex(index);
            } else {
                // If the current company no longer exists, clear the second combobox and line edit
                ui->comboBox_2->clear();
                ui->lineEdit->clear();
            }
        } else {
            qDebug() << query.lastError().text();
        }
    } else {
        qDebug() << query.lastError().text();
    }
}
