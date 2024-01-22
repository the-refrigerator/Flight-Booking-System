#ifndef MODEL_H
#define MODEL_H

#include "DataBaseConnection.h"

class Model {
public:
    virtual void save() = 0;
private:
    DataBaseConnection getConnection() {
        return DataBaseConnection::getInstance();
    }
};

#endif // !MODEL_H
