#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "RecordsCollection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RecordsCollection* currentCollection;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onComboboxChange();

    void addRecordFromForm();

    QString getFile(bool save=false);

    void loadFromFile();

    void saveToFile();

    void appendToCollection(QString lastName,
                            QString firstName,
                            QString middleName,
                            QString course,
                            QString date,
                            QString recordSign,
                            double recordValue);

    void showCollection();

    void addRecord(Record& record);

    void deleteRecord();

    bool check();

    void clearFields();

    void on_open_triggered();

    void on_save_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
