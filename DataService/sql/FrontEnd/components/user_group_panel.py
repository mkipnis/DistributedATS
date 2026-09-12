# Copyright (c) Mike Kipnis - DistributedATS
import logging
from datetime import datetime

import dash_ag_grid as dag
from dash import html, Input, Output, State, dash, ctx
import dash_bootstrap_components as dbc

import db_connection

logger = logging.getLogger("UserGroupPanel")


class UserGroupPanel(object):

    def __init__(self, app: dash.Dash):

        self.app = app

        column_defs = [
            {
                "field": "user_group",
                "headerName": "User Group",
                "flex": 2,
                "editable": True,
            },
            {
                "field": "last_update_time",
                "headerName": "Last Update Time",
                "flex": 2,
                "editable": False,
            },
        ]

        self.user_group_grid = dag.AgGrid(
            id="user_group_grid_id",
            columnDefs=column_defs,
            rowData=[],

            defaultColDef={
                "minWidth": 100,
                "resizable": True,
            },

            style={
                "height": "400px",
                "width": "100%",
            },

            dashGridOptions={
                "rowSelection": "single",
                "animateRows": True,
                "suppressMaintainUnsortedOrder": True,
                "doubleClickEdit": True,
            },
            className="ag-theme-quartz",
        )

        self._register_callbacks()

    def layout(self):

        return html.Div(
            children=[
                # -------------------------------------------------
                # Grid
                # -------------------------------------------------

                self.user_group_grid,

                # -------------------------------------------------
                # Input fields
                # -------------------------------------------------
                html.Div(
                    children=[
                        dbc.Row(
                            [
                                dbc.Col(
                                    [
                                        html.Label( "User Group", style={ "fontWeight": "bold", "marginBottom": "4px", }, ),
                                        dbc.Input( id="user_group_input", type="text", placeholder="Enter user group", size="sm", style={ "width": "300px", }, ),
                                    ],
                                    width="auto",
                                ),

                                dbc.Col(
                                    [
                                        html.Button( "Add", id="user_group_add", className="btn btn-dark btn-sm me-2", ),
                                        html.Button( "Remove", id="user_group_remove", className="btn btn-dark btn-sm", ),
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
                html.Div(id="user_group_error", style={ "color": "#dc3545", "marginTop": "5px", },),
            ],

            **{
                "data-ag-theme-mode": "dark"
            }
        )

    def _register_callbacks(self):

        # ---------------------------------------------------------
        # Load initial data
        # ---------------------------------------------------------

        @self.app.callback(
            Output("user_group_grid_id", "rowData"),
            Input("user-groups", "data"),
            prevent_initial_call=True,
        )
        def setup_entity_grid(user_groups):

            if not user_groups:
                return []

            return user_groups

        # ---------------------------------------------------------
        # Add row
        # ---------------------------------------------------------
        @self.app.callback(
            Output("user_group_error", "children", allow_duplicate=True, ),
            Output("database-sqlite", "children", allow_duplicate=True, ),
            Input("user_group_add","n_clicks",),
            State("user_group_input", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def add_user_group(add_clicks,user_group,database_sqlite):

            user_group = (user_group or "").strip()

            logger.info(f"add_user_group: {user_group}")

            if not user_group:
                return "User Group is required.", database_sqlite

            db_connection.insert_user_group(database_sqlite, user_group)

            return "", database_sqlite

        # ---------------------------------------------------------
        # Remove row
        # ---------------------------------------------------------

        @self.app.callback(
            Output("user_group_error", "children", allow_duplicate=True, ),
            Output("database-sqlite", "children", allow_duplicate=True, ),
            Input("user_group_remove","n_clicks",),
            State("user_group_input", "value", ),
            State("database-sqlite", "children"),
            prevent_initial_call=True,
        )
        def remove_row(remove_clicks,user_group, database_sqlite,):

            user_group = (user_group or "").strip().upper()

            logger.info(f"remove_user_group: {user_group}")

            if not user_group:
                return "User Group Name is required.", database_sqlite

            db_connection.remove_user_group(database_sqlite, user_group)

            return "", database_sqlite

        # ---------------------------------------------------------
        # Automatically select first row
        # ---------------------------------------------------------

        @self.app.callback(
            Output("user_group_grid_id","selectedRows",),
            Input("user_group_grid_id","rowData",),
            prevent_initial_call=True,
        )
        def select_first_row(row_data):

            if not row_data:
                return []

            return [row_data[0]]

        # ---------------------------------------------------------
        # Publish selected group
        # ---------------------------------------------------------
        @self.app.callback(
            Output("selected-group","data",),
            Output("user_group_input", "value", allow_duplicate=True, ),
            Input("user_group_grid_id","selectedRows",),
            prevent_initial_call=True,
        )
        def select_group(selected_rows):

            if not selected_rows:
                return []

            return [selected_rows[0]], selected_rows[0]['user_group']