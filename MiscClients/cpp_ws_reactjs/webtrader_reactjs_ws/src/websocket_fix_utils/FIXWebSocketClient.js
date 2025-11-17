// © Mike Kipnis - DistributedATS
// WebSocket FIX Client — reconnecting + heartbeat

export class FIXWebSocketClient {
  constructor(url, handler = null) {
    this.url = url;
    this.ws = null;

    this.handler = handler; // handler will send FIX messages

    this.forcedClose = false;
    this.reconnectAttempts = 0;
    this.reconnectDelayBase = 1000;

    this.heartbeatInterval = 30000;
    this.heartbeatTimer = null;
    this.reconnectTimer = null;

    // Callbacks
    this.onopen = null;
    this.onmessage = null;
    this.onerror = null;
    this.onclose = null;
  }

  get state() {
    if (!this.ws) return "CLOSED";
    const rs = this.ws.readyState;
    return rs === WebSocket.CONNECTING
      ? "CONNECTING"
      : rs === WebSocket.OPEN
      ? "OPEN"
      : rs === WebSocket.CLOSING
      ? "CLOSING"
      : "CLOSED";
  }

  connect() {
    this.tryConnect();
  }

  tryConnect() {
    if (this.ws && (this.ws.readyState === WebSocket.OPEN || this.ws.readyState === WebSocket.CONNECTING))
      return;

    try {
      this.ws = new WebSocket(this.url);
    } catch (err) {
      console.error("WS init failed:", err);
      return;
    }

    this.ws.onopen = evt => {
      this.reconnectAttempts = 0;
      // this.startHeartbeat(); - Don't heartbeat, rely on the socker state

      // Send FIX Logon automatically
      if (this.handler && typeof this.handler.sendLogon === "function") {
        this.handler.sendLogon(this);
      }

      if (this.onopen) this.onopen(evt);
    };

    this.ws.onmessage = evt => {
      try {
        const obj = JSON.parse(evt.data);
        if (this.onmessage) this.onmessage(obj);
      } catch (e) {
        console.error("Invalid WS message:", evt.data);
      }
    };

    this.ws.onerror = evt => {
      console.error("FIX WS error", evt);
      if (this.onerror) this.onerror(evt);
    };

    this.ws.onclose = evt => {
      this.stopHeartbeat();
      if (this.onclose) this.onclose(evt);
      if (!this.forcedClose) this.scheduleReconnect();
    };
  }

  disconnect() {
    this.forcedClose = true;
    this.stopHeartbeat();
    if (this.ws) this.ws.close();
  }

  scheduleReconnect() {
    this.reconnectAttempts++;
    const delay = Math.min(
      this.reconnectDelayBase * 2 ** (this.reconnectAttempts - 1),
      30000
    );

    console.warn(`Reconnecting in ${delay}ms...`);
    this.reconnectTimer = setTimeout(() => this.tryConnect(), delay);
  }

  startHeartbeat() {
    this.stopHeartbeat();
    this.heartbeatTimer = setInterval(() => {
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        this.sendJSON({ heartbeat: true });
      }
    }, this.heartbeatInterval);
  }

  stopHeartbeat() {
    if (this.heartbeatTimer) clearInterval(this.heartbeatTimer);
    this.heartbeatTimer = null;
  }

  sendJSON(obj) {
    if (this.ws && this.ws.readyState === WebSocket.OPEN) {
      this.ws.send(JSON.stringify(obj));
    }
    console.warn("WS send skipped — socket not OPEN");

    return obj;
  }
}
