import argparse
import time

import requests
import json
import jwt

investor_1 = \
    {
        'secret': 'secret_1',
        'token': 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImludmVzdG9yXzEifQ.UcFhJ9edf6egfK286FQoeFdfaN-VH8f7Ka1sXHdL38c'
    }
investor_2 = \
    {
        'secret': 'secret_2',
        'token': 'eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImludmVzdG9yXzIifQ.Bpnm03NeJU_tblzFTqxCpRxUm6K757nSpXkiLaA_UZ4'
    }


def print_json(json_object: json):
    json_formatted_str = json.dumps(json_object, indent=2)
    print(json_formatted_str)


def get_instruments(base_url: str) -> dict:
    response = requests.get(base_url + '/instruments')
    return json.loads(response.text)


def get_market_data(base_url: str, instrument_list: list) -> dict:
    response = requests.post(base_url + '/market_data', json=list(instrument_list))
    response_dict = json.loads(response.text)
    return response_dict


def submit_order_from_market_data(base_url: str, investor: dict, instrument: dict, side: str,
                                  price: int, order_qty: int):
    new_order_single = {}
    new_order_single['user_token'] = investor['token']
    new_order_single['instrument'] = instrument
    new_order_single['new_order_single'] = {}
    new_order_single['new_order_single']['side'] = side
    new_order_single['new_order_single']['order_type'] = '1'
    new_order_single['new_order_single']['price'] = price
    new_order_single['new_order_single']['order_qty'] = order_qty
    new_order_single['new_order_single']['time_in_force'] = '0'
    response = requests.post(base_url + '/submit_order', json=new_order_single)
    response_dict = jwt.decode(response.text, investor['secret'], algorithms="HS256")

    return response_dict


def cancel_order(base_url: str, investor: dict, instrument: dict, client_order_id: str):
    cancel_order = {}
    cancel_order['user_token'] = investor['token']
    cancel_order['instrument'] = instrument
    cancel_order['cancel_order'] = {}
    cancel_order['cancel_order']['side'] = '1'
    cancel_order['cancel_order']['client_order_id'] = client_order_id
    response = requests.post(base_url + '/cancel_order', json=cancel_order)
    response_dict = jwt.decode(response.text, investor['secret'], algorithms="HS256")

    return response_dict


def get_inverstor_orders(base_url: str, investor: dict):
    investor_order_request = {}
    investor_order_request['user_token'] = investor['token']
    response = requests.post(base_url + '/investor_orders', json=investor_order_request)
    response_dict = jwt.decode(response.text, investor['secret'], algorithms="HS256")
    return response_dict


def get_order_status(base_url: str, investor: dict, orders: list):
    order_status_request = {}
    order_status_request['user_token'] = investor['token']
    order_status_request['orders'] = orders
    response = requests.post(base_url + '/order_status_request', json=order_status_request)
    response_dict = jwt.decode(response.text, investor['secret'], algorithms="HS256")
    return response_dict


if __name__ == '__main__':

    parser = argparse.ArgumentParser(
        prog='Basic REST Client',
        description='Basic test client for golang RESTAPI to DistributedATS')

    parser.add_argument('-u', '--base_url', help='BASE_URL of RESTAPI', required=True)
    args = parser.parse_args()

    base_url = args.base_url
    print("BaseURL: " + args.base_url)

    instruments = get_instruments(base_url)
    market_data = get_market_data(base_url, instruments.values())

    for instrument, instrument_market_data in market_data.items():
        print("Instrument:")
        print_json(json.loads(instrument))
        print_json(instrument_market_data)
        passive_order_response = submit_order_from_market_data(base_url, investor_1, json.loads(instrument), '1',
                                                               instrument_market_data['open_price'], 100)
        print("Passive Order Response:")
        print_json(passive_order_response)
        time.sleep(1)
        aggressive_order_response = submit_order_from_market_data(base_url, investor_2, json.loads(instrument), '2',
                                                                  instrument_market_data['open_price'], 50)
        print("Aggressive Order Response:")
        print_json(aggressive_order_response)
        time.sleep(1)
        print("Cancelling remainder of the passive order:")
        cancel_response = cancel_order(base_url, investor_1, json.loads(instrument),
                                       passive_order_response['data']['client_order_id'])
        print("Cancel Order Response:")
        print_json(cancel_response)
        break

    print("Getting all orders submitted by secret_1")
    investor_orders = get_inverstor_orders(base_url, investor_1)
    for order in investor_orders['data']['orders']:
        print_json(order)

    print("Lets check orders")
    order_status = get_order_status(base_url, investor_1, investor_orders['data']['orders'])
    print_json(order_status)

    exit(0)
