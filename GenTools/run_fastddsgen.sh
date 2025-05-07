#!/bin/bash

FASTDDSGEN="${FAST_DDS_GEN_HOME}/scripts/fastddsgen"
IDL_DIR="idl" 

"$FASTDDSGEN" $IDL_DIR/Header.idl

for idl_file in "$IDL_DIR"/*.idl; do
  echo "Generating code for $idl_file..."
  "$FASTDDSGEN" -replace "$idl_file"
done
