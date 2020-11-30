#include "BranchRecord.h"

class RecordsCollection {
private:
    int length;
    int size = 10;
    Record **collection;

    Record* parseRecord(const std::string line) const;

public:
    RecordsCollection();

    RecordsCollection(const RecordsCollection &source);

    ~RecordsCollection();

    void addRecord(Record &record);

    void setAt(const Record &record, int pos);

    void insertAt(const Record &record, int pos);

    void deleteRecord(int pos);

    void erase();

    Record &at(int pos) const;

    int getLength() const;

    bool checkChronologicalOrder() const;

    bool checkGrooming() const;

    bool compare(const RecordsCollection &toCompare) const;

    void loadFromFile(const std::string &filePath);

    void writeToFile(const std::string &filePath) const;
};

