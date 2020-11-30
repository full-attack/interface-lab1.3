#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "iostream"
#include <QFileDialog>
#include <QMessageBox>
#include "stdexcept"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->currentCollection = new RecordsCollection();
    ui->setupUi(this);
    connect(ui->recordType, SIGNAL(currentTextChanged(QString)),
            this, SLOT(onComboboxChange()));
    connect(ui->addButton, SIGNAL(clicked()),
            this, SLOT(addRecordFromForm()));
    connect(ui->deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteRecord()));
    connect(ui->checkButton, SIGNAL(clicked()),
            this, SLOT(check()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onComboboxChange() {
    this->ui->branchSpin->setEnabled(!this->ui->branchSpin->isEnabled());
    this->ui->doubleVolumeBox->setEnabled(!this->ui->doubleVolumeBox->isEnabled());
}

void MainWindow::showCollection() {
    auto* list = this->ui->listRecords;
    list->clear();
    for (int index = 0; index < this->currentCollection->getLength(); index++) {
        Record* record = &(this->currentCollection->at(index));
        Student student = record->getStudent();
        QString lastName = QString::fromStdString(student.lastName);
        QString firstName = QString::fromStdString(student.firstName);
        QString middleName = QString::fromStdString(student.middleName);
        QString course = QString::fromStdString(student.course);
        QString date = QString::fromStdString(record->getDate());
        QString recordSign = nullptr;
        double recordValue = record->getRecordValue();
        if (record->identity() == "WateringRecord") {
            recordSign = QString{" Water: "};
        } else {
            recordSign = QString{" Branch: "};
        }
        QString qRecord{date + " " +
                       lastName + " " +
                       firstName + " " +
                       middleName + " " +
                       course +
                       recordSign +
                       QString::number(recordValue)};
        list->addItem(qRecord);
    }
}

void MainWindow::appendToCollection(QString lastName,
                        QString firstName,
                        QString middleName,
                        QString course,
                        QString date,
                        QString recordSign,
                        double recordValue) {
    std::string dateString = date.toStdString();
    std::string firstNameString = firstName.toStdString();
    std::string middleNameString = middleName.toStdString();
    std::string lastNameString = lastName.toStdString();
    std::string courseString = course.toStdString();
    Record* record = nullptr;
    if (recordSign == " Branch: ") {
        record = new BranchRecord{dateString,
                firstNameString, middleNameString,
                lastNameString, courseString, (int) recordValue};
    } else {
        record = new WateringRecord{dateString,
                firstNameString, middleNameString,
                lastNameString, courseString, (float) recordValue};
    }
    currentCollection->addRecord(*record);
}

void MainWindow::clearFields() {
    this->ui->lastNameLine->clear();
    this->ui->firstNameLine->clear();
    this->ui->middleNameLine->clear();
    this->ui->courseLine->clear();
}

void MainWindow::addRecordFromForm() {
    QString lastName = this->ui->lastNameLine->text();
    QString firstName = this->ui->firstNameLine->text();
    QString middleName = this->ui->middleNameLine->text();
    QString course = this->ui->courseLine->text();
    auto date = this->ui->dateEdit->date();
    QString dateFormatted = QString::number(date.day()) + "." +
                            QString::number(date.month()) + "." +
                            QString::number(date.year());
    QString record{dateFormatted + " " +
                   lastName + " " +
                   firstName + " " +
                   middleName + " " +
                   course};
    double recordValue = 0;
    QString recordSign = nullptr;
    if (this->ui->branchSpin->isEnabled()) {
        recordValue = this->ui->branchSpin->value();
        recordSign = QString{" Branch: "};
        record += recordSign;
    } else {
        recordValue = this->ui->doubleVolumeBox->value();
        recordSign = QString{" Water: "};
        record += recordSign;
    }
    record += QString::number(recordValue);
    try {
        appendToCollection(lastName, firstName, middleName, course, dateFormatted, recordSign, recordValue);
        auto* list = this->ui->listRecords;
        list->addItem(record);
    }  catch (...) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","An error has occured!");
        messageBox.setFixedSize(500,200);
    }
    clearFields();
}

void MainWindow::addRecord(Record& record) {
    this->currentCollection->addRecord(record);
}

QString MainWindow::getFile(bool save) {
    QString fileName = nullptr;
    if (save){
        fileName = QFileDialog::getSaveFileName(this,
                tr("Save Records"), "",
                tr("Records (*.txt);;All Files (*)"));
    } else {
        fileName = QFileDialog::getOpenFileName(this,
                tr("Load Records"), "",
                tr("Records (*.txt);;All Files (*)"));
    }
    return fileName;
}

void MainWindow::deleteRecord() {
    auto* list = this->ui->listRecords;
    int index = list->currentIndex().row();
    if (index >= 0 && index < this->currentCollection->getLength()){
        this->currentCollection->deleteRecord(index);
        showCollection();
    }
}

bool MainWindow::check() {
    bool chrono = this->currentCollection->checkChronologicalOrder();
    bool grooming = this->currentCollection->checkGrooming();
    if (!chrono) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Chronological error!");
        messageBox.setFixedSize(500,200);
    }
    if (!grooming) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Grooming error!");
        messageBox.setFixedSize(500,200);
    }

    return chrono && grooming;
}

void MainWindow::loadFromFile() {
    QString filePath = getFile();
    RecordsCollection collection{};
    collection.loadFromFile(filePath.toStdString());
    if (currentCollection != nullptr) {
        currentCollection->~RecordsCollection();
        currentCollection = nullptr;
    }
    this->currentCollection = new RecordsCollection{collection};
    this->showCollection();
}

void MainWindow::saveToFile() {
    QString filePath = getFile(true);
    this->currentCollection->writeToFile(filePath.toStdString());
}

void MainWindow::on_open_triggered()
{
    loadFromFile();
}

void MainWindow::on_save_triggered()
{
   if (check()) {
       saveToFile();
   }
}
