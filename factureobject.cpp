#include "factureobject.h"
#include "ui_factureobject.h"
#include "factureobject.h"
#include "ui_factureobject.h"

#include "qsqlerror.h"
#include "qsqlquery.h"
#include<QFileDialog>

#include <QTableWidgetItem>
#include <QPrinter>
#include <QPainter>
#include <QWidget>
#include <QInputDialog>
#include "databasehandler.h"
#include <QSqlDatabase>

#include <QSqlQuery>
factureObject::factureObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::factureObject)
{
    ui->setupUi(this);
    // Apply the style to the addObjectButton


    // Apply the style to the addLineButton
    ui->addLineButton->setStyleSheet("QPushButton {"
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

    // Apply the style to the finishedButton
    ui->finishedButton->setStyleSheet("QPushButton {"
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

    // Apply the style to the update button
    ui->update->setStyleSheet("QPushButton {"
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

    // Apply the style to the browseButton
    ui->browseButton->setStyleSheet("QPushButton {"
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

    QDate currentDate = QDate::currentDate();
    ui->dateLabel->setText(currentDate.toString("dd/MM/yyyy"));
    // Set column headers for second table
    QStringList headers;
    headers << "OBJET" << "MONTANT";
    ui->secondTable->setColumnCount(headers.size());
    ui->secondTable->setHorizontalHeaderLabels(headers);
}

factureObject::~factureObject()
{
    delete ui;
}
void factureObject::setCompanyName(const QString &name)
{
    ui->companyNameLabel->setText(name);
}

void factureObject::setCompanyId(const QString &id)
{
    ui->companyIdLabel->setText(id);
}





void factureObject::setTel(const QString &tel)
{
    ui->telBox->setText(tel);
}

void factureObject::setMatFisc(const QString &MatFisc)
{
    ui->MatFiscspinBox->setText(MatFisc);
}

void factureObject::setaddressLineEdit(const QString &address)
{
    ui->addressSpinBox->setText(address);
}
void factureObject::on_addLineButton_clicked()
{
    // Add a new row to the second table
    int row = ui->secondTable->rowCount();
    ui->secondTable->insertRow(row);

    // Initialize all cells
    for (int column = 0; column < ui->secondTable->columnCount(); ++column) {
        if (column == 0) {
            QLineEdit *lineEdit = new QLineEdit();
            ui->secondTable->setCellWidget(row, column, lineEdit);
        } else {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setData(Qt::EditRole, 0.0);
            ui->secondTable->setItem(row, column, item);
        }
    }

    // Update calculated totals
    updateAmounts();
}

void factureObject::on_finishedButton_clicked()
{
    // Increment facture ID in database
    DatabaseHandler db;
    db.closeConnection();
    db.openConnection();
    QSqlQuery maxIdQuery(db.m_database);
   maxIdQuery.prepare("SELECT MAX(CAST(id AS INT)) FROM facture");
    int nextId = 1;
    if (maxIdQuery.exec() && maxIdQuery.next()) {
        nextId = maxIdQuery.value(0).toInt() + 1;
    }
    QString strNextId = QString::number(nextId);
    QSqlQuery query(db.m_database);
    query.prepare("INSERT INTO facture (id , date , company) VALUES (:id, :date ,:company)");
    query.bindValue(":id", strNextId);
    query.bindValue(":date",  ui->dateLabel->text());
    query.bindValue(":company", ui->companyNameLabel->text());

    if (query.exec()) {
        qDebug() << "Facture ID incremented successfully.";
    } else {
        qDebug() << "Failed to increment facture ID:" << query.lastError().text();
    }

    // Update facture ID in UI
    ui->factureIdLabel->setText(QString::number(nextId));

    // Generate PDF of invoice using QPrinter
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("factureobject.pdf");

    QPainter painter(&printer);
    // Draw image
    if (!selectedImageFileName.isEmpty()) {
        QPixmap pix(selectedImageFileName);
        pix = pix.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation); // scale pixmap
        painter.drawPixmap(0, 0, pix);
    }
    // Draw title
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::red);
    QRectF titleRect = painter.boundingRect(printer.pageRect(QPrinter::DevicePixel), Qt::AlignCenter, "Facture");

    painter.drawText(printer.pageRect(QPrinter::DevicePixel).center().x() - titleRect.width() / 2, 30, "Facture");

    // Draw first table
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);
    painter.setPen(Qt::black);
    int y = 60;
    y+=40;

    // Draw first table rows
    QStringList leftColumn = {"SMCI", "Société de Maintenance des Circuits Idustriels", "Matricule Fiscale : 1722178/ABC", "À : FRIDHI FAYCEL", "SMCI", "Residence Ayoub Hamam Life 2050", "", "99 425 059"};
    QStringList rightColumn = {"Date : " + ui->dateLabel->text(), "N° de facture : " + ui->factureIdLabel->text(), "Matricule Fiscale : " + ui->MatFiscspinBox->text(), "Expédier à : " + ui->companyNameLabel->text(), "addresse : " + ui->addressSpinBox->text(), "tel" + ui->telBox->text(), "", ""};
    for (int row = 0; row < leftColumn.size(); row++) {
        QRectF leftRect = painter.boundingRect(QRectF(10, y, printer.pageRect(QPrinter::DevicePixel).width(), 20), Qt::AlignLeft, leftColumn[row]);
        painter.drawText(10, y, leftColumn[row]);
        QRectF rightRect = painter.boundingRect(QRectF(printer.pageRect(QPrinter::DevicePixel).left(), y, printer.pageRect(QPrinter::DevicePixel).width(), 20), Qt::AlignRight, rightColumn[row]);
        painter.drawText(printer.pageRect(QPrinter::DevicePixel).right() - rightRect.width() - 10, y, rightColumn[row]);
        y += 20;
    }

    // Draw second table
    font.setBold(true);
    painter.setFont(font);

    // Define rowHeight variable and set its value

    int rowHeighte = 30;

    // Define pageHeight variable and set its value
    int pageHeight = printer.pageRect(QPrinter::DevicePixel).height();

    // Check if the content exceeds the current page and start a new page if necessary
    if (y + rowHeighte > pageHeight) {
        printer.newPage();
        y = 0;
    }

    y += 40;

    // Draw second table rows
    font.setBold(false);
    painter.setFont(font);

    for (int row = 0; row < ui->secondTable->rowCount(); row++) {
        QLineEdit *objectLineEdit = qobject_cast<QLineEdit*>(ui->secondTable->cellWidget(row, 0));
        QTableWidgetItem *amountItem = ui->secondTable->item(row,1);

        if(objectLineEdit && amountItem){
            // Calculate the height of the text when it's wrapped to colWidth
            int textHeightObject = painter.fontMetrics().boundingRect(0, 0, printer.pageRect(QPrinter::DevicePixel).width() - 20 , INT_MAX, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, objectLineEdit->text()).height();
            int textHeightAmount = painter.fontMetrics().boundingRect(0, 0, printer.pageRect(QPrinter::DevicePixel).width() - 20 , INT_MAX, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, amountItem->text()).height();
            int maxTextHeight = qMax(textHeightObject,textHeightAmount);

            font.setPointSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(10 , y , printer.pageRect(QPrinter::DevicePixel).width() - 20, maxTextHeight, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, "Objet:");
            y += maxTextHeight;
            font.setPointSize(12);
            font.setBold(false);
            painter.setFont(font);
            painter.drawText(10 , y , printer.pageRect(QPrinter::DevicePixel).width() - 20, maxTextHeight, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, objectLineEdit->text());
            y += maxTextHeight;
            font.setPointSize(14);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(10 , y , printer.pageRect(QPrinter::DevicePixel).width() - 20, maxTextHeight, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, "Montant:");
            y += maxTextHeight;
            font.setPointSize(12);
            font.setBold(false);
            painter.setFont(font);
            painter.drawText(10 , y , printer.pageRect(QPrinter::DevicePixel).width() - 20, maxTextHeight, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, amountItem->text());
            y += maxTextHeight;

            // Check if the content exceeds the current page and start a new page if necessary
            if (y + maxTextHeight > pageHeight) {
                printer.newPage();
                y = 0;
            }
        }
    }

    font.setBold(true);
    painter.setFont(font);
    y += 40;

    int pageWidth = printer.pageRect(QPrinter::DevicePixel).width();
    int halfPageWidth = pageWidth / 2;
    int colWidth = pageWidth / 6;


    QStringList totalLabels = {"Total HTVA:", "Timbre Fiscal:", "Total TTC:"};
    QStringList totalValues = {ui->totalHTVALineEdit->text(), QString::number(ui->timbreFiscalSpinBox->value()), ui->totalTTCLineEdit->text()};

    // Check if remise is 0
    QString remiseStr = ui->remiseSpinBox->text();
    double remise = remiseStr.toDouble();
    if (remise != 0) {
        totalLabels.insert(1, "Remise:  "+ ui->remiseSpinBox->text()+"%");
        totalValues.insert(1, ui->totalRemiseLineEdit->text());
    }

    // Check if tva is 0
    QString tvaa = ui->tvaSpinBox->text();
    double tva = tvaa.toDouble();
    if (tva != 0) {
        totalLabels.insert(2, "TVA:   "+ui->tvaSpinBox->text()+"%");
        totalValues.insert(2, ui->totalTVALineEdit->text());
    }

    for (int i = 0; i < totalLabels.size(); i++) {
        QRect rect3(10, y, colWidth * 3, rowHeighte);
        painter.drawRect(rect3);
        painter.drawText(rect3.adjusted(5, -5, -5, -5), Qt::AlignLeft, totalLabels[i]);
        QRect rect4(10 + colWidth * 3, y, colWidth * 3, rowHeighte);
        painter.drawRect(rect4);
        painter.drawText(rect4.adjusted(5, -5, -5, -5), Qt::AlignLeft, totalValues[i]);
        y += rowHeighte;

        // Check if the content exceeds the current page and start a new page if necessary
        if (y + rowHeighte > pageHeight) {
            printer.newPage();
            y = 0;
        }
    }

    // Draw total TTC in words
    font.setBold(false);
    painter.setFont(font);
    y += 40;
    painter.drawText(10, y, ui->totalTTCInWordsLabel->text());
}

void factureObject::updateAmounts()
{
    // Calculate total HTVA
    double totalHTVA = 0;
    for (int row = 0; row < ui->secondTable->rowCount(); row++) {
        QTableWidgetItem* amountItem = ui->secondTable->item(row, 1);

        if(amountItem){
            double amount = amountItem->text().toDouble();
            totalHTVA += amount;
        }
    }

    // Calculate total remise
    QString remiseStr = ui->remiseSpinBox->text();
    double remise = remiseStr.toDouble();

    double totalRemise = (totalHTVA * remise) / 100;


    // Calculate total TVA
    QString tvaa = ui->tvaSpinBox->text();
    double tva = tvaa.toDouble();
    double totalTVA = (totalHTVA - totalRemise) * tva / 100;

    // Calculate total TTC
    double timbreFiscal = static_cast<double>(ui->timbreFiscalSpinBox->value());
    double totalTTC = totalHTVA - totalRemise + totalTVA + timbreFiscal;

    // Convert total TTC to words
    double dinars = static_cast<int>(totalTTC);
    double millimes = std::round((totalTTC - dinars) * 1000);
    QString totalTTCInWords = convertDinarsAndMillimes(dinars, millimes);

    // Update calculated totals in UI
    ui->totalHTVALineEdit->setText(QString::number(totalHTVA, 'f', 3));
    ui->totalRemiseLineEdit->setText(QString::number(totalRemise, 'f', 3));
    ui->totalTVALineEdit->setText(QString::number(totalTVA, 'f', 3));
    ui->totalTTCLineEdit->setText(QString::number(totalTTC, 'f', 3));
    ui->totalTTCInWordsLabel->setText(totalTTCInWords);
}

QString factureObject::numberToWords(int num) {
    if (num < 0 || num >= 1000000) {
        return "Number out of range";
    }

    QStringList units = {"", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf"};
    QStringList teens = {"dix", "onze", "douze", "treize", "quatorze", "quinze", "seize", "dix-sept", "dix-huit", "dix-neuf"};
    QStringList tens = {"", "dix", "vingt", "trente", "quarante", "cinquante", "soixante", "soixante-dix", "quatre-vingt", "quatre-vingt-dix"};
    QStringList hundreds = {"", "cent", "deux cents", "trois cents", "quatre cents", "cinq cents", "six cents", "sept cents", "huit cents", "neuf cents"};

    auto getUnits = [&](int n) -> QString {
        return units[n % 10];
    };

    auto getTens = [&](int n) -> QString {
        if (n < 10) {
            return getUnits(n);
        } else if (n >= 10 && n < 20) {
            return teens[n - 10];
        } else {
            int remainder = n % 10;
            if (n / 10 == 7 || n / 10 == 9) {
                return tens[n / 10 - 1] + "-" + teens[remainder];
            }
            else if (remainder == 1 && n / 10 != 8) {
                return tens[n / 10] + "-et-" + getUnits(remainder);
            } else {
                return tens[n / 10] + (remainder != 0 ? ("-" + getUnits(remainder)) : "");
            }
        }
    };

    auto getHundreds = [&](int n) -> QString {
        if (n % 100 != 0) {
            return hundreds[n / 100] + " " + getTens(n % 100);
        } else {
            return hundreds[n / 100];
        }
    };

    auto getThousands = [&](int n) -> QString {
        if (n < 1000) {
            return getHundreds(n);
        } else {
            int thousands = n / 1000;
            int remainder = n % 1000;
            return ((thousands == 1 ? "" : numberToWords(thousands)) + " mille" +
                    (remainder != 0 ? (" " + getHundreds(remainder)) : ""));
        }
    };

    return getThousands(num);
}


QString factureObject::convertDinarsAndMillimes(int dinars, int millimes) {
    return numberToWords(dinars) + " dinars et "  + QString::number(millimes) + " millimes"  ;
}

void factureObject::on_update_clicked()
{
    updateAmounts();
}
void factureObject::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/path/to/images/", tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty()) {
        selectedImageFileName = fileName;
    }
}
