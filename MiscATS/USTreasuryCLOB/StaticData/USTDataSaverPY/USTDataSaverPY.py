"""
   Copyright (C) 2022 Mike Kipnis

   This file is part of DistributedATS, a free-software/open-source project
   that integrates QuickFIX and LiquiBook over DDS. This project simplifies
   the process of having multiple FIX gateways communicating with multiple
   matching engines in realtime.

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.

"""


import sys,getopt
import json
import datetime
import sqlite3
import math

#UST_TICK_SIZE = 256
UST_TICK_SIZE = 100
UST_COUPON_TICK_SIZE = 8

def print_help():
    print('USTDataSaverPY populates local database with US Treasuries Static Data');
    print('USTDataSaverPY.py -s static_data file -d database');
    print('try this : python USTDataSaverPY -s ../Data/ust_notes_20210321.json -d ../../DataService/sql/sqlite/distributed_ats.db');


def main(argv):
    static_data = ''
    database = ''

    try:
        opts, args = getopt.getopt(argv, "hs:d:", ["static_data=", "database="])
    except getopt.GetoptError:
        print_help();
        sys.exit(2)
    for opt, arg in opts:
        if opt == '-h':
            print_help();
            sys.exit()
        elif opt in ("-s", "--static_data"):
            static_data = arg
        elif opt in ("-d", "--database"):
            database = arg

    if static_data == "" or database == "":
        print_help();
        exit(1);

    print('Static Data File :', static_data);
    print('Database File :', database)

    connection = sqlite3.connect(database)

    cursor = connection.cursor()
    cursor.execute("PRAGMA foreign_keys = ON");

    f = open(static_data)
    data = json.load(f)

    for instrument in data:
        marutity_date = datetime.datetime.strptime(instrument['maturityDate'], "%Y-%m-%dT%H:%M:%S").strftime("%m/%d/%Y")

        coupon = float(instrument['interestRate'])

        coupon_str = "0"
        if coupon != 0:
            whole_number = int(coupon)
            eighth = int((coupon - whole_number) * UST_COUPON_TICK_SIZE)

            if eighth != 0 :
                gcd = math.gcd(UST_COUPON_TICK_SIZE, eighth)
                coupon_str = str(whole_number) + "_" + str(int(eighth/gcd)) + "/" + str(int(UST_COUPON_TICK_SIZE/gcd))
            else:
                coupon_str = str(whole_number)

        instrument_name = "UST " + coupon_str + " " + marutity_date

        insert_statement = "REPLACE into instrument(instrument_name, properties) values('" + str(instrument_name)  + "','{\"type\":\"UST\", \"tick_size\":" + str(UST_TICK_SIZE) +"}')" #', '{\"type\":\"Equity\"}'")
        print(insert_statement)
        cursor.execute(insert_statement)

        instrument_ref_data = {}
        instrument_ref_data['cusip']=instrument['cusip']
        instrument_ref_data['issueDate']=instrument['issueDate']
        instrument_ref_data['maturityDate']=instrument['maturityDate']
        instrument_ref_data['interestRate'] = instrument['interestRate']

        instrument_ref_data_str = json.dumps(instrument_ref_data)

        update_ref_data = "update instrument set properties=json_insert(properties, \"$.ref_data\",'" + str(instrument_ref_data_str) + "') where instrument_name='" + instrument_name + "'"
        print(update_ref_data)
        cursor.execute(update_ref_data)

        map_instrument_to_market_sql = "REPLACE into instrument_market_map ( instrument_name, market_name ) values ('" + instrument_name +"','UST_MARKET');"
        cursor.execute(map_instrument_to_market_sql)

        price = str(100.0*UST_TICK_SIZE)
        if instrument['pricePer100'] != '':
           price = str(int(float(instrument['pricePer100'])*UST_TICK_SIZE))

        hist_price_statement_sql = "replace into hist_price (instrument_name, business_date, properties) values(\""+instrument_name+"\", 20210401, '{\"open\":"+ price +",\"low\":" + price +",\"high\":" + price + ",\"last\":" + price + ", \"volume\":1000000}')"
        print(hist_price_statement_sql)
        cursor.execute(hist_price_statement_sql)

        connection.commit()

    f.close()
    cursor.close()

if __name__ == "__main__":
   main(sys.argv[1:]);
