# Copyright (c) Mike Kipnis - DistributedATS

import datetime

import dash
from dash import Input, Output, html, dcc, State
from dash import callback_context
import dash_bootstrap_components as dbc
from dash_extensions import WebSocket


class LogonPanel(object):

    def __init__(self, app: dash.Dash):
        self.app = app
        self._register_callbacks()

    def layout(self):
        return dbc.Container(
            [

                # Store
                dcc.Store(
                    id="login-state",
                    data={
                        "text": "Please log in",
                        "sessionStateCode": 0,
                    },
                ),

                # Info message (used by callback)
                html.Div(
                    id="info-message",
                    className="mb-3",
                    style={"fontWeight": "500"}
                ),

                # Form wrapper (used by callback)
                html.Div(
                    id="form-wrapper",
                    children=[
                        dbc.Row(
                            [
                                dcc.Location(id="url", refresh=False),
                                html.Div(id="ws-url"),

                                # Username
                                dbc.Col(
                                    [
                                        dbc.Label("Username"),
                                        dbc.Input(
                                            id="username",
                                            type="text",
                                            placeholder="Enter username",
                                            style={
                                                "backgroundColor": "#0f1118",
                                                "color": "white",
                                                "border": "1px solid #2a2e39",
                                            },
                                        ),
                                    ],
                                    md=4,
                                ),

                                # Password
                                dbc.Col(
                                    [
                                        dbc.Label("Password"),
                                        dbc.Input(
                                            id="password",
                                            type="password",
                                            placeholder="Enter password",
                                            style={
                                                "backgroundColor": "#0f1118",
                                                "color": "white",
                                                "border": "1px solid #2a2e39",
                                            },
                                        ),
                                    ],
                                    md=4,
                                ),

                                # Button
                                dbc.Col(
                                    dbc.Button(
                                        "Login",
                                        id="login-btn",
                                        color="primary",
                                        className="mt-4 w-100",
                                        n_clicks=0,
                                    ),
                                    md=2,
                                    align="end",
                                ),
                            ],
                            className="g-3 align-items-end",
                            style={"marginTop": "20px"},
                        ),
                        WebSocket(id="ws", url="ws://127.0.0.1:8765")
                    ],
                ),
            ],
            fluid=True,
        )


    def _register_callbacks(self):

        @self.app.callback(Output("output", "children"),
                      Input("ws", "message"))
        def display_message(msg):
            if msg is None:
                return "No data yet"
            return f"Received: {msg['data']}"

        @self.app.callback(
            Output("ws-url", "children"),
            Input("url", "href")
        )
        def get_websocket_url(href):
            if not href:
                return ""

            from urllib.parse import urlparse

            parsed = urlparse(href)

            host = parsed.hostname
            port = 9002
            protocol = "wss" if parsed.scheme == "https" else "ws"

            return f"{protocol}://{host}:{port}"

        # ----------------------------
        # Login Submit Logic
        # ----------------------------
        @self.app.callback(
            Output("login-state", "data"),
            Input("login-btn", "n_clicks"),
            State("username", "value"),
            State("password", "value"),
            prevent_initial_call=True,
        )
        def handle_login(n, username, password):
            if not username or not password:
                return {
                    "text": "Username and password required",
                    "sessionStateCode": 0
                }

            # This replaces props.logonCallback(...)
            logon_value = {
                "username": username,
                "password": password,
            }

            print("Logon value:", logon_value)

            return {
                "text": "Ready to trade",
                "sessionStateCode": 1
            }

        @self.app.callback(
            Output("info-message", "children"),
            Output("info-message", "className"),
            Output("form-wrapper", "style"),
            Input("login-state", "data"),
        )
        def update_ui(login_state):
            text = login_state["text"]
            session_code = login_state["sessionStateCode"]

            # Equivalent to:
            # props.loginState.text === "Ready to trade"
            class_name = "header_ready_to_trade" if text == "Ready to trade" else ""

            # Equivalent to:
            # pointerEvents: "none", opacity: "0.4"
            if session_code != 0:
                style = {"pointerEvents": "none", "opacity": "0.4"}
            else:
                style = {}

            return text, class_name, style