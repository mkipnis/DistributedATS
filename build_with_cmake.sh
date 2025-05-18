#!/usr/bin/env bash

set -ex  # Print each command and exit on error

# Detect OS for library path
OS="$(uname)"
if [[ "$OS" == "Darwin" ]]; then
  LIB_PATH_VAR="DYLD_LIBRARY_PATH"
else
  LIB_PATH_VAR="LD_LIBRARY_PATH"
fi

# Create and enter build directory
mkdir -p build
cd build

# Get absolute path to current directory (fallback for macOS without realpath)
get_abs_path() {
  if command -v realpath >/dev/null 2>&1; then
    realpath "$1"
  else
    # Fallback using Python for macOS
    python3 -c "import os; print(os.path.abspath('$1'))"
  fi
}

ROOT_DIR="$(get_abs_path ..)"

# Set default paths
if [ -z "$1" ]; then
  DDS_HOME="$ROOT_DIR/dds"
  QUICKFIX_HOME="$ROOT_DIR/quickfix"
  LOG4CXX_HOME="$ROOT_DIR/log4cxx"
  INSTALL_PREFIX="$ROOT_DIR/DistributedATS"
else
  INSTALL_PREFIX="$(get_abs_path "$1")"
fi

# Run cmake and build
cmake ${CMAKE_FLAGS:-} .. -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"
cmake --build . --target install --config Debug -v

# Write the environment setup script
cat <<EOM > "$INSTALL_PREFIX/dats_env.sh"
#!/usr/bin/env bash

export DATS_HOME="$INSTALL_PREFIX"
export DDS_HOME="$DDS_HOME"
export QUICKFIX_HOME="$QUICKFIX_HOME"
export LOG4CXX_HOME="$LOG4CXX_HOME"

export $LIB_PATH_VAR="\$DATS_HOME/lib:\$DDS_HOME/lib:\$QUICKFIX_HOME/lib:\$LOG4CXX_HOME/lib:\$$LIB_PATH_VAR"
export LOG4CXX_CONFIGURATION="\$DATS_HOME/config/log4cxx.xml"

# Select one ATS config
export BASEDIR_ATS="\$DATS_HOME/MiscATS/CryptoCLOB"
export DATS_LOG_HOME="\$BASEDIR_ATS/logs"
mkdir -p "\$DATS_LOG_HOME"

# Uncomment one of these to switch ATS
# export BASEDIR_ATS="\$DATS_HOME/MiscATS/USTreasuryCLOB"
# export DATS_LOG_HOME="\$BASEDIR_ATS/logs"
# mkdir -p "\$DATS_LOG_HOME"

# export BASEDIR_ATS="\$DATS_HOME/MiscATS/MultiMatchingEngineATS"
# export DATS_LOG_HOME="\$BASEDIR_ATS/logs"
# mkdir -p "\$DATS_LOG_HOME"

EOM

chmod +x "$INSTALL_PREFIX/dats_env.sh"
