#!/bin/bash


${FAST_DDS_GEN_HOME}/scripts/fastddsgen Heartbeat.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen Logout.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen ExecutionReport.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderCancelReject.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen Logon.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen NewOrderSingle.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderCancelRequest.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderCancelReplaceRequest.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen MarketDataRequest.idl 
${FAST_DDS_GEN_HOME}/scripts/fastddsgen MarketDataSnapshotFullRefresh.idl
${FAST_DDS_GEN_HOME}/scripts/fastddsgen MarketDataIncrementalRefresh.idl
${FAST_DDS_GEN_HOME}/scripts/fastddsgen BusinessMessageReject.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderMassCancelRequest.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderMassCancelReport.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen SecurityListRequest.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen SecurityList.idl
${FAST_DDS_GEN_HOME}/scripts/fastddsgen OrderMassStatusRequest.idl -typeobject
${FAST_DDS_GEN_HOME}/scripts/fastddsgen Header.idl -typeobject
