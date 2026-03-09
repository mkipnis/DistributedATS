#!/bin/sh

psql "postgresql://postgres:secret@localhost:5430/distributed_ats" <<EOF

CREATE TABLE IF NOT EXISTS user_group (
    user_group text PRIMARY KEY,
    properties jsonb,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS user_code (
    user_name text PRIMARY KEY,
    user_group text,
    properties jsonb,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_group) REFERENCES user_group(user_group)
);

CREATE TABLE IF NOT EXISTS instrument (
    instrument_name text PRIMARY KEY,
    properties jsonb,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS hist_price (
    instrument_name text,
    business_date integer,
    properties jsonb,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(instrument_name, business_date)
);

CREATE TABLE IF NOT EXISTS market (
    market_name text PRIMARY KEY,
    properties jsonb,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS instrument_market_map (
    instrument_name text,
    market_name text,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(instrument_name) REFERENCES instrument(instrument_name),
    FOREIGN KEY(market_name) REFERENCES market(market_name),
    UNIQUE(instrument_name, market_name)
);

CREATE TABLE IF NOT EXISTS user_group_market_map (
    user_group text,
    market_name text,
    last_update_time timestamp DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_group) REFERENCES user_group(user_group),
    FOREIGN KEY(market_name) REFERENCES market(market_name),
    UNIQUE(user_group, market_name)
);

EOF