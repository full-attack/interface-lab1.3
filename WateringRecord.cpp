#include "WateringRecord.h"
#include <cassert>

WateringRecord::WateringRecord() : Record() {
    this->wateringVolume = 0;
}

WateringRecord::WateringRecord(
        std::string &dateGiven,
        std::string &firstNameGiven,
        std::string &middleNameGiven,
        std::string &lastNameGiven,
        std::string &courseGiven, float wateringVolumeGiven) :
        Record(dateGiven, firstNameGiven, middleNameGiven, lastNameGiven, courseGiven),
        wateringVolume(wateringVolumeGiven) {
    validateRecordValue(this->wateringVolume);
}

WateringRecord::WateringRecord(const WateringRecord &wateringRecordSource) :
        Record(wateringRecordSource),
        wateringVolume(wateringRecordSource.wateringVolume) {

}

float WateringRecord::getRecordValue() const {
    return this->wateringVolume;
}

void WateringRecord::setRecordValue(float wateringVolumeGiven) {
    Record::validateRecordValue(wateringVolumeGiven);
    this->wateringVolume = wateringVolumeGiven;
}

bool WateringRecord::compare(Record &toCompare) const {
    return (this->getRecordValue() ==
            toCompare.getRecordValue()) &&
           (Record::compare(toCompare));
}

std::string WateringRecord::identity() const {
    return "WateringRecord";
}









