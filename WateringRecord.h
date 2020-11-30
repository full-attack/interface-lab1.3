#include "Record.h"

class WateringRecord : public Record {
private:
    float wateringVolume;


public:
    WateringRecord();

    WateringRecord(std::string &dateGiven,
                   std::string &firstNameGiven,
                   std::string &middleNameGiven,
                   std::string &lastNameGiven,
                   std::string &courseGiven,
                   float wateringVolumeGiven);

    WateringRecord(const WateringRecord &wateringRecordSource);

    virtual bool compare(Record &toCompare) const;

    virtual float getRecordValue() const;

    virtual void setRecordValue(float wateringVolumeGiven);

    virtual std::string identity() const;
};
