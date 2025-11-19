// DistributedATS - Mike Kipnis (c) 2022
import "./App.css";
import "bootstrap/dist/css/bootstrap.min.css";

import React, {
  useEffect,
  useState,
  useRef,
  useCallback,
} from "react";

import { Container, Row, Col } from "react-bootstrap/";

import Login from "./components/Login";
import PositionsAndMarketData from "./components/PositionsAndMarketData";
import Ticket from "./components/Ticket";
import History from "./components/History";

import { FIXWebSocketClient } from "./websocket_fix_utils/FIXWebSocketClient";
import { FIXMessageHandler } from "./websocket_fix_utils/FIXMessageHandler";

import { DataMan } from "./data_man/DataMan";

function App() {
  const ticketRef = useRef();
  const histRef = useRef();
  const marketDataAndPositionsRef = useRef();

  const fixSessionHandler = useRef(null);
  const fixClient = useRef(null);
  const dataMan = useRef(null);

  const [sessionToken, setSessionToken] = useState(null);

  const [loginState, setLoginState] = useState({
    sessionStateCode: 0,
    text: "Please login",
  });

  const [blotterData, setBlotterData] = useState();
  const [selectedInstrument, setSelectedInstrument] = useState();
  const [lastExecReport, setLastExecReport] = useState();
  const [selectedInstrumentBlotterData, setSelectedInstrumentBlotterData] =
    useState();

  const [histData, setHistData] = useState([]);

  function getWebSocketUrl() {
   const host = window.location.hostname; // localhost, docker container, or prod host
   const port = 9002;                     // your FIX WS port
   const protocol = window.location.protocol === "https:" ? "wss" : "ws";
   return `${protocol}://${host}:${port}`;
  }


  // =====================================================================================
  // FIX LOGON CALLBACK  (USER PRESSES LOGIN BUTTON)
  // =====================================================================================
  const Logon_callback = useCallback((logonValue) => {

    if (!fixClient.current) {
      const wsUrl = getWebSocketUrl();

      // Create FIX handler
      fixSessionHandler.current = new FIXMessageHandler({
        username: logonValue.username,
        password: logonValue.password,
        targetCompID: "FIX_GWY_1",
      });

      // Pass handler into client
      fixClient.current = new FIXWebSocketClient(wsUrl, fixSessionHandler.current);

      dataMan.current = new DataMan(fixSessionHandler.current);

      fixClient.current.onopen = () => {
        console.log("WS opened");
      };
      // -------------------------
      // WS MESSAGE
      // -------------------------
      fixClient.current.onmessage = (msg) => {
        try {
          if (msg?.data_type !== "FIX") return;

          const msgType = msg?.Header?.["35"];

          // -------------------------
          // FIX LOGON (35=A)
          // -------------------------
          if (msgType === "A") {
            console.log("🔐 FIX Logon received");
            setLoginState({ text: "Logon successful" });
            setSessionToken({
              token: msg.session_qualifier,
              username: logonValue.username,
            });
            return;
          } else if ( msgType === "5")
          {
            console.log("🔐 FIX Logout received", msg);
            setLoginState({ text: msg?.Body?.["58"] });
            return;
          }

          // -------------------------
          // ALL OTHER FIX MSGS
          // -------------------------
          const result = dataMan.current.processFIXMessage(
            fixClient.current,
            msg
          );

          if (result !== undefined) {
            setBlotterData(result);
          }

          const lastType = dataMan.current.get_last_msg_type();

          if (lastType === "y") {
            fixSessionHandler.current.sendMassOrderStatusRequest(fixClient.current);
          } else if (lastType === "8") {
            const orderMan = dataMan.current.get_order_man();
            setHistData(orderMan.get_order_states());
          }
        } catch (err) {
          console.error("Failed to parse FIX WS message:", err);
        }
      };

      // -------------------------
      // WS ERROR
      // -------------------------
      fixClient.current.onerror = (err) => {
        console.error("❌ FIX WS error:", err);
        setLoginState({ text: "FIX WebSocket connection failed" });
      };

      // -------------------------
      // WS CLOSE
      // -------------------------
      fixClient.current.onclose = () => {
        console.warn("⚠️ FIX WS closed");
        setLoginState({ text: "Disconnected from FIX WS" });
        setBlotterData(undefined);
      };
    }

    fixClient.current.connect();
  }, []);

  // =====================================================================================
  // UPDATE UI ON BLOTTER CHANGES
  // =====================================================================================
  useEffect(() => {
    if (!blotterData) return;

    if (selectedInstrument) {
      setSelectedInstrumentBlotterData({
        ...blotterData[selectedInstrument],
      });

      setLastExecReport({
        ...blotterData["last_exec_report"],
      });
    }

    if (marketDataAndPositionsRef.current) {
      marketDataAndPositionsRef.current.update_data();
    }
  }, [blotterData, selectedInstrument]);

  // =====================================================================================
  // INSTRUMENT SELECTION CALLBACK
  // =====================================================================================
  const SelectedInstrument = (instrument) => {
    setSelectedInstrument(instrument);
  };

  // =====================================================================================
  // SESSION ESTABLISHED → REQUEST SECURITY LIST
  // =====================================================================================
  useEffect(() => {
    if (!sessionToken || !fixClient.current) return;

    console.log("🔐 FIX session token:", sessionToken);
    fixSessionHandler.current.sendSecurityListRequest(fixClient.current);
  }, [sessionToken]);

  // =====================================================================================
  // CLEANUP
  // =====================================================================================
  useEffect(() => {
    return () => {
      if (fixClient.current) fixClient.current.disconnect();
    };
  }, []);

  // =====================================================================================
  // RENDER UI
  // =====================================================================================
  return (
    <div className="body">
      <div className="ag-theme-balham-dark">
        <nav>
          <Login
            loginState={loginState}
            logonCallback={Logon_callback}
          />
        </nav>

        <Container fluid style={{ marginTop: 20 }}>
          <div
            style={
              blotterData == null
                ? { pointerEvents: "none", opacity: 0.4 }
                : {}
            }
          >
            <Row>
              <Col sm={8}>
                <PositionsAndMarketData
                  blotterData={blotterData}
                  selectedInstrument={SelectedInstrument}
                  ref={marketDataAndPositionsRef}
                />
                <div>Click on an instrument to trade.</div>
              </Col>

              <Col sm={4}>
                <Ticket
                  instrumentName={selectedInstrument}
                  selectedInstrumentBlotterData={
                    selectedInstrumentBlotterData
                  }
                  fixSessionHandler={fixSessionHandler}
                  sendOrder={(msg) => {
                       return fixClient.current.sendJSON(msg);
                     }}

                  sendCancelAll={(msg) => {
                      return fixClient.current.sendJSON(msg);
                    }}

                  ref={ticketRef}
                />
              </Col>
            </Row>

            <div style={{ marginTop: 10 }}>
              <History
                histData={histData}
                blotterData={blotterData}
                fixSessionHandler={fixSessionHandler}
                sendCancelOrder={(msg) => {
                     return fixClient.current.sendJSON(msg);
                   }}
                ref={histRef}
              />
            </div>
          </div>
        </Container>
      </div>
    </div>
  );
}

export default App;
