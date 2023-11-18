#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QWidget>

namespace Ui {
class UserWindow;
}

class UserWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();

private slots:
    void on_objectFacture_clicked();

    void on_tableauFacture_clicked();
    void on_FactureManage_clicked();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
