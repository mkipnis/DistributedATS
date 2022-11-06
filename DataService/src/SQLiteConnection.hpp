/*
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over OpenDDS. This project simplifies
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

#ifndef SQLiteConnection_hpp
#define SQLiteConnection_hpp

#include <sqlite3.h>
#include <string>
#include <vector>
#include <iostream>

#include <quickfix/DatabaseConnectionID.h>

namespace DistributedATS
{
class SQLiteQuery
{
    
public:
    SQLiteQuery( const std::string& query ) : m_stmt(0), m_query( query )
  {
      
      std::cout << query << std::endl;
  }

  ~SQLiteQuery()
  {
    sqlite3_finalize(m_stmt);
  }

  bool execute( sqlite3* pConnection )
  {
      if(sqlite3_prepare_v2(pConnection, m_query.c_str(), -1, &m_stmt, NULL) != SQLITE_OK) {
          printf("FATAL ERROR: while compiling sql: %s\n", sqlite3_errmsg(pConnection));
          sqlite3_close(pConnection);
          sqlite3_finalize(m_stmt);
          //return false;
          exit(-1);
      }
      
      int number_of_rows = sqlite3_column_count(m_stmt);
      
      while((m_status = sqlite3_step(m_stmt)) == SQLITE_ROW)
      {
          std::vector<std::string> row(number_of_rows);
          for ( int row_num = 0; row_num<number_of_rows; ++row_num )
          {
              row[row_num] = (char*)( sqlite3_column_text(m_stmt, row_num) );
          }
          m_rows.emplace_back(row);
      }
            
    return success();
  }

  bool success()
  {
    return m_status == 0;
  }

  unsigned long rows()
  {
      return m_rows.size();
  }

  const std::string reason()
  {
    return m_reason;
  }

  std::string getValue( int row, int column )
  {
      return m_rows[row][column];
  }

private:
  sqlite3_stmt* m_stmt;
  int m_status;
  std::string m_query;
  std::string m_reason;
  std::vector< std::vector< std::string > > m_rows;
};

class SQLiteConnection
{
public:
    SQLiteConnection( const FIX::DatabaseConnectionID & database )
  : m_pDatabase( database ), m_pConnection(NULL)
  {
    connect();
  }

  ~SQLiteConnection()
  {
    if( m_pConnection )
        sqlite3_close( m_pConnection );
  }

  bool execute( SQLiteQuery& pQuery )
  {
    return pQuery.execute( m_pConnection );
  }
    
  bool connected()
  {
      return ( m_pConnection != NULL );
  }

private:
  void connect()
  {
      int rc = sqlite3_open(m_pDatabase.getDatabase().c_str(), &m_pConnection);
      
      if (rc != SQLITE_OK)
      {
          fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(m_pConnection));
          sqlite3_close(m_pConnection);
      }
  }

    sqlite3* m_pConnection;
    FIX::DatabaseConnectionID m_pDatabase;
};

}

#endif /* SQLiteConnection_hpp */
