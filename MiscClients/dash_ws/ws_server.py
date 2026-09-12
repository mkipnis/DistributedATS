import asyncio
import json
import random
import websockets
from websockets.exceptions import ConnectionClosed

connected = set()

# ----------------------------
# Receive loop (client → server)
# ----------------------------
async def receiver(websocket):
    try:
        async for message in websocket:
            data = json.loads(message)
            print("Received:", data)
    except ConnectionClosed:
        print("Receiver: client disconnected")


# ----------------------------
# Send loop (server → client)
# ----------------------------
async def sender(websocket):
    try:
        while True:
            data = {"price": random.random()}
            await websocket.send(json.dumps(data))
            await asyncio.sleep(1)
    except ConnectionClosed:
        print("Sender: client disconnected")


# ----------------------------
# Connection handler
# ----------------------------
async def handler(websocket):
    connected.add(websocket)
    print("Client connected")

    recv_task = asyncio.create_task(receiver(websocket))
    send_task = asyncio.create_task(sender(websocket))

    try:
        done, pending = await asyncio.wait(
            [recv_task, send_task],
            return_when=asyncio.FIRST_COMPLETED,
        )
    finally:
        # Cancel remaining tasks properly
        for task in pending:
            task.cancel()
            try:
                await task
            except asyncio.CancelledError:
                pass

        connected.remove(websocket)
        print("Client disconnected")


# ----------------------------
# Server
# ----------------------------
async def ws_main():
    async with websockets.serve(handler, "127.0.0.1", 8765):
        print("WebSocket server running on ws://127.0.0.1:8765")
        await asyncio.Future()