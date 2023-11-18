#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H
#include "databasehandler.h"
#include <QWidget>

namespace Ui {
class userManagement;
}

class userManagement : public QWidget
{
    Q_OBJECT

public:
    explicit userManagement(QWidget *parent = nullptr);
    ~userManagement();

private slots:


    void on_comboBox_activated(int index);
    void on_refreshBtn_clicked();
    void on_createBtn_clicked();
    void on_UpdateBtn_clicked();
    void on_deleteBtn_clicked();

private:
    Ui::userManagement *ui;
   DatabaseHandler dbHandler;

    void resetForm();
    void populateComboBox();
};

#endif // USERMANAGEMENT_H
