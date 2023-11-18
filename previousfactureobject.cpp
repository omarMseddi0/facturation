#include "previousfactureobject.h"
#include "ui_previousfactureobject.h"

#include "factureobject.h"
#include "qdatetime.h"


previousFactureObject::previousFactureObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::previousFactureObject)
{
    ui->setupUi(this);
    m_db.openConnection();
    QList<Company> companies = m_db.getCompanies();
    for (const Company &company : companies) {
        ui->companyComboBox->addItem(company.getName(), company.getId());
    }


    // Update telephone number, matricule fiscale, and address fields when company is selected
    connect(ui->companyComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_companyComboBox_currentIndexChanged(int)));
}

previousFactureObject::~previousFactureObject()
{
    delete ui;
}

QString previousFactureObject::getCompanyName() const
{
    return ui->companyComboBox->currentText();
}

QString previousFactureObject::getCompanyId() const
{
    return ui->companyComboBox->currentData().toString();
}




QString previousFactureObject::getTel() const
{
    return ui->telLineEdit->text();
}

QString previousFactureObject::getMatFisc() const
{
    return ui->matFiscLineEdit->text();
}

QString previousFactureObject::getAddress() const
{
    return ui->addressLineEdit->text();
}

void previousFactureObject::on_buttonBox_accepted()
{
    // Create a new instance of the Facture class
    factureObject *facturee = new factureObject();

    // Set the values in the first table of the Facture UI
    facturee->setCompanyName(getCompanyName());
    facturee->setCompanyId(getCompanyId());


    facturee->setTel(getTel());
    facturee->setMatFisc(getMatFisc());
    facturee->setaddressLineEdit(getAddress());

    // Show the Facture UI
    facturee->show();
}

void previousFactureObject::on_companyComboBox_currentIndexChanged(int index)
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
