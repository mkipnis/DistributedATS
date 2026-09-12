# Copyright (c) Mike Kipnis - DistributedATS

import datetime

import dash
from dash import Input, Output, html, dcc, State
from dash import callback_context
import dash_bootstrap_components as dbc

class TicketPanel(object):

    def __init__(self, app: dash.Dash):
        self.app = app
        self._register_callbacks()

    def layout(self):
        return dbc.Container(
            [
            ],
            fluid=True,
        )

    def _register_callbacks(self):
        pass