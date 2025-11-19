// © Mike Kipnis - DistributedATS
// WebSocket FIX Client — heartbeat only, no auto-reconnect

export class FIXWebSocketClient {
  constructor(url, handler = null) {
    this.url = url;
    this.ws = null;

    this.handler = handler; // handler will send FIX messages

    this.heartbeatInterval = 30000;
    this.heartbeatTimer = null;

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
    if (this.ws && (this.ws.readyState === WebSocket.OPEN || this.ws.readyState === WebSocket.CONNECTING)) return;

    try {
      this.ws = new WebSocket(this.url);
    } catch (err) {
      console.error("WS init failed:", err);
      return;
    }

    this.ws.onopen = evt => {
      this.startHeartbeat();

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
      // No auto-reconnect
    };
  }

  disconnect() {
    this.stopHeartbeat();
    if (this.ws) this.ws.close();
  }

  startHeartbeat() {
    this.stopHeartbeat();
    this.heartbeatTimer = setInterval(() => {
      if (this.ws && this.ws.readyState === WebSocket.OPEN) {
        if (this.handler && typeof this.handler.sendHeartbeat === "function") {
          this.handler.sendHeartbeat(this);
        }
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
    } else {
      console.warn("WS send skipped — socket not OPEN");
    }
    return obj;
  }
}
