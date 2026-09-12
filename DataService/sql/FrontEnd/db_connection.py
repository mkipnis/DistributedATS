import json
import logging
import sqlite3

logger = logging.getLogger("db_connection")


def execute(conn, sql, parameters=()):
    logger.info("Executing SQL: %s | Parameters: %s", sql, parameters)
    cursor = conn.execute(sql, parameters)
    logger.info("SQL execution completed successfully")
    return cursor


def get_database_location(database_file):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "PRAGMA database_list")
        database_name = cursor.fetchone()[2]
        return database_name


def get_user_groups(database_file):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM user_group")
        user_groups = [dict(row) for row in cursor.fetchall()]
        return user_groups


def get_markets(database_file):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM market")
        markets = [dict(row) for row in cursor.fetchall()]
        return markets


def get_group_users(database_file, group_name):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM user_code WHERE user_group = ?", (group_name,))
        group_users = [dict(row) for row in cursor.fetchall()]
        return group_users


def get_markets_for_user_group(database_file, group_name):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM user_group_market_map WHERE user_group = ?", (group_name,))
        markets = [dict(row) for row in cursor.fetchall()]
        return markets


def get_instruments_for_market(database_file, market):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM instrument_market_map WHERE market_name = ?", (market,))
        instruments = [dict(row) for row in cursor.fetchall()]
        return instruments


def get_details_for_instrument(database_file, instrument_name):
    with sqlite3.connect(database_file) as conn:
        conn.row_factory = sqlite3.Row
        cursor = execute(conn, "SELECT * FROM instrument WHERE instrument_name = ?", (instrument_name,))
        instrument_details = [dict(row) for row in cursor.fetchall()]
        return instrument_details


def ins_details_for_instrument(database_file, instrument_name, instrument_details):
    instrument_details = instrument_details or {}
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO instrument (instrument_name, properties) VALUES (?, ?)", (instrument_name, json.dumps(instrument_details)))
    return {"instrument_name": instrument_name, "instrument_details": instrument_details}


def insert_market(database_file, market, properties=None):
    properties = properties or {}
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO market (market_name, properties) VALUES (?, ?)", (market, json.dumps(properties)))
    return {"market_name": market, "properties": properties}


def delete_market(database_file, market):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "DELETE FROM market WHERE market_name = ?", (market,))
    return {"market_name": market}


def delete_user(database_file, user_name):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "DELETE FROM user_code WHERE user_name = ?", (user_name,))
    return {"user_name": user_name}


def insert_instrument_into_market(database_file, instrument, market):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO instrument_market_map (instrument_name, market_name) VALUES (?, ?)", (instrument, market))
    return {"operation": "insert", "instrument": instrument, "market": market}


def delete_instrument_from_market(database_file, instrument, market):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "DELETE FROM instrument_market_map WHERE instrument_name = ? AND market_name = ?", (instrument, market))
    return {"operation": "delete", "instrument": instrument, "market": market}


def insert_user_group(database_file, user_group, properties=None):
    properties = properties or {}
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO user_group (user_group, properties) VALUES (?, ?)", (user_group, json.dumps(properties)))
    return {"user_group": user_group, "properties": properties}

def remove_user_group(database_file, user_group, properties=None):
    properties = properties or {}
    with sqlite3.connect(database_file) as conn:
        execute(conn, "DELETE FROM user_group where user_group=?", (user_group,))
    return {"user_group": user_group, "properties": properties}


def insert_user(database_file, user_name, user_group, properties=None):
    properties = properties or {}
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO user_code (user_name, user_group, properties) VALUES (?, ?, ?)", (user_name, user_group, json.dumps(properties)))
    return {"user_name": user_name, "user_group": user_group, "properties": properties}


def set_market_for_user_group(database_file, user_group, market_name):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "REPLACE INTO user_group_market_map (user_group, market_name) VALUES (?, ?)", (user_group, market_name))
    return {"user_group": user_group, "market_name": market_name}


def remove_market_for_user_group(database_file, user_group, market_name):
    with sqlite3.connect(database_file) as conn:
        execute(conn, "DELETE FROM user_group_market_map WHERE user_group = ? AND market_name = ?", (user_group, market_name))
    return {"user_group": user_group, "market_name": market_name}