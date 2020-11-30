#include "Record.h"
#include <cassert>
#include "regex"

Student Record::getStudent() const { return this->student; }

std::string Record::getDate() const { return this->date; }

void Record::setStudent(const Student &studentGiven) {
    validateStudent(studentGiven);
    this->student.firstName = studentGiven.firstName;
    this->student.middleName = studentGiven.middleName;
    this->student.lastName = studentGiven.lastName;
    this->student.course = studentGiven.course;
}

void Record::setDate(const std::string &dateGiven) {
    validateDate(dateGiven);
    this->date = dateGiven;
}

void Record::validateStudent(const Student &studentGiven) {
    assert(!studentGiven.firstName.empty());
    assert(!studentGiven.middleName.empty());
    assert(!studentGiven.lastName.empty());
    assert(!studentGiven.course.empty());
}


void Record::validateDate(const std::string &dateGiven) {
    bool isMatched = std::regex_match(
            dateGiven,
            std::regex("(\\d{1}|\\d{2})\\.{1}(\\d{1}|\\d{2})\\.{1}\\d{4}"));
    assert(isMatched);
    int days = stoi(dateGiven.substr(0, dateGiven.find('.')));
    int month = stoi(dateGiven.substr(dateGiven.find('.') + 1,
                                      dateGiven.rfind('.')));
    int year = stoi(dateGiven.substr(dateGiven.rfind('.') + 1,
                                     dateGiven.length()));
    assert(year >= 1970 && year < 3000);
    assert(month >= 1 && month <= 12);
    assert(days >= 1);
    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
        if (month == 2) {
            assert(days <= 29);
        }
    }
    if (month >= 1 && month <= 7) {
        if (month % 2 != 0) {
            assert(days <= 31);
        } else {
            assert(days <= 30);
        }
    } else {
        if (month % 2 != 0) {
            assert(days <= 30);
        } else {
            assert(days <= 31);
        }
    }
}

bool Record::compare(Record &toCompare) const {
    return (this->getDate() ==
            toCompare.getDate()) &&
           (this->getStudent().lastName ==
            toCompare.getStudent().lastName) &&
           (this->getStudent().firstName ==
            toCompare.getStudent().firstName) &&
           (this->getStudent().middleName ==
            toCompare.getStudent().middleName);
}

Record::Record() {
    this->date = std::string{""};
    this->student = Student{"", "", "", ""};
}

Record::Record(std::string &dateGiven, std::string &firstNameGiven, std::string &middleNameGiven,
               std::string &lastNameGiven, std::string &courseGiven) :
        date(dateGiven),
        student(Student{firstNameGiven, middleNameGiven, lastNameGiven, courseGiven}) {
    validateStudent(this->student);
    validateDate(this->date);
}

Record::Record(const Record &recordSource) :
        date(recordSource.date),
        student(recordSource.student) {

}

void Record::validateRecordValue(float RecordValueGiven) {
    assert(RecordValueGiven >= 0);
}
