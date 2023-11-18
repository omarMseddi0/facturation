#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_manageCompaniesButton_clicked();

    void on_manageUsersButton_clicked();

private:
    Ui::AdminWindow *ui;
};

#endif // ADMINWINDOW_H
