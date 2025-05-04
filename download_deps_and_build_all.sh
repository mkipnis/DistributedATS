#!/bin/bash

set -euo pipefail

set -x

# Define directories
DATS_SOURCE_DIR="$(pwd)"
DEPS_BUILD_DIR="$HOME/distributed_ats_deps_build"
INSTALL_DIR="${1:-$DEPS_BUILD_DIR}"
LD_LIBRARY_PATH="LD_LIBRARY_PATH"

mkdir -p "$DEPS_BUILD_DIR"

# Mac-specific flags
CMAKE_FLAGS=""
if [[ "$OSTYPE" == "darwin"* ]]; then
    export CXXFLAGS="-DWAZOO_64_BIT -std=c++11 -stdlib=libc++"
    CMAKE_FLAGS="-G Xcode"
    LD_LIBRARY_PATH="DYLD_LIBRARY_PATH"
fi

# Dependency versions
FOONATHAN_MEMORY_PKG=0.7-3
FAST_CDR_PKG=2.3.0
ASIO_PKG=1-28-0
FAST_DDS_PKG=3.2.1
LOG4CXX_PKG=1.2.0
QUICKFIX_PKG=1.15.1
LIQUIBOOK_PKG=2.0.0

# Download helper
download_if_missing() {
  local url=$1
  local dest=$2
  if [[ ! -f "$dest" ]]; then
    echo "Downloading $(basename "$dest")..."
    curl -L "$url" -o "$dest" || {
      echo " Failed to download $(basename "$dest")."
      exit 1
    }
  fi
}

# Download dependencies
download_if_missing "https://github.com/foonathan/memory/archive/refs/tags/v$FOONATHAN_MEMORY_PKG.tar.gz" "$DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz"
download_if_missing "https://github.com/eProsima/Fast-CDR/archive/refs/tags/v$FAST_CDR_PKG.tar.gz" "$DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz"
download_if_missing "https://github.com/eProsima/Fast-DDS/archive/refs/tags/v$FAST_DDS_PKG.tar.gz" "$DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz"
download_if_missing "https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-$ASIO_PKG.tar.gz" "$DEPS_BUILD_DIR/asio-$ASIO_PKG.tar.gz"
download_if_missing "https://github.com/apache/logging-log4cxx/archive/refs/tags/rel/v$LOG4CXX_PKG.tar.gz" "$DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz"
download_if_missing "https://github.com/quickfix/quickfix/archive/refs/tags/v$QUICKFIX_PKG.tar.gz" "$DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz"
download_if_missing "https://github.com/enewhuis/liquibook/archive/refs/tags/$LIQUIBOOK_PKG.tar.gz" "$DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz"

export INSTALL_PREFIX="$INSTALL_DIR"

build_and_install() {

   local tarball="$1"
    local dir="$2"
    local configure_cmd="${3:-./configure --prefix=$INSTALL_PREFIX --exec-prefix=$INSTALL_PREFIX}"
    local build_cmd="${4:-make install -j $(nproc)}"

    cd "$DEPS_BUILD_DIR"
    [[ ! -d "$dir" ]] && tar xvf "$tarball"
    cd "$dir"

    if [[ -f CMakeLists.txt ]]; then
        mkdir -p build && cd build
        cmake .. -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" -DBUILD_SHARED_LIBS=ON
        cmake --build . --target install
    else
        eval "$configure_cmd"
        eval "$build_cmd"
    fi

}

[[ ! -f $INSTALL_DIR/include/log4cxx/log4cxx.h ]] && build_and_install "$DEPS_BUILD_DIR/log4cxx-$LOG4CXX_PKG.tar.gz" "$DEPS_BUILD_DIR/logging-log4cxx-rel-v$LOG4CXX_PKG" "-DBUILD_TESTING=false"
[[ ! -f $INSTALL_DIR/include/foonathan_memory/foonathan/memory/config.hpp ]] && build_and_install "$DEPS_BUILD_DIR/memory-v$FOONATHAN_MEMORY_PKG.tar.gz" "memory-$FOONATHAN_MEMORY_PKG" ""
[[ ! -f $INSTALL_DIR/include/fastcdr/config.h ]] && build_and_install "$DEPS_BUILD_DIR/Fast-CDR-v$FAST_CDR_PKG.tar.gz" "Fast-CDR-$FAST_CDR_PKG" ""
[[ ! -f $INSTALL_DIR/include/asio.hpp ]] && build_and_install "$DEPS_BUILD_DIR/asio-$ASIO_PKG.tar.gz" "asio-asio-$ASIO_PKG" "cd asio && ./autogen.sh && ./configure --prefix=$INSTALL_PREFIX --exec-prefix=$INSTALL_PREFIX}"
[[ ! -f $INSTALL_DIR/include/fastdds/config.hpp ]] && build_and_install "$DEPS_BUILD_DIR/Fast-DDS-v$FAST_DDS_PKG.tar.gz" "Fast-DDS-$FAST_DDS_PKG" ""
[[ ! -f $INSTALL_DIR/include/quickfix/config-all.h ]] && build_and_install "$DEPS_BUILD_DIR/quickfix-v$QUICKFIX_PKG.tar.gz" "quickfix-$QUICKFIX_PKG" ""
[[ ! -f $INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src/liquibook_export.h ]] && build_and_install "$DEPS_BUILD_DIR/liquibook-$LIQUIBOOK_PKG.tar.gz" "liquibook-$LIQUIBOOK_PKG" ""


# Generate environment script
cat <<EOM > "$HOME/DistributedATS/dats_env.sh"
export DATS_HOME=\$HOME/DistributedATS
export DEPS_HOME=$INSTALL_DIR
export $LD_LIBRARY_PATH=\$DEPS_HOME/lib:\$DATS_HOME/lib:\$LD_LIBRARY_PATH
export $LD_LIBRARY_PATH=\$DATS_HOME/config/log4cxx.xml
export BASEDIR_ATS=\$DATS_HOME/MiscATS/CryptoCLOB
export DATS_LOG_HOME=\$BASEDIR_ATS/logs
EOM

cd $DATS_SOURCE_DIR

# Build DistributedATS
mkdir -p build
cd build

cmake $CMAKE_FLAGS .. \
  -Dfastcdr_DIR=$INSTALL_DIR/lib/cmake/fastcdr/ \
  -Dfastdds_DIR=$INSTALL_DIR/share/fastdds/cmake/ \
  -Dfoonathan_memory_DIR=$INSTALL_DIR/lib/foonathan_memory/cmake/ \
  -Dlog4cxx_DIR=$INSTALL_DIR/lib/cmake/log4cxx \
  -DCMAKE_INSTALL_PREFIX="$HOME/DistributedATS" \
  -DLIQUIBOOK_HOME="$INSTALL_DIR/liquibook-$LIQUIBOOK_PKG/src" \
  -DQUICKFIX_INSTALL_PREFIX="$INSTALL_DIR"

cmake --build . --target install --config Debug
