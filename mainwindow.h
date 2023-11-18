#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databasehandler.h"
#include <QMainWindow>
#include <QComboBox>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_comboBox_activated(int index);

    void on_refreshBtn_clicked();

    void on_createBtn_clicked();

    void on_UpdateBtn_clicked();

    void on_deleteBtn_clicked();

private:
    Ui::MainWindow *ui;
    DatabaseHandler dbHandler;
    void generateConfigFile();
    void populateComboBox();
    void resetForm();

};
#endif // MAINWINDOW_H
