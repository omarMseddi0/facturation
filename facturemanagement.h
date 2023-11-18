#ifndef FACTUREMANAGEMENT_H
#define FACTUREMANAGEMENT_H
#include "databasehandler.h"
#include <QWidget>

namespace Ui {
class factureManagement;
}

class factureManagement : public QWidget
{
    Q_OBJECT

public:
    explicit factureManagement(QWidget *parent = nullptr);
    ~factureManagement();


private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_2_currentIndexChanged(int index);
    void on_deleteButton_clicked();

private:
    Ui::factureManagement *ui;
    DatabaseHandler db;
};

#endif // FACTUREMANAGEMENT_H
