#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "vapp/core/database.hpp"

class DatabaseTest : public ::testing::Test {
   protected:
    std::string dbFile = ":memory:";  // in-memory sqlite is actually a thing!
    Vapp::Database* db;

    void SetUp() override {
        db = new Vapp::Database(dbFile);
    }

    void TearDown() override {
        delete db;
    }
};

TEST_F(DatabaseTest, Constructor) {
    EXPECT_NO_THROW(Vapp::Database db(dbFile));
}

TEST_F(DatabaseTest, Query) {
    db->exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT);");
    db->exec("INSERT INTO test (value) VALUES ('test_value');");

    auto query = db->query("SELECT value FROM test WHERE id = 1;");
    query.executeStep();
    EXPECT_EQ(query.getColumn(0).getText(), std::string("test_value"));
}

TEST_F(DatabaseTest, Exec) {
    EXPECT_NO_THROW(db->exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT);"));
    EXPECT_NO_THROW(db->exec("INSERT INTO test (value) VALUES ('test_value');"));
}

TEST_F(DatabaseTest, QueryNonExistentTable) {
    EXPECT_ANY_THROW(db->query("SELECT * FROM non_existent_table;"));
}

TEST_F(DatabaseTest, ExecInvalidQuery) {
    EXPECT_ANY_THROW(db->exec("INVALID SQL QUERY"));
}

TEST_F(DatabaseTest, QueryWithoutExec) {
    EXPECT_ANY_THROW(db->query("SELECT * FROM test;"));
}

// TEST_F(DatabaseTest, ExecWithNullQuery) {
//     EXPECT_FALSE(db->exec(nullptr));
// }