#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <memory>

namespace Vapp {

class Database {
   public:
    Database(const std::string& dbFile);
    ~Database();

    SQLite::Statement query(const std::string& query);
    bool exec(const std::string& query);

   private:
   std::unique_ptr<SQLite::Database> m_db;
};

}  // namespace Vapp