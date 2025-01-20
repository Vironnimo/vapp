#include "database.hpp"

#include <spdlog/spdlog.h>

namespace Vapp {

Database::Database(const std::string& dbFile) {
    spdlog::debug("Constructor Database");
    m_db = std::make_unique<SQLite::Database>(dbFile, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
}

Database::~Database() {
    spdlog::debug("Destructor Database");
}

SQLite::Statement Database::query(const std::string& queryString) {
    SQLite::Statement query(*m_db, queryString);
    return query;
}

bool Database::exec(const std::string& query) {
    if (query.empty()) {
        return false;
    }
    if (m_db->exec(query) > 0) {
        return true;
    }

    return false;
}

}  // namespace Vapp