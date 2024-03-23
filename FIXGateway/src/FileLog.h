/*
   Copyright (C) 2021 Mike Kipnis

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

#include <fstream>
#include <quickfix/Log.h>
#include <quickfix/SessionSettings.h>

namespace DistributedATS {
/**
 * Creates a file based implementation of Log
 *
 * This stores all log events into flat files
 */
class FileLogFactory : public FIX::LogFactory {
public:
    FileLogFactory(const FIX::SessionSettings &settings, std::string fix_prefix)
    : m_settings(settings), m_globalLog(0), m_globalLogCount(0),
    m_fix_prefix(fix_prefix){};
    FileLogFactory(const std::string &path)
    : m_path(path), m_backupPath(path), m_globalLog(0), m_globalLogCount(0){};
    FileLogFactory(const std::string &path, const std::string &backupPath)
    : m_path(path), m_backupPath(backupPath), m_globalLog(0),
    m_globalLogCount(0){};
    
public:
    FIX::Log *create();
    FIX::Log *create(const FIX::SessionID &);
    void destroy(FIX::Log *log);
    
private:
    std::string m_path;
    std::string m_backupPath;
    FIX::SessionSettings m_settings;
    FIX::Log *m_globalLog;
    int m_globalLogCount;
    
    std::string m_fix_prefix;
};

/**
 * File based implementation of Log
 *
 * Two files are created by this implementation.  One for messages,
 * and one for events.
 *
 */
class FileLog : public FIX::Log {
public:
    FileLog(const std::string &path, const std::string &);
    FileLog(const std::string &path, const std::string &backupPath,
            const std::string &);
    FileLog(const std::string &path, const FIX::SessionID &sessionID,
            const std::string &);
    FileLog(const std::string &path, const std::string &backupPath,
            const FIX::SessionID &sessionID, const std::string &);
    virtual ~FileLog();
    
    void clear();
    void backup();
    
    void onIncoming(const std::string &value) {
        m_messages << FIX::UtcTimeStampConvertor::convert(FIX::UtcTimeStamp(), 9)
        << " : " << value << std::endl;
    }
    void onOutgoing(const std::string &value) {
        m_messages << FIX::UtcTimeStampConvertor::convert(FIX::UtcTimeStamp(), 9)
        << " : " << value << std::endl;
    }
    void onEvent(const std::string &value) {
        FIX::UtcTimeStamp now;
        m_event << FIX::UtcTimeStampConvertor::convert(now, 9) << " : " << value
        << std::endl;
    }
    
private:
    std::string generatePrefix(const std::string &filePrefix,
                               const FIX::SessionID &sessionID);
    void init(std::string path, std::string backupPath,
              const std::string &prefix);
    
    std::ofstream m_messages;
    std::ofstream m_event;
    std::string m_messagesFileName;
    std::string m_eventFileName;
    std::string m_fullPrefix;
    std::string m_fullBackupPrefix;
};

} // namespace DistributedATS
