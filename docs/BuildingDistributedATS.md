## Building Distributed ATS

All DistributedATS dependences need to be built and MySQL needs to be installed prior to building the project.

[See: Dependences](README.md#Dependences)

The following environmental variables along with DDS variables must be set prior to building the project: DATS_HOME(DistributedATS home), QUICKFIX_HOME, MYSQL_HOME, LIQUIDBOOK_HOME as well as LD_LIBRARY_PATH. 

Example of an environmental variable file where all dependences were built under $HOME and MySQL was installed in its default location:
```
. $HOME/OpenDDS-3.9/setenv.sh # OpenDDS and it's dependences

export DATS_HOME=$HOME/DistributedATS # DistributedATS home

export QUICKFIX_HOME=$HOME/quickfix
export MYSQL_HOME=/usr/local/mysql
export LIQUIDBOOK_HOME=$HOME/liquibook-master
export DYLD_LIBRARY_PATH=$HOME/quickfix/src/C++/.libs:$DYLD_LIBRARY_PATH
```

DistributedATS utilizes Make Project Creator as a build tool (Make Project Creator)[http://downloads.ociweb.com/MPC/docs/html/MakeProjectCreator.html]

Upon successfully building and installing all the dependences, the following set of commands can be used to build the project:
```
$ACE_ROOT/bin/mwc.pl -type gnuace
make realclean
make
```
