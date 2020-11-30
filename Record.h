#include "ctime"
#include "string"

struct Student {
    std::string firstName;
    std::string middleName;
    std::string lastName;
    std::string course;
};

class Record {

private:
    std::string date;
    Student student;

protected:
    static void validateStudent(const Student &studentGiven);

    static void validateDate(const std::string &dateGiven);

    static void validateRecordValue(float RecordValueGiven);

public:
    Record();

    Record(std::string &dateGiven,
           std::string &firstNameGiven,
           std::string &middleNameGiven,
           std::string &lastNameGiven,
           std::string &courseGiven);

    Record(const Record &recordSource);

    std::string getDate() const;

    Student getStudent() const;

    void setDate(const std::string &dateGiven);

    void setStudent(const Student &studentGiven);

    virtual bool compare(Record &toCompare) const = 0;

    virtual float getRecordValue() const = 0;

    virtual void setRecordValue(float recordValue) = 0;

    virtual std::string identity() const = 0;
};

