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

#include "FileLog.h"

using namespace FIX;

namespace DistributedATS {

FIX::Log *FileLogFactory::create() {
  m_globalLogCount++;
  if (m_globalLogCount > 1)
    return m_globalLog;

  try {
    if (m_path.size())
      return new FileLog(m_path, m_fix_prefix);
    std::string path;
    std::string backupPath;

    FIX::Dictionary settings = m_settings.get();
    path = settings.getString(FILE_LOG_PATH);
    backupPath = path;
    if (settings.has(FILE_LOG_BACKUP_PATH))
      backupPath = settings.getString(FILE_LOG_BACKUP_PATH);

    return m_globalLog =
               new DistributedATS::FileLog(path, backupPath, m_fix_prefix);
  } catch (ConfigError &) {
    m_globalLogCount--;
    throw;
  }
}

Log *FileLogFactory::create(const FIX::SessionID &s) {
  if (m_path.size() && m_backupPath.size())
    return new DistributedATS::FileLog(m_path, m_backupPath, s, m_fix_prefix);
  if (m_path.size())
    return new DistributedATS::FileLog(m_path, s, m_fix_prefix);

  std::string path;
  std::string backupPath;
  FIX::SessionID defaultSession("FIX.4.4", "DEFAULT", "DEFAULT");
  Dictionary settings = m_settings.get(defaultSession);
  path = settings.getString(FILE_LOG_PATH);
  backupPath = path;
  if (settings.has(FILE_LOG_BACKUP_PATH))
    backupPath = settings.getString(FILE_LOG_BACKUP_PATH);

  return new DistributedATS::FileLog(path, backupPath, s, m_fix_prefix);
}

void FileLogFactory::destroy(FIX::Log *pLog) {
  if (pLog == m_globalLog) {
    m_globalLogCount--;
    if (m_globalLogCount == 0) {
      delete pLog;
      m_globalLogCount = 0;
    }
  } else {
    delete pLog;
  }
}

FileLog::FileLog(const std::string &path, const std::string &prefix) {
  init(path, path, prefix + "-GLOBAL");
}

FileLog::FileLog(const std::string &path, const std::string &backupPath,
                 const std::string &prefix) {
  init(path, backupPath, prefix + "-GLOBAL");
}

FileLog::FileLog(const std::string &path, const FIX::SessionID &s,
                 const std::string &prefix) {
  init(path, path, generatePrefix(prefix, s));
}

FileLog::FileLog(const std::string &path, const std::string &backupPath,
                 const FIX::SessionID &s, const std::string &prefix) {
  init(path, backupPath, generatePrefix(prefix, s));
}

std::string FileLog::generatePrefix(const std::string &filePrefix,
                                    const SessionID &s) {
  const std::string &begin = s.getBeginString().getString();
  const std::string &sender = s.getSenderCompID().getString();
  const std::string &target = s.getTargetCompID().getString();
  const std::string &qualifier = s.getSessionQualifier();

  std::string prefix = filePrefix + "-" + begin + "-" + sender + "-" + target;
  if (qualifier.size())
    prefix += "-" + qualifier;

  return prefix;
}

void FileLog::init(std::string path, std::string backupPath,
                   const std::string &prefix) {
  file_mkdir(path.c_str());
  file_mkdir(backupPath.c_str());

  if (path.empty())
    path = ".";
  if (backupPath.empty())
    backupPath = path;

  m_fullPrefix = file_appendpath(path, prefix + ".");
  m_fullBackupPrefix = file_appendpath(backupPath, prefix + ".");

  m_messagesFileName = m_fullPrefix + "messages.current.log";
  m_eventFileName = m_fullPrefix + "event.current.log";

  m_messages.open(m_messagesFileName.c_str(), std::ios::out | std::ios::app);
  if (!m_messages.is_open())
    throw ConfigError("Could not open messages file: " + m_messagesFileName);
  m_event.open(m_eventFileName.c_str(), std::ios::out | std::ios::app);
  if (!m_event.is_open())
    throw ConfigError("Could not open event file: " + m_eventFileName);
}

FileLog::~FileLog() {
  m_messages.close();
  m_event.close();
}

void FileLog::clear() {
  m_messages.close();
  m_event.close();

  m_messages.open(m_messagesFileName.c_str(), std::ios::out | std::ios::trunc);
  m_event.open(m_eventFileName.c_str(), std::ios::out | std::ios::trunc);
}

void FileLog::backup() {
  m_messages.close();
  m_event.close();

  int i = 0;
  while (true) {
    std::stringstream messagesFileName;
    std::stringstream eventFileName;

    messagesFileName << m_fullBackupPrefix << "messages.backup." << ++i
                     << ".log";
    eventFileName << m_fullBackupPrefix << "event.backup." << i << ".log";
    FILE *messagesLogFile = file_fopen(messagesFileName.str().c_str(), "r");
    FILE *eventLogFile = file_fopen(eventFileName.str().c_str(), "r");

    if (messagesLogFile == NULL && eventLogFile == NULL) {
      file_rename(m_messagesFileName.c_str(), messagesFileName.str().c_str());
      file_rename(m_eventFileName.c_str(), eventFileName.str().c_str());
      m_messages.open(m_messagesFileName.c_str(),
                      std::ios::out | std::ios::trunc);
      m_event.open(m_eventFileName.c_str(), std::ios::out | std::ios::trunc);
      return;
    }

    if (messagesLogFile != NULL)
      file_fclose(messagesLogFile);
    if (eventLogFile != NULL)
      file_fclose(eventLogFile);
  }
}

} // namespace DistributedATS
