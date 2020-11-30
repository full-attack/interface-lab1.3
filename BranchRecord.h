#include "WateringRecord.h"

class BranchRecord : public Record {
private:
    int branches;

public:
    BranchRecord();

    BranchRecord(std::string &dateGiven,
                 std::string &firstNameGiven,
                 std::string &middleNameGiven,
                 std::string &lastNameGiven,
                 std::string &courseGiven,
                 int branchesGiven);

    BranchRecord(const BranchRecord &branchRecordSource);

    virtual bool compare(Record &toCompare) const;

    virtual float getRecordValue() const;

    virtual void setRecordValue(float branchesGiven);

    virtual std::string identity() const;
};
