#include "facture.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_facture.h"
#include <QTableWidgetItem>
#include <QPrinter>
#include <QPainter>
#include <QWidget>
#include <QInputDialog>
#include "databasehandler.h"
#include <QSqlDatabase>
#include<QFileDialog>
#include <QSqlQuery>
Facture::Facture(QWidget *parent) :
    QDialog(parent),
ui(new Ui::Facture)
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
    headers << "N°     " << "          DESIGNATION        " << "UNITE" << "QUANTITE" << "P.UNITAIRE" << "MONTANT";
               ui->secondTable->setColumnCount(headers.size());
    ui->secondTable->setHorizontalHeaderLabels(headers);
    ui->secondTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

Facture::~Facture()
{
    delete ui;
}

void Facture::setCompanyName(const QString &name)
{
    ui->companyNameLabel->setText(name);
}

void Facture::setCompanyId(const QString &id)
{
    ui->companyIdLabel->setText(id);
}





void Facture::setTel(const QString &tel)
{
    ui->telBox->setText(tel);
}

void Facture::setMatFisc(const QString &MatFisc)
{
    ui->MatFiscspinBox->setText(MatFisc);
}

void Facture::setaddressLineEdit(const QString &address)
{
    ui->addressSpinBox->setText(address);
}

void Facture::on_addObjectButton_clicked()
{
    // Prompt user to enter object name
    QString objectName = QInputDialog::getText(this, "Add Object", "Enter object name:");

    // Add new row to second table with object name
    int row = ui->secondTable->rowCount();
    ui->secondTable->insertRow(row);
    ui->secondTable->setItem(row, 0, new QTableWidgetItem(objectName));
    // Initialize rest of the columns in the new row
    ui->secondTable->setItem(row, 1, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 2, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 3, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 4, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 5, new QTableWidgetItem(""));

}

void Facture::on_addLineButton_clicked()
{
    // Add a new row to the second table
    int row = ui->secondTable->rowCount();
    ui->secondTable->insertRow(row);

    // Initialize all cells
    for (int column = 0; column < ui->secondTable->columnCount(); ++column) {
        ui->secondTable->setItem(row, column, new QTableWidgetItem(""));
    }

    // Calculate row number
    int objectNumber = 1;
    int lineNumber = 1;
    for (int i = 0; i < row; i++) {
        if (ui->secondTable->item(i, 0)->text().isEmpty()) {
            lineNumber++;
        } else {
            objectNumber++;
            lineNumber = 1;
        }
    }

    QString rowNumber = QString::number(objectNumber) + "." + QString::number(lineNumber);

    // Add cells for row number, designation, unit, quantity, unit price, and amount
    ui->secondTable->setItem(row, 0, new QTableWidgetItem(rowNumber));
    ui->secondTable->setItem(row, 1, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 2, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 3, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 4, new QTableWidgetItem(""));
    ui->secondTable->setItem(row, 5, new QTableWidgetItem(""));

    // Update calculated totals
    updateAmounts();
}

void Facture::on_finishedButton_clicked()
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
    ui->factureIdLabel->setText(strNextId);
    // Generate PDF of invoice using QPrinter
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("facture.pdf");

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
    y += 40;
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

    // Draw second table headers with borders
    QStringList headers = {"N° ", "DESIGNATION  ", "UNITE", "QUANTITE", "P.UNITAIRE", "MONTANT"};
    int colWidth = (printer.pageRect(QPrinter::DevicePixel).width() - 20) / headers.size();

    for (int col = 0; col < headers.size(); col++) {
        QRect rect(10 + col * colWidth, y, colWidth, rowHeighte);
        painter.drawRect(rect);
        painter.drawText(rect, Qt::AlignCenter, headers[col]);
    }
    y += rowHeighte;

    // Draw second table rows with borders
    // Draw second table rows with borders
    font.setBold(false);
    painter.setFont(font);

    QRect rect;

    for (int row = 0; row < ui->secondTable->rowCount(); row++) {
        int maxTextHeight = 0;
        for (int col = 1; col < ui->secondTable->columnCount(); col++) { // Start from col 1
            QTableWidgetItem *item = ui->secondTable->item(row,col);
            if (item) {
                // Calculate the height of the text when it's wrapped to colWidth, and find the maximum height
                int textHeight = painter.fontMetrics().boundingRect(0, 0, colWidth, INT_MAX, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, item->text()).height();
                maxTextHeight = qMax(maxTextHeight, textHeight);
            }
        }

        // Now draw the first column with maxTextHeight
        QTableWidgetItem *firstItem = ui->secondTable->item(row, 0);
        if (firstItem) {
            rect.setRect(10, y, colWidth, maxTextHeight);
            painter.drawRect(rect);
            painter.drawText(rect.adjusted(5,0,-5,0), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, firstItem->text());
        }

        // Draw the rest of the columns
        for (int col = 1; col < ui->secondTable->columnCount(); col++) {
            QTableWidgetItem *item = ui->secondTable->item(row,col);
            if (item) {
                rect.setRect(10 + col * colWidth, y, colWidth, maxTextHeight);
                painter.drawRect(rect);
                painter.drawText(rect.adjusted(5,0,-5,0), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, item->text());
            }
        }

        y += maxTextHeight;

        // Check if the content exceeds the current page and start a new page if necessary
        if (y + maxTextHeight > pageHeight) {
            printer.newPage();
            y = 0;
        }
    }


    bool remiseIsZero = false;
    QString remiseText = ui->remiseSpinBox->text();
    double remise = remiseText.toDouble();

    QString tvaText = ui->tvaSpinBox->text();
    double tva = tvaText.toDouble();

    if (remise == 0) {
        remiseIsZero = true;
    }
    bool tvaIsZero = false;
    if (tva == 0) {
        tvaIsZero = true;
    }
    // Draw totals
    font.setBold(true);
    painter.setFont(font);
    y += 40;

    QStringList totalLabels = {"Total HTVA:", "Timbre Fiscal:", "Total TTC:"};
    QStringList totalValues = {ui->totalHTVALineEdit->text(), QString::number(ui->timbreFiscalSpinBox->value()), ui->totalTTCLineEdit->text()};

    if (!remiseIsZero) {
        totalLabels.insert(1, "Remise:  "+ ui->remiseSpinBox->text()+"%");
        totalValues.insert(1, ui->totalRemiseLineEdit->text());
    }
    if (!tvaIsZero) {
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
    updateAmounts();
    QString totalText = ui->totalTTCInWordsLabel->text() ;
    painter.drawText(10, y, totalText);
}


void Facture::updateAmounts()
{
    // Calculate total HTVA
    double totalHTVA = 0.0;
    for (int row = 0; row < ui->secondTable->rowCount(); row++) {
        QTableWidgetItem* quantityItem = ui->secondTable->item(row, 3);
        QTableWidgetItem* unitPriceItem = ui->secondTable->item(row, 4);

        if(quantityItem && unitPriceItem){
            double quantity = quantityItem->text().toDouble();
            double unitPrice = unitPriceItem->text().toDouble();
            double amount = quantity * unitPrice;
            totalHTVA += amount;

            // Update amount cell
            ui->secondTable->setItem(row, 5, new QTableWidgetItem(QString::number(amount, 'f', 2)));
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

QString Facture::numberToWords(int num) {
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
QString Facture::convertDinarsAndMillimes(int dinars, int millimes) {
    return numberToWords(dinars) + " dinars et "  + QString::number(millimes) + " millimes"  ;
}
void Facture::on_update_clicked()
{
    updateAmounts();
}


void Facture::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/path/to/images/", tr("Image Files (*.png *.jpg *.bmp)"));
    if(!fileName.isEmpty()) {
        selectedImageFileName = fileName;
    }
}

