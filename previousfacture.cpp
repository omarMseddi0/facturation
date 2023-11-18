#include "previousfacture.h"
#include "facture.h"
#include "qdatetime.h"
#include "ui_previousfacture.h"
#include "factureobject.h"

PreviousFacture::PreviousFacture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviousFacture)
{
    ui->setupUi(this);
    // Apply the style to the buttonBox
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

        ui->companyComboBox->setStyleSheet("QPushButton {"
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

    // Populate company combo box with companies from database
    m_db.openConnection();
    QList<Company> companies = m_db.getCompanies();
    for (const Company &company : companies) {
        ui->companyComboBox->addItem(company.getName(), company.getId());
    }


    // Update telephone number, matricule fiscale, and address fields when company is selected
    connect(ui->companyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_companyComboBox_currentIndexChanged(int)));
}

PreviousFacture::~PreviousFacture()
{
    delete ui;
}

QString PreviousFacture::getCompanyName() const
{
    return ui->companyComboBox->currentText();
}

QString PreviousFacture::getCompanyId() const
{
    return ui->companyComboBox->currentData().toString();
}




QString PreviousFacture::getTel() const
{
    return ui->telLineEdit->text();
}

QString PreviousFacture::getMatFisc() const
{
    return ui->matFiscLineEdit->text();
}

QString PreviousFacture::getAddress() const
{
    return ui->addressLineEdit->text();
}

void PreviousFacture::on_buttonBox_accepted()
{
    // Create a new instance of the Facture class
    Facture *facture = new Facture();
    // Set the values in the first table of the Facture UI
    facture->setCompanyName(getCompanyName());
    facture->setCompanyId(getCompanyId());


    facture->setTel(getTel());
    facture->setMatFisc(getMatFisc());
    facture->setaddressLineEdit(getAddress());

    // Show the Facture UI
    facture->show();
}

void PreviousFacture::on_companyComboBox_currentIndexChanged(int index)
{
    // Get selected company ID
    QString companyId = ui->companyComboBox->itemData(index).toString();

    // Get company information from database
    Company company = m_db.getCompanyById(companyId.toInt());

    // Update telephone number, matricule fiscale, and address fields with company information
    ui->telLineEdit->setText(company.getTel());
    ui->matFiscLineEdit->setText(company.getMat());
    ui->addressLineEdit->setText(company.getAdr());
}
