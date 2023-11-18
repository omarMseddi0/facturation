#ifndef FACTUREOBJECT_H
#define FACTUREOBJECT_H
#include <QWidget>
#include <QWidget>
#include "databasehandler.h"
#include "previousfacture.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QDate>
#include <QWidget>
#include "databasehandler.h"
#include <QInputDialog>
#include <QPrinter>
#include <QPainter>
#include <QSqlDatabase>
#include <QWidget>

namespace Ui {
class factureObject;
}

class factureObject : public QWidget
{
    Q_OBJECT

public:
    explicit factureObject(QWidget *parent = nullptr);
    ~factureObject();

    void setCompanyName(const QString &name);
    void setCompanyId(const QString &id);


    void setTel(const QString &tel);
    void setMatFisc(const QString &MatFisc);
    void setaddressLineEdit(const QString &address);
private slots:
    void on_addLineButton_clicked();
    void on_finishedButton_clicked();
    void on_update_clicked();
    void on_browseButton_clicked();

private:
    Ui::factureObject *ui;
    void updateAmounts();
    QString numberToWords(int num);
    QString convertDinarsAndMillimes(int dinars, int millimes);
    DatabaseHandler db ;
    QString selectedImageFileName;

};

#endif // FACTUREOBJECT_H
