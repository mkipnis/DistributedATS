export DATS_HOME=/opt/DistributedATS
export DEPS_HOME=/opt/distributed_ats_deps_build
export LD_LIBRARY_PATH=$DEPS_HOME/lib:$DATS_HOME/lib:$LD_LIBRARY_PATH
export LOG4CXX_CONFIGURATION=/opt/DistributedATS/config/log4cxx.xml

#echo Specific Specific

#echo Crypto ATS
#export BASEDIR_ATS=$DATS_HOME/MiscATS/CryptoCLOB
#export DATS_LOG_HOME=$BASEDIR_ATS/logs
#mkdir -p $BASEDIR_ATS/logs

#echo US Treasuries
export BASEDIR_ATS=$DATS_HOME/MiscATS/USTreasuryCLOB
export DATS_LOG_HOME=$BASEDIR_ATS/logs
mkdir -p $BASEDIR_ATS/logs

#echo US Treasuries
#export BASEDIR_ATS=$DATS_HOME/MiscATS/MultiMatchingEngineATS
#export DATS_LOG_HOME=$BASEDIR_ATS/logs
#mkdir -p $BASEDIR_ATS/logs
