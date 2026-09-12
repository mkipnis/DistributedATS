# Copyright (c) Mike Kipnis - DistributedATS
import json
import logging
import dash_ag_grid as dag
from dash import html, dash, Output, Input, State
from numpy.ma.core import append
import dash_bootstrap_components as dbc

import db_connection

logger = logging.getLogger("UsersPanel")

class UsersPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {"field": "user_name", "headerName": "User Group", "flex": 2},
            {"field": "name", "headerName": "Name", "flex": 2},
            {"field": "type", "headerName": "Type", "flex": 2},
            {"field": "last_update_time", "headerName": "Last Update Time", "flex": 2},
        ]

        self.users_grid = dag.AgGrid(
            id="users_grid_id",
            columnDefs=column_defs,
            rowData=[],
            defaultColDef={"minWidth": 100, "resizable": True},
            style={"height": "400px", "width": "100%"},
            className="ag-theme-quartz",
            dashGridOptions={
                "rowSelection": "single",
                "animateRows": True,
                "suppressMaintainUnsortedOrder": True
            },
        )

        self._register_callbacks()

        pass

    def layout(self):
        return html.Div(
            children=[
                self.users_grid,
                html.Div(
                    children=[
                        dbc.Row(
                            [
                                dbc.Col(
                                    [
                                        html.Label("User Name",
                                                   style={"fontWeight": "bold", "marginBottom": "4px", }, ),
                                        dbc.Input(id="user_name_input", type="text", placeholder="Enter User Name",
                                                  size="sm", style={"width": "300px", }, ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Label("Name",
                                                   style={"fontWeight": "bold", "marginBottom": "4px", }, ),
                                        dbc.Input(id="name_input", type="text", placeholder="Enter Name",
                                                  size="sm", style={"width": "300px", }, ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Label(
                                            "Password",
                                            style={"fontWeight": "bold", "marginBottom": "4px"},
                                        ),
                                        dbc.Input(
                                            id="password_input",
                                            type="password",
                                            placeholder="Enter Password",
                                            size="sm",
                                            style={"width": "300px"},
                                        ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Label(
                                            "Role",
                                            style={"fontWeight": "bold", "marginBottom": "4px"},
                                        ),
                                        dbc.Select(
                                            id="role_select",
                                            options=[
                                                {"label": "Matching Engine", "value": "MATCHING_ENGINE"},
                                                {"label": "Trader", "value": "TRADER"},
                                            ],
                                            value="TRADER",
                                            placeholder="Select Role",
                                            size="sm",
                                            style={"width": "300px"},
                                        ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Button("Add", id="user_add", className="btn btn-dark btn-sm me-2", ),
                                        html.Button("Remove", id="user_remove", className="btn btn-dark btn-sm", ),
                                    ],
                                    width="auto",
                                    className="d-flex align-items-end",
                                ),
                            ],
                            className="align-items-end",
                        ),
                    ],
                ),

                # -------------------------------------------------
                # Error message
                # -------------------------------------------------

                html.Div(
                    id="user_error",
                    style={
                        "color": "#dc3545",
                        "marginTop": "5px",
                    },
                ),
            ],
            **{"data-ag-theme-mode": "dark"}  # Unpacks the dictionary as an HTML keyword attribute
        )

    def _register_callbacks(self):
        @self.app.callback(
            Output("users_grid_id", "rowData"),

            Output("user_name_input", "value"),
            Output("name_input", "value"),
            Output("password_input", "value"),
            Output("role_select", "value"),

            Input("database-sqlite", "children"),
            Input("selected-group", "data"),
            prevent_initial_call=True,
        )
        def select_first_row(database_sqlite, selected_group):

            if not selected_group:
                return [], '', False, '', '', ''

            user_group = selected_group[0]

            users = db_connection.get_group_users(database_sqlite, user_group['user_group'])

            users_out = []
            for user in users:
                properties = json.loads(user.get('properties',{}))
                user_out = user.copy()
                user_out.update({'name':properties.get('name',''), 'type':properties.get('type','')})
                users_out.append(user_out)

            return users_out, '', '', '', 'TRADER'

        @self.app.callback(
            Output("user_name_input", "value", allow_duplicate=True),
            Output("name_input", "value", allow_duplicate=True),
            Output("password_input", "value", allow_duplicate=True),
            Output("role_select", "value", allow_duplicate=True),
            Input("users_grid_id", "selectedRows"),
            prevent_initial_call=True,
        )
        def populate_user_inputs(selected_rows):

            if not selected_rows:
                return "", "", "", "TRADER"

            user = selected_rows[0]

            return user.get("user_name", ""), user.get("name", ""), "", user.get("type", "TRADER"),

        @self.app.callback(
            Output("user_name_input", "value",  allow_duplicate=True, ),
            Output("user_error", "children", allow_duplicate=True, ),
            Output("selected-group", "data", allow_duplicate=True,),
            Input("user_add", "n_clicks", ),
            State("user_name_input", "value", ),
            State("selected-group", "data"),
            State("name_input", "value", ),
            State("password_input", "value", ),
            State("role_select", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def add_user(add_clicks, user_name_input, selected_group, name_input, password_input, role_select, database_sqlite):

            user_name_input = (user_name_input or "").strip().upper()

            logger.info(f"adding user: {user_name_input}")

            if not user_name_input:
                return user_name_input, "Username is required.", selected_group

            user_props = {'name': name_input, 'type': role_select, 'password':password_input}
            db_connection.insert_user(database_sqlite, user_name_input,
                                      selected_group[0]['user_group'], user_props)

            return user_name_input, "", selected_group

        @self.app.callback(
            Output("user_error", "children", allow_duplicate=True, ),
            Output("database-sqlite", "children", allow_duplicate=True, ),
            Input("user_remove", "n_clicks", ),
            State("user_name_input", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def user_remove(user_clicks, user_name_input, database_sqlite):

            user_name_input = (user_name_input or "").strip().upper()

            logger.info(f"deleting user: {user_name_input}")

            if not user_name_input:
                return "User Name Name is required.", database_sqlite

            db_connection.delete_user(database_sqlite, user_name_input)

            return "", database_sqlite
