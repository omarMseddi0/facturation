

#include <QStringList>

#define FACTURE_H

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

namespace Ui {
class Facture;
}

class Facture : public  QDialog
{
    Q_OBJECT

public:
    explicit Facture(QWidget *parent = nullptr);
    ~Facture();

    void setCompanyName(const QString &name);
    void setCompanyId(const QString &id);


    void setTel(const QString &tel);
    void setMatFisc(const QString &MatFisc);
    void setaddressLineEdit(const QString &address);


private slots:
    void on_addObjectButton_clicked();
    void on_addLineButton_clicked();
    void on_finishedButton_clicked();
    void on_browseButton_clicked();
    void on_update_clicked();



private:
    Ui::Facture *ui;
    void updateAmounts();
    QString numberToWords(int num);
    QString convertDinarsAndMillimes(int dinars, int millimes);
    QString formatText(const QString &text, int charLimit);
    DatabaseHandler db;
    QString selectedImageFileName;
    double millimes ;

};
