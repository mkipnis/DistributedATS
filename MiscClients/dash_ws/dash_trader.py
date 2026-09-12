# Copyright (c) Mike Kipnis - DistributedATS

import os
import logging
import threading
from datetime import datetime, timedelta

import dash
import dash_ag_grid as dag
import dash_bootstrap_components as dbc
from dash import html, dcc, Output, Input
from components import login_panel
from components import positions_and_market_data
from components import price_level_panel
from components import ticket_panel

from dash_extensions import WebSocket

import asyncio
import websockets
import json
import random

from ws_server import ws_main

# ----------------------------
# Logging configuration
# ----------------------------
logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
    handlers=[
        logging.StreamHandler(),
        logging.FileHandler("dash_trader.log", "a"),
    ],
)

logger = logging.getLogger("dash_trader")

# =============================
# Dash App Initialization
# =============================
app = dash.Dash(
    __name__,
    title="Dash Trader",
    external_stylesheets=[dbc.themes.SUPERHERO],
)

server = app.server  # Gunicorn expects this

class DashTrader:
    def __init__(self, app: dash.Dash):
        self.app = app
        self.login_panel = login_panel.LogonPanel(app)
        self.positions_and_market_data_panel = positions_and_market_data.PositionAndMarketDataPanel(app)
        self.price_level_panel = price_level_panel.PriceLevelPanel(app)
        self.ticket_panel = ticket_panel.TicketPanel(app)

    def layout(self):
        return dbc.Container(
            [
                dbc.Navbar(
                    dbc.Container(
                        [
                            # Left side
                            dbc.NavbarBrand(
                                "Dash Trader",
                                href="#",
                                style={"fontSize": "20px"},
                            ),

                            # Right side
                            html.Div(
                                [
                                    dbc.Row(
                                        [
                                            self.login_panel.layout(),
                                            self.ticket_panel.layout()
                                        ]
                                    ),

                                    html.Div(id="status", className="text-white ms-2"),
                                ],
                                className="d-flex flex-column align-items-start ms-auto",
                            ),
                        ],
                        fluid=True,
                    ),
                    color="#0f2538",
                    dark=True,
                ),

                html.Div(
                    [
                dbc.Accordion(
                    [

                        # --- Sector / Industry ---
                        dbc.AccordionItem(
                            dbc.Row(
                                [
                                    dbc.Col(
                                        self.positions_and_market_data_panel.layout()
                                    ),
                                    dbc.Col(
                                        dbc.Row(
                                            [
                                                self.price_level_panel.layout()
                                            ]),
                                    )
                                ],
                                className="p-3",
                            ),
                            title="Sector & Industry",
                            item_id="sector-industry",
                        ),

                        # --- Institution / Stock ---
                        dbc.AccordionItem(
                            dbc.Row(
                                [
                                ],
                                className="p-3",
                            ),
                            title="Institution & Stock",
                            item_id="institution-stock",
                        ),

                        # --- Price Chart / Market Data ---
                        dbc.AccordionItem(
                            dbc.Row(
                                [
                                ],
                                className="p-3 flex-nowrap",  # flex-nowrap keeps them on the same row
                                style={"overflowX": "auto"},  # optional: allows horizontal scroll if content is wide
                                # no_gutters=True  # older versions of dash-bootstrap-components
                            ),
                            title="Price Chart & Market Data",
                        ),

                        # --- News ---
                        dbc.AccordionItem(
                            dbc.Row(
                                [
                                ],
                                className="p-3",
                            ),
                            title="News",
                        ),

                    ],
                    id="main-accordion",
                    start_collapsed=False,  # all collapsed on load
                    flush=True,  # cleaner look
                    always_open=True,  # allow multiple open at once
                    active_item=["sector-industry", "institution-stock"],
                )], id="accordion-wrapper",),

                html.Div(
                    [
                        "For support, contact: ",
                        html.A(
                            "mike.kipnis@gmail.com",
                            href="mailto:mike.kipnis@gmail.com",
                            style={
                                "textDecoration": "underline",
                                "color": "#AAAAAA",
                            },
                        ),
                    ],
                    style={
                        "fontSize": "12px",
                        "color": "#AAAAAA",
                        "marginTop": "8px",
                        "textAlign": "center",
                        "width": "100%",
                    },
                ),
            ],
            fluid=True,
            className="p-0",
        )


def start_ws_server():
    asyncio.run(ws_main())

# Create sandbox with this prefix
dash_trader = DashTrader(app)

# Set layout once
app.layout = dash_trader.layout

# ----------------------------
# Main
# ----------------------------
if __name__ == "__main__":

    # Start WebSocket server in background thread
    ws_thread = threading.Thread(target=start_ws_server, daemon=True)
    ws_thread.start()

    host = os.getenv("DASH_HOST", "127.0.0.1")
    port = int(os.getenv("DASH_PORT", "8050"))
    debug = os.getenv("DASH_DEBUG", "true").lower() == "true"

    app.run(host=host, port=port, debug=debug, use_reloader=False)