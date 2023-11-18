#ifndef PREVIOUSFACTURE_H
#define PREVIOUSFACTURE_H
#include "databasehandler.h"
#include <QDialog>
#include <QWidget>

namespace Ui {
class PreviousFacture;
}

class PreviousFacture : public QWidget
{
    Q_OBJECT

public:
    explicit PreviousFacture(QWidget *parent = nullptr);
    ~PreviousFacture();

    QString getCompanyName() const;
    QString getCompanyId() const;


    QString getTel() const;
    QString getMatFisc() const;
    QString getAddress() const;

private slots:
    void on_buttonBox_accepted();
    void on_companyComboBox_currentIndexChanged(int index);


private:
    Ui::PreviousFacture *ui;
    DatabaseHandler m_db;
};

#endif // PREVIOUSFACTURE_H
