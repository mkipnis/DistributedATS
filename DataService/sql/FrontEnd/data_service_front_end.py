# Copyright (c) Mike Kipnis - DistributedATS

import argparse
import logging
import os
import shutil

import dash
import dash_bootstrap_components as dbc
from dash import html, dcc, Input, Output

from components import (
    user_group_panel,
    markets_panel,
    user_panel,
    instruments_panel,
    instrument_details_panel,
    user_group_markets_panel,
)

import db_connection

# ----------------------------
# Logging configuration
# ----------------------------
logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
    handlers=[
        logging.StreamHandler(),
        logging.FileHandler("data_service_front_end.log", "a"),
    ],
)

logger = logging.getLogger("DataServiceFrontEnd")


# ----------------------------
# Command-line arguments
# ----------------------------
parser = argparse.ArgumentParser()

parser.add_argument(
    "--database",
    default="distributed_ats.db",
    help="Source SQLite database",
)

args = parser.parse_args()


# ----------------------------
# Database paths
# ----------------------------
original_database = db_connection.get_database_location( args.database )
base, ext = os.path.splitext(original_database)
working_database = f"{base}_working{ext}"

# ----------------------------
# Database management
# ----------------------------
def reset_working_database():
    """
    Recreate the working database from the original database.
    This is called when the browser loads/reloads the application.
    """
    try:
        # Remove the old working database first
        if os.path.exists(working_database):
            os.remove(working_database)

        # Create a fresh working copy
        shutil.copy2( original_database, working_database, )

        logger.info( "Working database reset: %s -> %s", original_database, working_database, )

        return working_database

    except Exception:
        logger.exception("Failed to create working database")
        raise


def save_working_database():
    """
    Replace the original database with the current
    working database.
    """
    try:
        shutil.copy2(working_database, original_database,)

        logger.info("Database saved: %s -> %s", working_database, original_database, )

    except Exception:
        logger.exception("Failed to save database")
        raise


# ----------------------------
# DataServiceFrontEnd class
# ----------------------------
class DataServiceFrontEnd:

    def __init__(self, app: dash.Dash):

        self.app = app
        self.user_groups = user_group_panel.UserGroupPanel(self.app)
        self.users = user_panel.UsersPanel(self.app)
        self.instrument_groups = markets_panel.MarketsPanel(self.app)
        self.instruments = instruments_panel.InstrumentsPanel(self.app)
        self.instrument_details = instrument_details_panel.InstrumentDetailsPanel( self.app)
        self.user_group_markets_panel = user_group_markets_panel.UserGroupMarketsPanel( self.app )

    def layout(self):

        return dbc.Container(
            [

                # ==================================================
                # Navbar
                # ==================================================
                dbc.Navbar(
                    dbc.Container(
                        [

                            # ----------------------------
                            # Left side
                            # ----------------------------
                            dbc.NavbarBrand(
                                "DistributedATS - Data Service Front-End",
                                href="#",
                                style={ "fontSize": "20px" },
                            ),

                            # ----------------------------
                            # Right side
                            # ----------------------------
                            html.Div(
                                [
                                    html.Div(
                                        [
                                            html.Div(
                                                [
                                                    dbc.Label(
                                                        id="database-sqlite",
                                                        className="navbar-label mb-0 me-2",
                                                    ),
                                                    dbc.Button(
                                                        "Save",
                                                        id="save-database",
                                                        color="success",
                                                        size="sm",
                                                        className="py-0 px-2",
                                                        style={
                                                            "fontSize": "15px",
                                                            "height": "22px",
                                                            "lineHeight": "18px",
                                                        },
                                                    ),
                                                ],
                                                className="d-flex align-items-center",
                                            ),

                                            html.Div(
                                                id="status",
                                                className="text-white ms-2",
                                            ),
                                        ],
                                        className="d-flex align-items-center ms-auto",
                                    ),

                                ],
                                className=(
                                    "d-flex flex-column "
                                    "align-items-start ms-auto"
                                ),
                            ),

                        ],
                        fluid=True,
                    ),
                    color="#0f2538",
                    dark=True,
                ),

                # ==================================================
                # Main content
                # ==================================================
                html.Div(
                    [

                        dbc.Accordion(
                            [

                                # ----------------------------
                                # Markets and Instruments
                                # ----------------------------
                                dbc.AccordionItem(
                                    dbc.Row(
                                        [
                                            dbc.Col(
                                                self.instrument_groups.layout()
                                            ),

                                            dbc.Col(
                                                self.instruments.layout()
                                            ),

                                            dbc.Col(
                                                self.instrument_details.layout()
                                            ),
                                        ],
                                        className="p-3",
                                    ),
                                    title="Markets and Instruments",
                                    item_id="institution-stock",
                                ),

                                # ----------------------------
                                # User Groups and Users
                                # ----------------------------
                                dbc.AccordionItem(
                                    dbc.Row(
                                        [
                                            dbc.Col(
                                                self.user_groups.layout()
                                            ),

                                            dbc.Col(
                                                self.users.layout()
                                            ),

                                            dbc.Col(
                                                self.user_group_markets_panel.layout()
                                            ),
                                        ],
                                        className="p-3",
                                    ),
                                    title="User Groups and Users",
                                    item_id="sector-industry",
                                ),

                            ],
                            id="main-accordion",
                            start_collapsed=False,
                            flush=True,
                            always_open=True,
                            active_item=[
                                "sector-industry",
                                "institution-stock",
                            ],
                        ),

                    ],
                    id="accordion-wrapper",
                ),

                # ==================================================
                # Startup trigger
                # ==================================================
                dcc.Interval(id="startup", n_intervals=0, max_intervals=1,),

                # ==================================================
                # Stores
                # ==================================================
                dcc.Store( id="database-name" ),
                dcc.Store( id="user-groups" ),
                dcc.Store( id="selected-group" ),
                dcc.Store( id="markets" ),
                dcc.Store( id="selected-market" ),
                dcc.Store( id="selected-instrument" ),
            ],
            fluid=True,
            className="p-0",
        )


# ==================================================
# Dash App Initialization
# ==================================================
app = dash.Dash(
    __name__,
    title="DistributedATS - Data Service Front-End",
    external_stylesheets=[dbc.themes.DARKLY],
)

server = app.server

# ==================================================
# Initialize Front End
# ==================================================
data_service_front_end = DataServiceFrontEnd(app)

app.layout = data_service_front_end.layout

# ==================================================
# Database Callbacks
# ==================================================

# ----------------------------
# Browser load / reload
# ----------------------------
@app.callback(
    Output("database-sqlite","children",),
    Input("startup","n_intervals",),
)
def set_data_service_data(_):
    # Create a fresh working database.
    #
    # IMPORTANT:
    # This does NOT modify the original database.
    #
    # Browser reload:
    #
    #     original.db
    #          |
    #          v
    #     working.db
    #
    return reset_working_database()


# ----------------------------
# Save working database
# ----------------------------
@app.callback(
    Output("status", "children",),
    Input("save-database","n_clicks",),
    prevent_initial_call=True,
)
def save_database(n_clicks):
    try:
        save_working_database()
        return "Database saved"

    except Exception as e:
        return f"Save failed: {e}"


# ==================================================
# Data Callbacks
# ==================================================

@app.callback(
    Output("user-groups","data",),
    Input("database-sqlite","children",),
)
def get_user_groups_data(database_sqlite):

    if not database_sqlite:
        return []

    return db_connection.get_user_groups(
        database_sqlite
    )


@app.callback(
    Output("markets","data",),
    Input("database-sqlite","children",),
)
def get_markets(database_sqlite):

    if not database_sqlite:
        return []

    return db_connection.get_markets(
        database_sqlite
    )


# ==================================================
# Main
# ==================================================
if __name__ == "__main__":

    host = os.getenv( "DASH_HOST", "127.0.0.1", )
    port = int( os.getenv( "DASH_PORT", "8050", ))

    debug = (os.getenv( "DASH_DEBUG", "true", ).lower() == "true")

    app.run( host=host, port=port, debug=debug, use_reloader=False,)