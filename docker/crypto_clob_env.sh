# dependencies
export DATS_HOME=/usr/local
export DEPS_HOME=/usr/local
export LD_LIBRARY_PATH=$DEPS_HOME/lib:$LD_LIBRARY_PATH
export LOG4CXX_CONFIGURATION=$DATS_HOME/config/log4cxx.xml
export FASTDDS_DEFAULT_PROFILES_FILE=/usr/local/FastDDS.xml

# ATS HOME
export BASEDIR_ATS=$DATS_HOME/MiscATS/CryptoCLOB
export DATS_LOG_HOME=$BASEDIR_ATS/logs
mkdir -p $BASEDIR_ATS/logs
