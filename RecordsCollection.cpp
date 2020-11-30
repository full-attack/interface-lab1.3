#include "RecordsCollection.h"
#include "fstream"
#include "cassert"
#include <sstream>

RecordsCollection::RecordsCollection() {
    length = 0;
    collection = new Record *[size];
}

void RecordsCollection::addRecord(Record &record) {
    if (length == size) {
        size += 10;
        auto **new_collection = new Record *[size];
        for (int index = 0; index < length; index++) {
            new_collection[index] = collection[index];
        }
        delete[] collection;
        collection = new_collection;
    }
    collection[length] = &record;
    length++;
}

Record &RecordsCollection::at(int pos) const {
    assert(pos < length);
    return *collection[pos];
}

int RecordsCollection::getLength() const {
    return length;
}

RecordsCollection::~RecordsCollection() {
    delete[] collection;
}

void RecordsCollection::deleteRecord(int pos) {
    assert(pos >= 0 && pos < length);
    if (pos != length - 1) {
        for (int i = pos; i < length - 1; i++) {
            collection[i] = collection[i + 1];
        }
    }
    length--;
}

RecordsCollection::RecordsCollection(const RecordsCollection &source) {
    length = source.length;
    collection = new Record *[source.size];
    for (int index = 0; index < source.length; index++) {
        collection[index] = source.collection[index];
    }
}

void RecordsCollection::erase() {
    size = 10;
    length = 0;
    delete[] collection;
    collection = new Record *[size];
}

bool RecordsCollection::checkChronologicalOrder() const {
    bool isCorrect = true;
    for (const std::string& component : (std::string[]){"WateringRecord", "BranchRecord"}) {
        int index = 0 ;
        int prev_date[3] = {0, 0, 0};
        while (index < length && isCorrect) {
            if (collection[index]->identity() != component) {
                index++;
                continue;
            }
            std::string stringDate = collection[index]->getDate();
            int day = stoi(stringDate.substr(0,
                                             stringDate.find('.')));
            int month = stoi(stringDate.substr(stringDate.find('.') + 1,
                                               stringDate.rfind('.')));
            int year = stoi(stringDate.substr(stringDate.rfind('.') + 1,
                                              stringDate.length()));
            if (prev_date[0] != 0 && prev_date[1] != 0 && prev_date[2] != 0) {
                if (prev_date[2] != year) {
                    isCorrect &= prev_date[2] < year;
                    isCorrect &= year - prev_date[2] == 1;
                } else if (prev_date[1] != month) {
                    isCorrect &= prev_date[1] < month;
                    isCorrect &= month - prev_date[1] == 1;
                    if (prev_date[2] % 4 == 0 &&
                        (prev_date[2] % 100 != 0 || prev_date[2] % 400 == 0)) {
                        if (prev_date[1] == 2) {
                            isCorrect &= prev_date[0] == 29;
                        }
                    }
                    if (prev_date[1] >= 1 && prev_date[1] <= 7) {
                        if (prev_date[1] % 2 != 0) {
                            isCorrect &= prev_date[0] == 31;
                        } else {
                            isCorrect &= prev_date[0] == 30;
                        }
                    } else {
                        if (month % 2 != 0) {
                            isCorrect &= prev_date[0] == 30;
                        } else {
                            isCorrect &= prev_date[0] == 31;
                        }
                    }
                } else {
                    isCorrect &= prev_date[0] < day;
                    isCorrect &= day - prev_date[0] == 1;
                }
            }
            prev_date[2] = year;
            prev_date[1] = month;
            prev_date[0] = day;
            index++;
        }
        if (!isCorrect){
            return isCorrect;
        }
    }
    return isCorrect;
}

bool RecordsCollection::checkGrooming() const {
    int days = 0;
    int weekBranches = 0;
    bool isCorrect = true;
    for (int index = 0; index < length && isCorrect; index++) {
        days++;
        if (collection[index]->identity() == "BranchRecord") {
            weekBranches += collection[index]->getRecordValue();
        }
        if (days == 7) {
            assert(weekBranches >= 4);
            assert(weekBranches <= 8);
            days = 0;
            weekBranches = 0;
        }
        if (collection[index]->identity() == "WateringRecord") {
            isCorrect = collection[index]->getRecordValue() >= 1 &&
                   collection[index]->getRecordValue() <= 2;
        }

    }
    return isCorrect;
}

bool RecordsCollection::compare(const RecordsCollection &toCompare) const {
    bool isEqual = true;
    isEqual &= (this->length == toCompare.length);
    for (int i = 0; i < this->length && isEqual; i++) {
        Record **compareCollection = toCompare.collection;
        isEqual &= this->collection[i]->compare(*compareCollection[i]);
    }
    return isEqual;
}

Record* RecordsCollection::parseRecord(const std::string line) const {
    auto offset = 0;
    int index = stoi(line.substr(offset, line.find(' ')));
    offset += line.find(' ') + 1;
    std::string date = line.substr(offset,
                                   line.find(' ', offset) - offset);
    offset += date.length() + 1;
    std::string lastName = line.substr(offset,
                                       line.find(' ', offset) - offset);
    offset += lastName.length() + 1;
    std::string firstName = line.substr(offset,
                                        line.find(' ', offset) - offset);
    offset += firstName.length() + 1;
    std::string middleName = line.substr(offset,
                                         line.find(' ', offset) - offset);
    offset += middleName.length() + 1;
    std::string course = line.substr(offset,
                                     line.find(' ', offset) - offset);
    offset += course.length() + 1;
    std::string recordValueName = line.substr(offset,
                                              line.find(' ', offset) - offset);
    Record *record = nullptr;
    if (recordValueName == "Water:") {
        float recordValue = stof(line.substr(line.rfind(' ') + 1,
                                             line.length()));
        record = new WateringRecord{date, firstName,
                                    middleName, lastName,
                                    course, recordValue};
    } else if ((recordValueName == "Branch:")) {
        int recordValue = stoi(line.substr(line.rfind(' ') + 1,
                                           line.length()));
        record = new BranchRecord{date, firstName,
                                  middleName, lastName,
                                  course, (int) recordValue};
    }
    return record;
}

void RecordsCollection::loadFromFile(const std::string &filePath) {
    this->erase();
    std::fstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        Record* record = parseRecord(line);
        this->addRecord(*record);
    }
    file.close();
}

void RecordsCollection::writeToFile(const std::string &filePath) const {
    std::fstream file;
    file.open(filePath, std::fstream::out);
    for (int i = 0; i < this->length; i++) {
        file << i + 1 << " ";
        file << this->collection[i]->getDate() << " ";
        file << this->collection[i]->getStudent().lastName << " ";
        file << this->collection[i]->getStudent().firstName << " ";
        file << this->collection[i]->getStudent().middleName << " ";
        file << this->collection[i]->getStudent().course << " ";
        file << "Water: " << this->collection[i]->getRecordValue() << "\n";
    }
    file.close();
}

void RecordsCollection::setAt(const Record &record, int pos) {
    assert(pos >= 0);
    assert(pos < length);
    *collection[pos] = record;
}

void RecordsCollection::insertAt(const Record &record, int pos) {
    assert(pos >= 0);
    assert(pos < length);
    if (this->length == this->size) {
        this->size += 10;
        auto *new_collection = new Record *[this->size];
        for (int index = 0; index < this->length; index++) {
            new_collection[index] = this->collection[index];
        }
        delete[] this->collection;
        this->collection = new_collection;
    }
    for (int i = length - 1; i >= pos; i--) {
        this->collection[i + 1] = this->collection[i];
    }
    *collection[pos] = record;
    this->length++;
}



