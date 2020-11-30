#include "BranchRecord.h"

BranchRecord::BranchRecord() : Record() {
    this->branches = 0;
}

BranchRecord::BranchRecord(std::string &dateGiven, std::string &firstNameGiven, std::string &middleNameGiven,
                           std::string &lastNameGiven, std::string &courseGiven, int branchesGiven) :
        Record(dateGiven, firstNameGiven, middleNameGiven, lastNameGiven, courseGiven),
        branches(branchesGiven) {

}

BranchRecord::BranchRecord(const BranchRecord &branchRecordSource) :
        Record(branchRecordSource), branches(branchRecordSource.branches) {

}

bool BranchRecord::compare(Record &toCompare) const {
    return (this->getRecordValue() == toCompare.getRecordValue()) &&
           (Record::compare(toCompare));
}

float BranchRecord::getRecordValue() const {
    return branches;
}

void BranchRecord::setRecordValue(float branchesGiven) {
    Record::validateRecordValue(branchesGiven);
    this->branches = branchesGiven;
}

std::string BranchRecord::identity() const {
    return "BranchRecord";
}
