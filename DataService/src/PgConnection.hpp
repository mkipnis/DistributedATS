/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>

#include <quickfix/DatabaseConnectionID.h>

#include <pqxx/pqxx>
#include <iostream>

namespace DistributedATS {
    class PostgresQuery {
    public:
        explicit PostgresQuery(const std::string &query)
            : m_query(query) {
        }
        bool execute(pqxx::connection *conn) {
            try {
                pqxx::work txn(*conn);
                pqxx::result result = txn.exec(m_query);
                int cols = result.columns();

                for (auto row: result) {
                    std::vector<std::string> values(cols);

                    for (int i = 0; i < cols; ++i) {
                        values[i] = row[i].c_str();
                    }

                    m_rows.emplace_back(values);
                }

                txn.commit();
                return true;
            } catch (const std::exception &e) {
                m_reason = e.what();
                return false;
            }
        }

        unsigned long rows() {
            return m_rows.size();
        }

        std::string getValue(int row, int column) {
            return m_rows[row][column];
        }

        std::string getQuery()
        {
            return m_query;
        }

    private:
        std::string m_query;
        std::string m_reason;
        std::vector<std::vector<std::string> > m_rows;
    };

    class PostgresConnection {
    public:
        PostgresConnection(const FIX::DatabaseConnectionID &database)
            : m_database(database) {
            connect();
        }

        bool execute(PostgresQuery &query) {
            return query.execute(m_connection);
        }

        bool connected() {
            return m_connection != nullptr;
        }

    private:
        void connect() {
            LOG4CXX_INFO(logger, "Attempting to connect");
            try {
                std::string conn_str =
                        "dbname=" + m_database.getDatabase() +
                        " user=postgres password=postgres host=localhost port=5430";

                m_connection = new pqxx::connection(conn_str);
            } catch (const std::exception &e) {
                std::cerr << "PostgreSQL connection failed: "
                        << e.what() << std::endl;
                LOG4CXX_ERROR(logger, std::string("Postgres connection failed: ") + e.what());
            }
        }

        pqxx::connection *m_connection = nullptr;
        FIX::DatabaseConnectionID m_database;
    };
}
