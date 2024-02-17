 # Copyright (C) 2024 Mike Kipnis - DistributedATS
import jwt
import sqlite3

connection = sqlite3.connect("investors.db")

cursor = connection.cursor()

cursor.execute("CREATE TABLE IF NOT EXISTS investor (jwt TEXT PRIMARY KEY, secret TEXT)")
connection.commit()

investors = [
    {'username':'investor_1', 'secret':'secret_1'},
    {'username':'investor_2', 'secret':'secret_2'},
    {'username':'investor_3', 'secret':'secret_3'},
    {'username':'investor_4', 'secret':'secret_4'},
    {'username':'investor_5', 'secret':'secret_5'}
]
 
for investor in investors:
    secret = investor.pop('secret')
    encoded_jwt = jwt.encode(investor, secret, algorithm="HS256")
    cursor.execute("REPLACE INTO investor (jwt,secret) VALUES (?,?)", (encoded_jwt,secret))
    connection.commit()