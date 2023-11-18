#ifndef PREVIOUSFACTUREOBJECT_H
#define PREVIOUSFACTUREOBJECT_H
#include "databasehandler.h"
#include <QDialog>
#include <QWidget>
#include <QWidget>

namespace Ui {
class previousFactureObject;
}

class previousFactureObject : public QWidget
{
    Q_OBJECT

public:
    explicit previousFactureObject(QWidget *parent = nullptr);
    ~previousFactureObject();
    QString getCompanyName() const;
    QString getCompanyId() const;


    QString getTel() const;
    QString getMatFisc() const;
    QString getAddress() const;

private slots:
    void on_buttonBox_accepted();
    void on_companyComboBox_currentIndexChanged(int index);
private:
    Ui::previousFactureObject *ui;
    DatabaseHandler m_db;
};

#endif // PREVIOUSFACTUREOBJECT_H
