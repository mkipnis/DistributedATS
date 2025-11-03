# Copyright (C) Mike Kipnis - DistributedATS
import asyncio
import json
import argparse
from datetime import datetime, timezone

import websockets


def compose_header(sender_comp_id: str, target_comp_id: str, msg_type: str):
    return {
        'Header': {
            '8': "FIX.4.4",
            '35': msg_type,
            '49': sender_comp_id,
            '56': target_comp_id,
        }
    }


def compose_logon(sender_comp_id: str, target_comp_id: str):
    logon = compose_header(sender_comp_id, target_comp_id, 'A')
    logon['Body'] = {'553': sender_comp_id, '554': "TEST"}
    return logon


def compose_security_list_request(sender_comp_id: str, target_comp_id: str):
    req = compose_header(sender_comp_id, target_comp_id, 'x')
    req['Body'] = {'320': 'RequestInstrList1', '559': 4}
    return req


def compose_mass_order_status_request(sender_comp_id: str, target_comp_id: str):
    req = compose_header(sender_comp_id, target_comp_id, 'AF')
    req['Body'] = {'584': 'RequestMassOrdStatus1', '559': 7}
    return req


def compose_market_data_request(sender_comp_id: str, target_comp_id: str, instruments: list):
    req = compose_header(sender_comp_id, target_comp_id, 'V')
    req['Body'] = {
        '262': 'MarketDataSnapshot_1',
        '263': 1,
        '264': 0,
        '146': [],
        '267': [{"269": 0}, {"269": 1}],
    }

    for instr in instruments:
        req['Body']['146'].append({k: instr[k] for k in ["55", "207"] if k in instr})

    return req


def compose_order_from_open_price(sender_comp_id: str, target_comp_id: str, snapshot: dict):
    order = compose_header(sender_comp_id, target_comp_id, 'D')
    order['Body'] = {
        '11': f"{snapshot['Body']['55']}_1",
        '55': snapshot['Body']['55'],
        '207': snapshot['Body']['207'],
        '40': 2,  # Limit
        '59': 0,  # Time in force
        '60': datetime.now(timezone.utc).strftime("%Y%m%d-%H:%M:%S.%f")[:-3],
    }

    for entry in snapshot['Body'].get('268', []):
        if entry.get('269') == '4':  # Open price
            order['Body'].update({'54': 1, '44': entry['270'], '38': 1000})
            break

    return order


def pretty_print(description: str, message: dict):
    print(f"\n{description}:\n{json.dumps(message, indent=4)}")


async def run_fix_session(sender_comp_id: str, target_comp_id: str, ws_url: str):
    async with websockets.connect(ws_url) as ws:
        print(f"✅ Connected to {ws_url}")

        # Send logon
        logon = compose_logon(sender_comp_id, target_comp_id)
        await ws.send(json.dumps(logon))
        pretty_print("➡️ Sent Logon", logon)

        session_msg = await ws.recv()
        pretty_print("⬅️ Session Qualifier", json.loads(session_msg))

        try:
            async for raw_msg in ws:
                msg = json.loads(raw_msg)

                if msg.get("data_type") == "WS":
                    continue

                msg_type = msg.get('Header', {}).get('35')

                pretty_print("⬅️ Incoming Message", msg)

                if msg_type == 'A':  # Logon Ack
                    req1 = compose_security_list_request(sender_comp_id, target_comp_id)
                    req2 = compose_mass_order_status_request(sender_comp_id, target_comp_id)
                    await ws.send(json.dumps(req1))
                    await ws.send(json.dumps(req2))
                    pretty_print("➡️ Sent Security List Request", req1)
                    pretty_print("➡️ Sent Mass Order Request", req2)

                elif msg_type == 'y':  # Security List
                    instruments = msg['Body']['146']
                    md_req = compose_market_data_request(sender_comp_id, target_comp_id, instruments)
                    await ws.send(json.dumps(md_req))
                    pretty_print("➡️ Sent Market Data Request", md_req)

                elif msg_type == 'W':  # Market Data Snapshot
                    pretty_print("📈 Market Data Snapshot", msg)
                    order = compose_order_from_open_price(sender_comp_id, target_comp_id, msg)
                    await ws.send(json.dumps(order))
                    pretty_print("➡️ Sent Order", order)

                elif msg_type == 'X':
                    pretty_print("📊 Market Data Update", msg)

                elif msg_type == '8':
                    pretty_print("🧾 Execution Report", msg)

        except websockets.ConnectionClosed:
            print("❌ Connection closed.")
        except Exception as e:
            print(f"⚠️ Error: {e}")


def main():
    parser = argparse.ArgumentParser(description="FIX over WebSocket client")
    parser.add_argument("--sender", required=True, help="SenderCompID (e.g., TRADER_1)")
    parser.add_argument("--target", required=True, help="TargetCompID (e.g., FIX_GWY_1)")
    parser.add_argument("--url", required=True, help="WebSocket URL (e.g., ws://localhost:9002)")

    args = parser.parse_args()

    asyncio.run(run_fix_session(args.sender, args.target, args.url))


if __name__ == "__main__":
    main()
