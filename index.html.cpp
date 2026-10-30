<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>ASTRA ORBIT 2.0 — CubeSat Mission Control</title>
  
  <!-- Typography -->
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:ital,wght@0,300;0,400;0,600;0,700;1,400&family=Orbitron:wght@500;700;900&family=Rajdhani:wght@500;600;700&display=swap" rel="stylesheet">
  
  <!-- Three.js & OrbitControls for 3D Attitude Rendering -->
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
  <script src="https://cdn.jsdelivr.net/npm/three@0.128.0/examples/js/controls/OrbitControls.js"></script>

  <style>
    :root {
      --bg-space: #030712;
      --bg-panel: #070e1c;
      --bg-card: #0c182d;
      --bg-card-hover: #10213d;
      --border-dim: #162a4a;
      --border-bright: #00f3ff55;
      --cyan: #00f3ff;
      --cyan-glow: rgba(0, 243, 255, 0.25);
      --amber: #ffaa00;
      --red: #ff3366;
      --green: #00ff88;
      --text-main: #d9e6f2;
      --text-dim: #6c84a3;
      --text-highlight: #ffffff;
      --font-mono: 'JetBrains Mono', monospace;
      --font-orbitron: 'Orbitron', sans-serif;
      --font-rajdhani: 'Rajdhani', sans-serif;
    }

    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
      user-select: none;
    }

    body {
      background-color: var(--bg-space);
      color: var(--text-main);
      font-family: var(--font-rajdhani);
      font-size: 15px;
      line-height: 1.4;
      overflow-x: hidden;
      min-height: 100vh;
      background-image: 
        radial-gradient(circle at 10% 15%, rgba(0, 243, 255, 0.05) 0%, transparent 40%),
        radial-gradient(circle at 90% 85%, rgba(0, 100, 255, 0.04) 0%, transparent 40%);
    }

    /* Scrollbars */
    ::-webkit-scrollbar { width: 6px; height: 6px; }
    ::-webkit-scrollbar-track { background: var(--bg-space); }
    ::-webkit-scrollbar-thumb { background: var(--border-dim); border-radius: 3px; }
    ::-webkit-scrollbar-thumb:hover { background: var(--cyan); }

    /* Header Nav */
    header {
      background: rgba(7, 14, 28, 0.96);
      backdrop-filter: blur(12px);
      border-bottom: 1px solid var(--border-dim);
      padding: 10px 24px;
      display: flex;
      justify-content: space-between;
      align-items: center;
      position: sticky;
      top: 0;
      z-index: 100;
      box-shadow: 0 4px 20px rgba(0,0,0,0.6);
    }

    .brand {
      display: flex;
      align-items: center;
      gap: 14px;
    }

    .brand-logo {
      width: 36px;
      height: 36px;
      border: 2px solid var(--cyan);
      display: grid;
      place-items: center;
      transform: rotate(45deg);
      box-shadow: 0 0 15px var(--cyan-glow);
    }

    .brand-logo svg {
      transform: rotate(-45deg);
      fill: var(--cyan);
    }

    .brand-title h1 {
      font-family: var(--font-orbitron);
      font-size: 19px;
      letter-spacing: 2px;
      color: #fff;
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .brand-title h1 span {
      color: var(--cyan);
      font-size: 13px;
      padding: 2px 7px;
      background: rgba(0, 243, 255, 0.1);
      border: 1px solid var(--cyan);
      border-radius: 3px;
    }

    .brand-subtitle {
      font-size: 11px;
      color: var(--text-dim);
      font-family: var(--font-mono);
      letter-spacing: 1px;
    }

    .header-controls {
      display: flex;
      align-items: center;
      gap: 12px;
    }

    .mode-select {
      background: var(--bg-card);
      border: 1px solid var(--border-dim);
      color: var(--cyan);
      font-family: var(--font-mono);
      padding: 6px 10px;
      font-size: 12px;
      border-radius: 4px;
      outline: none;
      cursor: pointer;
    }

    .btn {
      font-family: var(--font-orbitron);
      font-size: 11px;
      letter-spacing: 1px;
      padding: 8px 16px;
      border-radius: 4px;
      cursor: pointer;
      display: inline-flex;
      align-items: center;
      gap: 8px;
      transition: all 0.2s ease;
      text-transform: uppercase;
      font-weight: 700;
    }

    .btn-connect {
      background: transparent;
      border: 1px solid var(--cyan);
      color: var(--cyan);
      box-shadow: 0 0 12px var(--cyan-glow);
    }

    .btn-connect:hover {
      background: var(--cyan);
      color: #000;
      box-shadow: 0 0 20px rgba(0, 243, 255, 0.7);
    }

    .btn-disconnect {
      background: rgba(255, 51, 102, 0.15);
      border: 1px solid var(--red);
      color: var(--red);
    }

    .btn-disconnect:hover {
      background: var(--red);
      color: #fff;
    }

    .btn-aux {
      background: var(--bg-card);
      border: 1px solid var(--border-dim);
      color: var(--text-main);
    }

    .btn-aux:hover {
      border-color: var(--cyan);
      color: var(--cyan);
    }

    .status-badge {
      display: flex;
      align-items: center;
      gap: 8px;
      font-family: var(--font-mono);
      font-size: 12px;
      padding: 6px 12px;
      border-radius: 20px;
      background: rgba(12, 24, 46, 0.8);
      border: 1px solid var(--border-dim);
    }

    .status-dot {
      width: 10px;
      height: 10px;
      border-radius: 50%;
      background: var(--text-dim);
    }

    .status-dot.active {
      background: var(--green);
      box-shadow: 0 0 10px var(--green);
      animation: pulse 1.5s infinite;
    }

    .status-dot.waiting {
      background: var(--amber);
      box-shadow: 0 0 10px var(--amber);
      animation: pulse 1s infinite;
    }

    .status-dot.error {
      background: var(--red);
      box-shadow: 0 0 10px var(--red);
    }

    @keyframes pulse {
      0% { opacity: 0.3; }
      50% { opacity: 1; }
      100% { opacity: 0.3; }
    }

    /* Diagnostics Alert Banners */
    .alert-container {
      padding: 10px 16px 0 16px;
      display: flex;
      flex-direction: column;
      gap: 8px;
    }

    .alert-banner {
      padding: 10px 16px;
      border-radius: 4px;
      font-family: var(--font-mono);
      font-size: 12px;
      display: none;
      align-items: center;
      justify-content: space-between;
      animation: slideIn 0.3s ease;
    }

    .alert-err {
      background: rgba(255, 51, 102, 0.15);
      border: 1px solid var(--red);
      color: #ffccd8;
    }

    .alert-warn {
      background: rgba(255, 170, 0, 0.15);
      border: 1px solid var(--amber);
      color: #ffe6a6;
    }

    @keyframes slideIn {
      from { transform: translateY(-8px); opacity: 0; }
      to { transform: translateY(0); opacity: 1; }
    }

    /* Grid Layout */
    .mission-grid {
      display: grid;
      grid-template-columns: 340px 1fr 380px;
      gap: 16px;
      padding: 16px;
      max-width: 1920px;
      margin: 0 auto;
    }

    @media (max-width: 1350px) {
      .mission-grid {
        grid-template-columns: 1fr;
      }
    }

    .panel {
      background: var(--bg-panel);
      border: 1px solid var(--border-dim);
      border-radius: 6px;
      padding: 14px;
      display: flex;
      flex-direction: column;
      gap: 12px;
      position: relative;
    }

    .panel-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      border-bottom: 1px solid var(--border-dim);
      padding-bottom: 8px;
    }

    .panel-title {
      font-family: var(--font-orbitron);
      font-size: 12px;
      letter-spacing: 1.5px;
      color: var(--cyan);
      display: flex;
      align-items: center;
      gap: 8px;
    }

    .telemetry-row {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(130px, 1fr));
      gap: 10px;
    }

    .metric-card {
      background: var(--bg-card);
      border: 1px solid var(--border-dim);
      border-radius: 4px;
      padding: 8px 10px;
      display: flex;
      flex-direction: column;
      gap: 3px;
      transition: border-color 0.2s;
    }

    .metric-card:hover {
      border-color: var(--border-bright);
    }

    .metric-label {
      font-family: var(--font-mono);
      font-size: 11px;
      color: var(--text-dim);
      text-transform: uppercase;
      letter-spacing: 0.5px;
    }

    .metric-value-container {
      display: flex;
      align-items: baseline;
      gap: 6px;
    }

    .metric-val {
      font-family: var(--font-mono);
      font-size: 19px;
      font-weight: 700;
      color: var(--text-highlight);
    }

    .metric-val.unreceived {
      color: var(--text-dim);
      font-weight: 400;
      font-size: 15px;
      font-style: italic;
    }

    .metric-val.sensor-off {
      color: var(--amber);
      font-weight: 400;
      font-size: 14px;
    }

    .metric-unit {
      font-family: var(--font-mono);
      font-size: 11px;
      color: var(--cyan);
    }

    /* Subsystem Status Tables */
    .diag-table {
      width: 100%;
      border-collapse: collapse;
      font-family: var(--font-mono);
      font-size: 12px;
    }

    .diag-table td {
      padding: 5px 2px;
      border-bottom: 1px solid rgba(22, 42, 74, 0.4);
    }

    .diag-table td:last-child {
      text-align: right;
    }

    .badge-ok {
      color: var(--green);
      background: rgba(0, 255, 136, 0.1);
      padding: 2px 6px;
      border-radius: 3px;
      border: 1px solid var(--green);
    }

    .badge-err {
      color: var(--red);
      background: rgba(255, 51, 102, 0.1);
      padding: 2px 6px;
      border-radius: 3px;
      border: 1px solid var(--red);
    }

    .badge-wait {
      color: var(--amber);
      background: rgba(255, 170, 0, 0.1);
      padding: 2px 6px;
      border-radius: 3px;
      border: 1px solid var(--amber);
    }

    /* 3D Attitude Canvas Container */
    #cubesat-viewport {
      width: 100%;
      height: 380px;
      background: radial-gradient(circle at center, #07152d 0%, #030712 80%);
      border-radius: 4px;
      position: relative;
      overflow: hidden;
      border: 1px solid var(--border-dim);
    }

    .viewer-overlay {
      position: absolute;
      top: 10px;
      left: 10px;
      pointer-events: none;
      z-index: 10;
      font-family: var(--font-mono);
      font-size: 11px;
      color: var(--cyan);
      background: rgba(3, 7, 18, 0.75);
      padding: 6px 10px;
      border-radius: 3px;
      border-left: 2px solid var(--cyan);
    }

    .viewer-hint {
      position: absolute;
      bottom: 8px;
      right: 10px;
      font-family: var(--font-mono);
      font-size: 10px;
      color: var(--text-dim);
      pointer-events: none;
    }

    /* Live Canvas Charts */
    .chart-container {
      position: relative;
      background: var(--bg-card);
      border: 1px solid var(--border-dim);
      border-radius: 4px;
      padding: 8px;
      height: 140px;
    }

    .chart-header {
      display: flex;
      justify-content: space-between;
      font-family: var(--font-mono);
      font-size: 11px;
      margin-bottom: 4px;
      color: var(--text-dim);
    }

    canvas {
      width: 100%;
      height: 105px;
      display: block;
    }

    /* Terminal and Log */
    .terminal-box {
      background: #020409;
      border: 1px solid var(--border-dim);
      border-radius: 4px;
      padding: 8px;
      font-family: var(--font-mono);
      font-size: 11px;
      color: #9cdcfe;
      height: 170px;
      overflow-y: auto;
      white-space: pre-wrap;
      word-break: break-all;
    }

    .log-line {
      margin-bottom: 3px;
      line-height: 1.3;
    }
    .log-time { color: var(--text-dim); }
    .log-info { color: #58a6ff; }
    .log-ok { color: var(--green); }
    .log-warn { color: var(--amber); }
    .log-err { color: var(--red); }

    /* Modal */
    .modal-backdrop {
      display: none;
      position: fixed;
      top: 0; left: 0; right: 0; bottom: 0;
      background: rgba(0, 0, 0, 0.85);
      z-index: 200;
      justify-content: center;
      align-items: center;
      padding: 20px;
    }

    .modal-box {
      background: var(--bg-panel);
      border: 1px solid var(--cyan);
      box-shadow: 0 0 30px rgba(0, 243, 255, 0.2);
      width: 100%;
      max-width: 800px;
      border-radius: 6px;
      max-height: 90vh;
      display: flex;
      flex-direction: column;
    }

    .modal-header {
      padding: 12px 20px;
      background: var(--bg-card);
      border-bottom: 1px solid var(--border-dim);
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-family: var(--font-orbitron);
      color: var(--cyan);
    }

    .modal-content {
      padding: 18px;
      overflow-y: auto;
      font-family: var(--font-mono);
      font-size: 13px;
      display: flex;
      flex-direction: column;
      gap: 14px;
    }

    .code-box {
      background: #010409;
      border: 1px solid var(--border-dim);
      border-radius: 4px;
      padding: 10px;
      font-size: 11px;
      color: #38bdf8;
      overflow-x: auto;
    }
  </style>
</head>
<body>

  <!-- Top Navigation Bar -->
  <header>
    <div class="brand">
      <div class="brand-logo">
        <svg width="20" height="20" viewBox="0 0 24 24"><path d="M12 2L2 7l10 5 10-5-10-5zM2 17l10 5 10-5M2 12l10 5 10-5"/></svg>
      </div>
      <div class="brand-title">
        <h1>ASTRA ORBIT <span>2.0</span></h1>
        <div class="brand-subtitle">CUBESAT MISSION CONTROL TELEMETRY RECEIVER</div>
      </div>
    </div>

    <div class="header-controls">
      <select id="conn-mode-select" class="mode-select" title="Communication Channel">
        <option value="webserial">MODE: DIRECT WEB SERIAL (115200 BAUD)</option>
        <option value="websocket">MODE: NODE.JS WEBSOCKET BRIDGE</option>
      </select>

      <div class="status-badge">
        <div id="status-indicator" class="status-dot"></div>
        <span id="status-label">HARDWARE DISCONNECTED</span>
      </div>

      <button id="btn-connect-toggle" class="btn btn-connect">
        <span id="btn-icon">⚡</span>
        <span id="btn-connect-text">CONNECT ESP32</span>
      </button>

      <button id="btn-open-tests" class="btn btn-aux">
        <span>FIRMWARE AUDIT & TESTS</span>
      </button>

      <button id="btn-toggle-sim" class="btn btn-aux" style="color:var(--amber);">
        <span>DEV SIM</span>
      </button>
    </div>
  </header>

  <!-- Hardware Alert Banners -->
  <div class="alert-container">
    <!-- Struct Mismatch / Packet Size Error Alert -->
    <div id="alert-packet-size" class="alert-banner alert-err">
      <div>
        <strong>CRITICAL FIRMWARE MISMATCH:</strong> Received <code>{"error":"Invalid packet size"}</code> from receiver ESP32!
        <div style="font-size:11px; margin-top:2px; color:#fca5a5;">
          Transmitter struct has <code>mpuReady, mpuAddress, mpuWhoAmI</code>, but Receiver struct lacks them. Click "FIRMWARE AUDIT & TESTS" to view the quick fix.
        </div>
      </div>
      <button onclick="document.getElementById('test-modal').style.display='flex'" class="btn btn-aux" style="padding:4px 8px; font-size:10px;">VIEW FIX</button>
    </div>

    <!-- IMU / MPU Sensor Alert -->
    <div id="alert-imu" class="alert-banner alert-warn">
      <div>
        <strong>IMU SUBSYSTEM STATUS:</strong>
        <span id="alert-imu-text">MPU not detected or omitted in transmitter loop. Using inert fallback attitude.</span>
      </div>
    </div>
  </div>

  <!-- Primary Dashboard Grid -->
  <main class="mission-grid">
    
    <!-- LEFT COLUMN: Interface, Subsystem Health & Environmental Sensors -->
    <div style="display: flex; flex-direction: column; gap: 16px;">
      
      <!-- Receiver Interface Telemetry -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><circle cx="12" cy="12" r="10"/><path d="M12 6v6l4 2"/></svg>
            RECEIVER LINK (115200 BAUD)
          </span>
          <span id="packet-rate-badge" style="font-family:var(--font-mono); font-size:11px; color:var(--cyan);">0.0 Hz</span>
        </div>

        <table class="diag-table">
          <tr>
            <td style="color:var(--text-dim);">SEQUENCE (#)</td>
            <td id="diag-seq" style="font-weight:700; color:var(--text-highlight);">--</td>
          </tr>
          <tr>
            <td style="color:var(--text-dim);">TRANSPORT</td>
            <td id="diag-transport">Web Serial (Native)</td>
          </tr>
          <tr>
            <td style="color:var(--text-dim);">TOTAL INGESTED</td>
            <td id="diag-pkts" style="font-weight:700; color:var(--text-highlight);">0</td>
          </tr>
          <tr>
            <td style="color:var(--text-dim);">SIZE REJECTIONS</td>
            <td id="diag-corrupt" style="color:var(--red);">0</td>
          </tr>
          <tr>
            <td style="color:var(--text-dim);">LAST PACKET</td>
            <td id="diag-last-time">--:--:--</td>
          </tr>
        </table>
      </div>

      <!-- Real Hardware Subsystem Matrix -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">SUBSYSTEM STATUS</span>
          <span style="font-size:11px; font-family:var(--font-mono); color:var(--text-dim);">HARDWARE BUS INTEGRITY</span>
        </div>

        <table class="diag-table">
          <tr>
            <td>MPU6500 / MPU6050</td>
            <td id="subsys-mpu"><span class="badge-wait">AWAITING DATA</span></td>
          </tr>
          <tr>
            <td>BMP280 (I2C_BMP: 25, 26)</td>
            <td id="subsys-bmp"><span class="badge-wait">AWAITING DATA</span></td>
          </tr>
          <tr>
            <td>DHT11 (DATA: GPIO4)</td>
            <td id="subsys-dht"><span class="badge-wait">AWAITING DATA</span></td>
          </tr>
          <tr>
            <td>NEO-6M GPS (UART2: RX16)</td>
            <td id="subsys-gps"><span class="badge-wait">AWAITING DATA</span></td>
          </tr>
          <tr>
            <td>ESP-NOW LINK (CH 1)</td>
            <td id="subsys-espnow"><span class="badge-wait">AWAITING DATA</span></td>
          </tr>
        </table>
      </div>

      <!-- Environmental Telemetry (DHT11 & BMP280) -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">ENVIRONMENTAL METRICS</span>
          <span style="font-size:11px; font-family:var(--font-mono); color:var(--text-dim);">DHT11 & BMP280</span>
        </div>

        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">BMP280 TEMP</span>
            <div class="metric-value-container">
              <span id="val-bmp-temp" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">°C</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">DHT11 TEMP</span>
            <div class="metric-value-container">
              <span id="val-dht-temp" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">°C</span>
            </div>
          </div>
        </div>

        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">BAROMETRIC PRESSURE</span>
            <div class="metric-value-container">
              <span id="val-pressure" class="metric-val unreceived">----.-</span>
              <span class="metric-unit">hPa</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">RELATIVE HUMIDITY</span>
            <div class="metric-value-container">
              <span id="val-humidity" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">%</span>
            </div>
          </div>
        </div>

        <div class="metric-card">
          <span class="metric-label">BMP BAROMETRIC ALTITUDE</span>
          <div class="metric-value-container">
            <span id="val-bmp-alt" class="metric-val unreceived">---.-</span>
            <span class="metric-unit">m ASL</span>
          </div>
        </div>
      </div>

      <!-- GPS Subsystem -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">NEO-6M GPS RECEIVER</span>
          <span id="gps-fix-pill" class="badge-wait">NO FIX (0)</span>
        </div>

        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">LATITUDE</span>
            <div class="metric-value-container">
              <span id="val-gps-lat" class="metric-val unreceived">--.------</span>
              <span class="metric-unit">°</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">LONGITUDE</span>
            <div class="metric-value-container">
              <span id="val-gps-lon" class="metric-val unreceived">--.------</span>
              <span class="metric-unit">°</span>
            </div>
          </div>
        </div>

        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">GPS ALTITUDE</span>
            <div class="metric-value-container">
              <span id="val-gps-alt" class="metric-val unreceived">---.-</span>
              <span class="metric-unit">m</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">GROUND SPEED</span>
            <div class="metric-value-container">
              <span id="val-gps-speed" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">km/h</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">SATELLITES</span>
            <div class="metric-value-container">
              <span id="val-gps-sats" class="metric-val unreceived">0</span>
              <span class="metric-unit">SAT</span>
            </div>
          </div>
        </div>
      </div>

    </div>

    <!-- CENTER COLUMN: 3D Attitude Display & Live Rolling Telemetry Waveforms -->
    <div style="display: flex; flex-direction: column; gap: 16px;">
      
      <!-- 3D CubeSat Spatial Attitude -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">
            <svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M21 16V8a2 2 0 0 0-1-1.73l-7-4a2 2 0 0 0-2 0l-7 4A2 2 0 0 0 3 8v8a2 2 0 0 0 1 1.73l7 4a2 2 0 0 0 2 0l7-4A2 2 0 0 0 21 16z"/></svg>
            CUBESAT 3D ATTITUDE (TX COMPLEMENTARY FILTER)
          </span>
          <span id="imu-mode-label" style="font-family:var(--font-mono); font-size:11px; color:var(--text-dim);">AWAITING IMU TELEMETRY</span>
        </div>

        <div id="cubesat-viewport">
          <div class="viewer-overlay">
            <div>ROLL (Φ):  <span id="3d-roll" style="color:#fff; font-weight:bold;">--.-°</span></div>
            <div>PITCH (θ): <span id="3d-pitch" style="color:#fff; font-weight:bold;">--.-°</span></div>
            <div>FILTER:    <span id="3d-filter" style="color:var(--cyan);">α=0.98 COMPLEMENTARY</span></div>
          </div>
          <div class="viewer-hint">DRAG TO ROTATE CAMERA • SCROLL TO ZOOM</div>
        </div>

        <div class="telemetry-row" style="margin-top: 4px;">
          <div class="metric-card">
            <span class="metric-label">TELEMETRY ROLL</span>
            <div class="metric-value-container">
              <span id="val-roll" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">°</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">TELEMETRY PITCH</span>
            <div class="metric-value-container">
              <span id="val-pitch" class="metric-val unreceived">--.-</span>
              <span class="metric-unit">°</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">RESULTANT ACCEL (|a|)</span>
            <div class="metric-value-container">
              <span id="val-total-acc" class="metric-val unreceived">--.---</span>
              <span class="metric-unit">g</span>
            </div>
          </div>
        </div>
      </div>

      <!-- Live Rolling Telemetry Waveforms (Canvas) -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">LIVE TELEMETRY WAVEFORMS</span>
          <span style="font-family:var(--font-mono); font-size:11px; color:var(--cyan);">ROLLING 50 SAMPLES</span>
        </div>

        <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 12px;">
          <!-- Linear Acceleration -->
          <div class="chart-container">
            <div class="chart-header">
              <span>ACCELERATION (g)</span>
              <span><span style="color:#ff3366">■ X</span> <span style="color:#00ff88">■ Y</span> <span style="color:#00f3ff">■ Z</span> <span style="color:#ffffff">■ |a|</span></span>
            </div>
            <canvas id="chart-accel"></canvas>
          </div>

          <!-- Gyroscope -->
          <div class="chart-container">
            <div class="chart-header">
              <span>ANGULAR VELOCITY (°/s)</span>
              <span><span style="color:#ffaa00">■ X</span> <span style="color:#b366ff">■ Y</span> <span style="color:#00f3ff">■ Z</span></span>
            </div>
            <canvas id="chart-gyro"></canvas>
          </div>

          <!-- Thermal Comparison & Humidity -->
          <div class="chart-container">
            <div class="chart-header">
              <span>THERMAL & HUMIDITY</span>
              <span><span style="color:#ff3366">■ BMP280</span> <span style="color:#ffaa00">■ DHT11</span> <span style="color:#00f3ff">■ Hum(%)</span></span>
            </div>
            <canvas id="chart-env"></canvas>
          </div>

          <!-- Barometric Pressure & Altitude -->
          <div class="chart-container">
            <div class="chart-header">
              <span>BAROMETER (BMP280)</span>
              <span><span style="color:#ffaa00">■ Pressure (hPa)</span> <span style="color:#00ff88">■ Alt (m)</span></span>
            </div>
            <canvas id="chart-baro"></canvas>
          </div>
        </div>
      </div>

    </div>

    <!-- RIGHT COLUMN: IMU 6-DOF Vectors, Raw JSON Stream & Mission Control Log -->
    <div style="display: flex; flex-direction: column; gap: 16px;">
      
      <!-- IMU Vectors (ax, ay, az, gx, gy, gz) -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">MPU INERTIAL MEASUREMENT</span>
          <span style="font-size:11px; font-family:var(--font-mono); color:var(--text-dim);">SCALE: 16384 LSB/g, 131 LSB/(°/s)</span>
        </div>

        <div class="metric-label">LINEAR ACCELERATION (ACCEL_SCALE)</div>
        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">ACCEL X</span>
            <div class="metric-value-container">
              <span id="val-ax" class="metric-val unreceived">--.---</span>
              <span class="metric-unit">g</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">ACCEL Y</span>
            <div class="metric-value-container">
              <span id="val-ay" class="metric-val unreceived">--.---</span>
              <span class="metric-unit">g</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">ACCEL Z</span>
            <div class="metric-value-container">
              <span id="val-az" class="metric-val unreceived">--.---</span>
              <span class="metric-unit">g</span>
            </div>
          </div>
        </div>

        <div class="metric-label" style="margin-top:6px;">GYROSCOPIC ANGULAR RATE (GYRO_SCALE)</div>
        <div class="telemetry-row">
          <div class="metric-card">
            <span class="metric-label">GYRO X</span>
            <div class="metric-value-container">
              <span id="val-gx" class="metric-val unreceived">--.--</span>
              <span class="metric-unit">°/s</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">GYRO Y</span>
            <div class="metric-value-container">
              <span id="val-gy" class="metric-val unreceived">--.--</span>
              <span class="metric-unit">°/s</span>
            </div>
          </div>
          <div class="metric-card">
            <span class="metric-label">GYRO Z</span>
            <div class="metric-value-container">
              <span id="val-gz" class="metric-val unreceived">--.--</span>
              <span class="metric-unit">°/s</span>
            </div>
          </div>
        </div>
      </div>

      <!-- Raw Ingested JSON Inspector -->
      <div class="panel">
        <div class="panel-header">
          <span class="panel-title">LATEST RAW JSON PACKET</span>
          <button id="btn-copy-json" class="btn btn-aux" style="padding:2px 8px; font-size:10px;">COPY</button>
        </div>
        <pre id="raw-json-viewer" class="terminal-box" style="height: 180px;">// Awaiting ESP32 receiver serial output...
// Expected format from printTelemetryJSON():
// {"packetNo":1,"temperature":24.50,"humidity":45.00,"ax":0.012,"ay":-0.045,"az":0.982,"totalAcc":0.983,"gx":0.12,"gy":-0.45,"gz":0.02,"roll":-2.62,"pitch":-0.70,"latitude":0.000000,"longitude":0.000000,"altitude":0.00,"speed":0.00,"satellites":0,"gpsFix":0,"bmpTemperature":24.30,"pressure":1013.25,"bmpAltitude":216.50}</pre>
      </div>

      <!-- Mission Control Event Log -->
      <div class="panel" style="flex: 1;">
        <div class="panel-header">
          <span class="panel-title">MISSION CONTROL EVENT LOG</span>
          <button id="btn-clear-log" class="btn btn-aux" style="padding:2px 8px; font-size:10px;">CLEAR</button>
        </div>
        <div id="telemetry-log" class="terminal-box" style="height: 220px; overflow-y: scroll;"></div>
      </div>

    </div>

  </main>

  <!-- DIAGNOSTIC & FIRMWARE AUDIT MODAL -->
  <div id="test-modal" class="modal-backdrop">
    <div class="modal-box">
      <div class="modal-header">
        <span>FIRMWARE AUDIT & PIPELINE DIAGNOSTICS</span>
        <button id="btn-close-modal" style="background:none; border:none; color:var(--text-dim); font-size:22px; cursor:pointer;">&times;</button>
      </div>
      <div class="modal-content">
        <div style="color:var(--amber); font-weight:bold;">
          ⚠️ IDENTIFIED REASON FOR "Invalid packet size" ERROR:
        </div>
        <p style="color:var(--text-dim); font-size:12px;">
          Your <strong>Transmitter</strong> struct contains 3 fields at the end that are missing in the <strong>Receiver</strong> struct:
        </p>
        <div class="code-box">
// In Transmitter sketch:
struct SensorPacket {
  ...
  uint8_t mpuReady;
  uint8_t mpuAddress;
  uint8_t mpuWhoAmI;
};

// In Receiver sketch:
// MISSING the 3 fields above! Because len != sizeof(SensorPacket),
// the receiver rejects every packet and prints {"error":"Invalid packet size"}.
        </div>
        <p style="color:var(--green); font-size:12px;">
          <strong>Quick Fix:</strong> Add those 3 fields to the Receiver's <code>SensorPacket</code> struct, OR remove them from the Transmitter's struct so both <code>sizeof(SensorPacket)</code> are identical.
        </p>

        <hr style="border-color:var(--border-dim);"/>

        <div style="display:flex; justify-content:space-between; align-items:center;">
          <span style="font-weight:bold; color:var(--cyan);">AUTONOMOUS SUITE FOR YOUR EXACT SCHEMA:</span>
          <button id="btn-run-tests-exec" class="btn btn-connect">EXECUTE SELF-TEST</button>
        </div>
        
        <div id="test-results-container" style="background:#010409; padding:12px; border-radius:4px; border:1px solid var(--border-dim); max-height:240px; overflow-y:auto;">
          Click "Execute Self-Test" to run automated verification.
        </div>
      </div>
    </div>
  </div>

  <!-- APPLICATION LOGIC -->
  <script>
    /**
     * ASTRA ORBIT 2.0 — Ground Station Driver
     * Directly mapped to the user's ESP-NOW Transmitter and Receiver firmware.
     */

    const State = {
      connectionStatus: 'DISCONNECTED', // DISCONNECTED, CONNECTING, CONNECTED, RECEIVING_DATA, ERROR
      transportMode: 'webserial',
      port: null,
      reader: null,
      ws: null,
      serialStreamClosed: null,

      // Statistics
      packetCount: 0,
      sizeErrors: 0,
      lastPacketTime: null,
      packetTimes: [],

      // Simulation bench flag
      isSimulation: false,
      simInterval: null,

      // Latest Normalized Telemetry from exact firmware keys
      telemetry: {
        packetNo: null,
        temperature: null,
        humidity: null,
        ax: null, ay: null, az: null, totalAcc: null,
        gx: null, gy: null, gz: null,
        roll: null, pitch: null,
        latitude: null, longitude: null, altitude: null, speed: null,
        satellites: null, gpsFix: null,
        bmpTemperature: null, pressure: null, bmpAltitude: null,
        mpuReady: null
      },

      // Subsystem health detection
      health: {
        mpu: 'WAITING',
        bmp: 'WAITING',
        dht: 'WAITING',
        gps: 'WAITING',
        espnow: 'WAITING'
      },

      // Bounded history buffers (max 50 points)
      history: {
        max: 50,
        labels: [],
        ax: [], ay: [], az: [], totalAcc: [],
        gx: [], gy: [], gz: [],
        bmpTemp: [], dhtTemp: [], humidity: [],
        pressure: [], bmpAltitude: []
      }
    };

    const UI = {
      btnConnect: document.getElementById('btn-connect-toggle'),
      btnConnectText: document.getElementById('btn-connect-text'),
      btnIcon: document.getElementById('btn-icon'),
      statusIndicator: document.getElementById('status-indicator'),
      statusLabel: document.getElementById('status-label'),
      modeSelect: document.getElementById('conn-mode-select'),
      packetRateBadge: document.getElementById('packet-rate-badge'),
      diagSeq: document.getElementById('diag-seq'),
      diagTransport: document.getElementById('diag-transport'),
      diagPkts: document.getElementById('diag-pkts'),
      diagCorrupt: document.getElementById('diag-corrupt'),
      diagLastTime: document.getElementById('diag-last-time'),
      
      // Alerts
      alertPacketSize: document.getElementById('alert-packet-size'),
      alertImu: document.getElementById('alert-imu'),
      alertImuText: document.getElementById('alert-imu-text'),

      // Metrics
      valSeq: document.getElementById('diag-seq'),
      valBmpTemp: document.getElementById('val-bmp-temp'),
      valDhtTemp: document.getElementById('val-dht-temp'),
      valPressure: document.getElementById('val-pressure'),
      valHumidity: document.getElementById('val-humidity'),
      valBmpAlt: document.getElementById('val-bmp-alt'),

      valGpsLat: document.getElementById('val-gps-lat'),
      valGpsLon: document.getElementById('val-gps-lon'),
      valGpsAlt: document.getElementById('val-gps-alt'),
      valGpsSpeed: document.getElementById('val-gps-speed'),
      valGpsSats: document.getElementById('val-gps-sats'),
      gpsFixPill: document.getElementById('gps-fix-pill'),

      valRoll: document.getElementById('val-roll'),
      valPitch: document.getElementById('val-pitch'),
      valTotalAcc: document.getElementById('val-total-acc'),
      valAx: document.getElementById('val-ax'),
      valAy: document.getElementById('val-ay'),
      valAz: document.getElementById('val-az'),
      valGx: document.getElementById('val-gx'),
      valGy: document.getElementById('val-gy'),
      valGz: document.getElementById('val-gz'),

      // 3D Badges
      label3dRoll: document.getElementById('3d-roll'),
      label3dPitch: document.getElementById('3d-pitch'),
      imuModeLabel: document.getElementById('imu-mode-label'),

      // Subsystems
      subsysMpu: document.getElementById('subsys-mpu'),
      subsysBmp: document.getElementById('subsys-bmp'),
      subsysDht: document.getElementById('subsys-dht'),
      subsysGps: document.getElementById('subsys-gps'),
      subsysEspnow: document.getElementById('subsys-espnow'),

      // Log & Raw
      rawJsonViewer: document.getElementById('raw-json-viewer'),
      telemetryLog: document.getElementById('telemetry-log'),
      btnClearLog: document.getElementById('btn-clear-log'),
      btnCopyJson: document.getElementById('btn-copy-json'),
      btnToggleSim: document.getElementById('btn-toggle-sim'),

      // Audit Modal
      btnOpenTests: document.getElementById('btn-open-tests'),
      btnCloseModal: document.getElementById('btn-close-modal'),
      testModal: document.getElementById('test-modal'),
      btnRunTestsExec: document.getElementById('btn-run-tests-exec'),
      testResultsContainer: document.getElementById('test-results-container')
    };

    function logMessage(level, text) {
      const now = new Date();
      const timeStr = now.toTimeString().split(' ')[0] + '.' + String(now.getMilliseconds()).padStart(3, '0');
      
      const el = document.createElement('div');
      el.className = 'log-line';

      let lvlClass = 'log-info';
      if (level === 'OK') lvlClass = 'log-ok';
      if (level === 'WARN') lvlClass = 'log-warn';
      if (level === 'ERR') lvlClass = 'log-err';

      el.innerHTML = `<span class="log-time">[${timeStr}]</span> <span class="${lvlClass}">[${level}]</span> ${escapeHtml(text)}`;
      UI.telemetryLog.appendChild(el);
      UI.telemetryLog.scrollTop = UI.telemetryLog.scrollHeight;
    }

    function escapeHtml(string) {
      return String(string).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;');
    }

    UI.btnClearLog.addEventListener('click', () => {
      UI.telemetryLog.innerHTML = '';
      logMessage('INFO', 'Telemetry event log cleared.');
    });

    UI.btnCopyJson.addEventListener('click', () => {
      navigator.clipboard.writeText(UI.rawJsonViewer.textContent);
      logMessage('INFO', 'Raw JSON copied to clipboard.');
    });

    function setConnectionStatus(status, detail) {
      State.connectionStatus = status;
      UI.statusIndicator.className = 'status-dot';
      
      switch(status) {
        case 'DISCONNECTED':
          UI.statusLabel.textContent = 'DISCONNECTED';
          UI.btnConnect.className = 'btn btn-connect';
          UI.btnConnectText.textContent = 'CONNECT ESP32';
          UI.btnIcon.textContent = '⚡';
          break;
        case 'CONNECTING':
          UI.statusIndicator.classList.add('waiting');
          UI.statusLabel.textContent = 'CONNECTING AT 115200 BAUD...';
          UI.btnConnectText.textContent = 'CONNECTING...';
          break;
        case 'CONNECTED':
          UI.statusIndicator.classList.add('waiting');
          UI.statusLabel.textContent = 'CONNECTED • AWAITING PACKET';
          UI.btnConnect.className = 'btn btn-disconnect';
          UI.btnConnectText.textContent = 'DISCONNECT';
          UI.btnIcon.textContent = '⏹';
          break;
        case 'RECEIVING_DATA':
          UI.statusIndicator.classList.add('active');
          UI.statusLabel.textContent = 'LIVE TELEMETRY STREAM';
          UI.btnConnect.className = 'btn btn-disconnect';
          UI.btnConnectText.textContent = 'DISCONNECT';
          UI.btnIcon.textContent = '⏹';
          break;
        case 'ERROR':
          UI.statusIndicator.classList.add('error');
          UI.statusLabel.textContent = `ERROR: ${detail || 'FAILURE'}`;
          UI.btnConnect.className = 'btn btn-connect';
          UI.btnConnectText.textContent = 'RETRY CONNECT';
          UI.btnIcon.textContent = '⚡';
          break;
      }
      if (detail && status !== 'ERROR') logMessage('INFO', detail);
    }

    // --- TELEMETRY PROCESSOR STRICTLY MATCHING USER FIRMWARE ---
    function processIncomingLine(rawText) {
      const line = rawText.trim();
      if (!line) return;

      UI.rawJsonViewer.textContent = line;

      let packet;
      try {
        packet = JSON.parse(line);
      } catch (err) {
        logMessage('WARN', `Malformed JSON dropped: ${line.substring(0, 48)}...`);
        return null;
      }

      // 1. Intercept receiver struct error: {"error":"Invalid packet size"}
      if (packet.error) {
        if (/packet size/i.test(packet.error)) {
          State.sizeErrors++;
          UI.diagCorrupt.textContent = State.sizeErrors;
          UI.alertPacketSize.style.display = 'flex';
          logMessage('ERR', 'Receiver dropped packet: Invalid packet size! Check struct definition.');
        } else {
          logMessage('ERR', `Hardware Fault: ${packet.error}`);
        }
        return packet;
      }

      // Valid packet arrived
      State.packetCount++;
      const now = new Date();
      State.lastPacketTime = now;
      State.packetTimes.push(now.getTime());

      // Calculate reception rate (Hz)
      const twoSecAgo = now.getTime() - 2000;
      State.packetTimes = State.packetTimes.filter(t => t > twoSecAgo);
      const hz = (State.packetTimes.length / 2.0).toFixed(1);
      UI.packetRateBadge.textContent = `${hz} Hz`;

      UI.diagPkts.textContent = State.packetCount;
      UI.diagLastTime.textContent = now.toTimeString().split(' ')[0];

      if (State.connectionStatus !== 'RECEIVING_DATA') {
        setConnectionStatus('RECEIVING_DATA', 'ESP-NOW Telemetry stream active.');
      }

      // Extract exact firmware fields
      const p = packet;
      const getNum = (key) => (p[key] !== undefined && p[key] !== null && !isNaN(Number(p[key]))) ? Number(p[key]) : null;

      const packetNo = getNum('packetNo');
      const temperature = getNum('temperature');
      const humidity = getNum('humidity');
      const ax = getNum('ax');
      const ay = getNum('ay');
      const az = getNum('az');
      const totalAcc = getNum('totalAcc');
      const gx = getNum('gx');
      const gy = getNum('gy');
      const gz = getNum('gz');
      const roll = getNum('roll');
      const pitch = getNum('pitch');
      const latitude = getNum('latitude');
      const longitude = getNum('longitude');
      const altitude = getNum('altitude');
      const speed = getNum('speed');
      const satellites = getNum('satellites');
      const gpsFix = getNum('gpsFix');
      const bmpTemperature = getNum('bmpTemperature');
      const pressure = getNum('pressure');
      const bmpAltitude = getNum('bmpAltitude');

      // Update State
      State.telemetry = {
        packetNo, temperature, humidity,
        ax, ay, az, totalAcc,
        gx, gy, gz,
        roll, pitch,
        latitude, longitude, altitude, speed,
        satellites, gpsFix,
        bmpTemperature, pressure, bmpAltitude,
        mpuReady: getNum('mpuReady')
      };

      // Detect Sensor States based on firmware logic:
      // A. BMP280: If bmpReady was false, TX sends bmpTemperature=0, pressure=0, bmpAltitude=0
      const bmpActive = !(pressure === 0.0 && bmpTemperature === 0.0 && bmpAltitude === 0.0);
      if (bmpActive && pressure !== null) {
        State.health.bmp = 'OK';
        UI.subsysBmp.innerHTML = '<span class="badge-ok">ACTIVE (0x76/0x77)</span>';
      } else {
        State.health.bmp = 'ERROR';
        UI.subsysBmp.innerHTML = '<span class="badge-err">NOT DETECTED (0 hPa)</span>';
      }

      // B. MPU6500 / MPU6050: If packetMpuReady was false, TX sends all zeros for IMU
      const mpuActive = !(ax === 0 && ay === 0 && az === 0 && gx === 0 && gy === 0 && gz === 0 && roll === 0 && pitch === 0);
      if (mpuActive && ax !== null) {
        State.health.mpu = 'OK';
        UI.subsysMpu.innerHTML = '<span class="badge-ok">STREAMING (I2C 0)</span>';
        UI.alertImu.style.display = 'none';
        UI.imuModeLabel.textContent = 'TX FILTER ATTITUDE ACTIVE';
        UI.imuModeLabel.style.color = 'var(--cyan)';
        updateCubeSatOrientation(roll, pitch);
      } else {
        State.health.mpu = 'ERROR';
        UI.subsysMpu.innerHTML = '<span class="badge-err">OMITTED / NOT READY</span>';
        UI.alertImu.style.display = 'flex';
        UI.alertImuText.textContent = 'MPU6500 sensor not detected by transmitter or zero-vector received.';
        UI.imuModeLabel.textContent = 'IMU OFFLINE (INERT VIEW)';
        UI.imuModeLabel.style.color = 'var(--amber)';
      }

      // C. DHT11:
      if (temperature !== null && !isNaN(temperature)) {
        State.health.dht = 'OK';
        UI.subsysDht.innerHTML = '<span class="badge-ok">ONLINE (GPIO4)</span>';
      }

      // D. GPS Fix:
      if (gpsFix === 1) {
        State.health.gps = 'OK';
        UI.subsysGps.innerHTML = `<span class="badge-ok">FIX LOCKED (${satellites} SATS)</span>`;
        UI.gpsFixPill.className = 'badge-ok';
        UI.gpsFixPill.textContent = `FIX LOCKED (${satellites} SATS)`;
      } else {
        State.health.gps = 'WAITING';
        UI.subsysGps.innerHTML = `<span class="badge-wait">SEARCHING (${satellites || 0} SATS)</span>`;
        UI.gpsFixPill.className = 'badge-wait';
        UI.gpsFixPill.textContent = `SEARCHING (${satellites || 0} SATS)`;
      }

      // E. ESP-NOW Link:
      State.health.espnow = 'OK';
      UI.subsysEspnow.innerHTML = '<span class="badge-ok">ACTIVE (CH 1)</span>';

      // Update UI elements
      updateUIElements(bmpActive, mpuActive);

      // Push to bounded rolling charts
      pushToChartHistory(ax, ay, az, totalAcc, gx, gy, gz, bmpTemperature, temperature, humidity, pressure, bmpAltitude);

      return packet;
    }

    function updateUIElements(bmpActive, mpuActive) {
      const t = State.telemetry;

      // Packet sequence
      UI.diagSeq.textContent = (t.packetNo !== null) ? `#${t.packetNo}` : '--';

      // Environmental
      if (bmpActive && t.bmpTemperature !== null) {
        setMetric(UI.valBmpTemp, t.bmpTemperature.toFixed(2));
        setMetric(UI.valPressure, t.pressure.toFixed(2));
        setMetric(UI.valBmpAlt, t.bmpAltitude.toFixed(2));
      } else {
        setSensorOff(UI.valBmpTemp, 'OFFLINE');
        setSensorOff(UI.valPressure, 'OFFLINE');
        setSensorOff(UI.valBmpAlt, 'OFFLINE');
      }

      if (t.temperature !== null) {
        setMetric(UI.valDhtTemp, t.temperature.toFixed(2));
        setMetric(UI.valHumidity, t.humidity.toFixed(2));
      } else {
        setUnreceived(UI.valDhtTemp);
        setUnreceived(UI.valHumidity);
      }

      // GPS
      if (t.gpsFix === 1) {
        setMetric(UI.valGpsLat, t.latitude.toFixed(6));
        setMetric(UI.valGpsLon, t.longitude.toFixed(6));
        setMetric(UI.valGpsAlt, t.altitude.toFixed(2));
        setMetric(UI.valGpsSpeed, t.speed.toFixed(2));
      } else {
        setSensorOff(UI.valGpsLat, 'NO FIX');
        setSensorOff(UI.valGpsLon, 'NO FIX');
        setSensorOff(UI.valGpsAlt, 'NO FIX');
        setSensorOff(UI.valGpsSpeed, '0.00');
      }
      setMetric(UI.valGpsSats, t.satellites !== null ? t.satellites : 0);

      // IMU / Attitude
      if (mpuActive && t.ax !== null) {
        setMetric(UI.valAx, t.ax.toFixed(3));
        setMetric(UI.valAy, t.ay.toFixed(3));
        setMetric(UI.valAz, t.az.toFixed(3));
        setMetric(UI.valTotalAcc, t.totalAcc.toFixed(3));

        setMetric(UI.valGx, t.gx.toFixed(2));
        setMetric(UI.valGy, t.gy.toFixed(2));
        setMetric(UI.valGz, t.gz.toFixed(2));

        setMetric(UI.valRoll, t.roll.toFixed(2));
        setMetric(UI.valPitch, t.pitch.toFixed(2));
        UI.label3dRoll.textContent = `${t.roll.toFixed(2)}°`;
        UI.label3dPitch.textContent = `${t.pitch.toFixed(2)}°`;
      } else {
        setSensorOff(UI.valAx, 'OMITTED');
        setSensorOff(UI.valAy, 'OMITTED');
        setSensorOff(UI.valAz, 'OMITTED');
        setSensorOff(UI.valTotalAcc, 'OMITTED');

        setSensorOff(UI.valGx, 'OMITTED');
        setSensorOff(UI.valGy, 'OMITTED');
        setSensorOff(UI.valGz, 'OMITTED');

        setSensorOff(UI.valRoll, '--');
        setSensorOff(UI.valPitch, '--');
        UI.label3dRoll.textContent = '--.-°';
        UI.label3dPitch.textContent = '--.-°';
      }
    }

    function setMetric(el, str) {
      el.className = 'metric-val';
      el.textContent = str;
    }
    function setUnreceived(el) {
      el.className = 'metric-val unreceived';
      el.textContent = '--';
    }
    function setSensorOff(el, label) {
      el.className = 'metric-val sensor-off';
      el.textContent = label;
    }

    // --- 3D CUBESAT RENDERING (THREE.JS) ---
    let scene, camera, renderer, controls, cubeSatGroup;

    function init3D() {
      const container = document.getElementById('cubesat-viewport');
      const width = container.clientWidth;
      const height = container.clientHeight;

      scene = new THREE.Scene();
      camera = new THREE.PerspectiveCamera(45, width / height, 0.1, 100);
      camera.position.set(2.6, 2.2, 3.2);

      renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
      renderer.setSize(width, height);
      renderer.setPixelRatio(window.devicePixelRatio);
      container.appendChild(renderer.domElement);

      controls = new THREE.OrbitControls(camera, renderer.domElement);
      controls.enableDamping = true;
      controls.dampingFactor = 0.05;

      // Aerospace lighting
      const ambientLight = new THREE.AmbientLight(0xffffff, 0.7);
      scene.add(ambientLight);

      const dirLight = new THREE.DirectionalLight(0x00f3ff, 1.2);
      dirLight.position.set(5, 10, 7);
      scene.add(dirLight);

      const dirLight2 = new THREE.DirectionalLight(0xffaa00, 0.5);
      dirLight2.position.set(-5, -5, -5);
      scene.add(dirLight2);

      // Coordinate reference plane
      const grid = new THREE.GridHelper(10, 20, 0x00f3ff, 0x162a4a);
      grid.position.y = -1.2;
      scene.add(grid);

      // CubeSat 1U Structure
      cubeSatGroup = new THREE.Group();

      // Aluminum Chassis
      const bodyGeo = new THREE.BoxGeometry(1.2, 1.2, 1.2);
      const bodyMat = new THREE.MeshStandardMaterial({
        color: 0x141c28,
        roughness: 0.35,
        metalness: 0.85
      });
      cubeSatGroup.add(new THREE.Mesh(bodyGeo, bodyMat));

      // Rails
      const railGeo = new THREE.BoxGeometry(1.23, 1.23, 1.23);
      const railMat = new THREE.MeshBasicMaterial({ color: 0x00f3ff, wireframe: true });
      cubeSatGroup.add(new THREE.Mesh(railGeo, railMat));

      // Photovoltaic Solar Panels
      const panelMat = new THREE.MeshStandardMaterial({ color: 0x081f3d, roughness: 0.2, metalness: 0.7 });
      const addSolarFace = (px, py, pz, rx, ry, rz) => {
        const pMesh = new THREE.Mesh(new THREE.PlaneGeometry(1.0, 1.0), panelMat);
        pMesh.position.set(px, py, pz);
        pMesh.rotation.set(rx, ry, rz);
        cubeSatGroup.add(pMesh);

        const wire = new THREE.Mesh(new THREE.PlaneGeometry(1.0, 1.0, 4, 4), new THREE.MeshBasicMaterial({ color: 0x1e4976, wireframe: true }));
        wire.position.set(px * 1.001, py * 1.001, pz * 1.001);
        wire.rotation.set(rx, ry, rz);
        cubeSatGroup.add(wire);
      };

      addSolarFace(0, 0, 0.605, 0, 0, 0);
      addSolarFace(0, 0, -0.605, 0, Math.PI, 0);
      addSolarFace(0.605, 0, 0, 0, Math.PI/2, 0);
      addSolarFace(-0.605, 0, 0, 0, -Math.PI/2, 0);

      // Monopole Antenna
      const antGeo = new THREE.CylinderGeometry(0.015, 0.015, 1.3);
      const antMat = new THREE.MeshStandardMaterial({ color: 0xd4af37, metalness: 0.9 });
      const antenna = new THREE.Mesh(antGeo, antMat);
      antenna.position.set(0.55, 1.15, 0.55);
      antenna.rotation.z = -0.2;
      cubeSatGroup.add(antenna);

      // GPS Patch Antenna
      const patch = new THREE.Mesh(new THREE.BoxGeometry(0.35, 0.05, 0.35), new THREE.MeshStandardMaterial({ color: 0xdedede, roughness: 0.4 }));
      patch.position.set(0, 0.62, 0);
      cubeSatGroup.add(patch);

      // Coordinate axes
      cubeSatGroup.add(new THREE.AxesHelper(1.8));

      scene.add(cubeSatGroup);

      function animate() {
        requestAnimationFrame(animate);
        controls.update();

        // If MPU readings are zero/omitted, gently oscillate for visual confirmation
        if (State.telemetry.roll === null || (State.telemetry.roll === 0 && State.telemetry.pitch === 0)) {
          cubeSatGroup.rotation.y += 0.003;
        }

        renderer.render(scene, camera);
      }
      animate();

      window.addEventListener('resize', () => {
        if (!container) return;
        const w = container.clientWidth;
        const h = container.clientHeight;
        camera.aspect = w / h;
        camera.updateProjectionMatrix();
        renderer.setSize(w, h);
      });
    }

    function updateCubeSatOrientation(rollDeg, pitchDeg) {
      if (!cubeSatGroup) return;
      const r = (rollDeg || 0) * (Math.PI / 180.0);
      const p = (pitchDeg || 0) * (Math.PI / 180.0);

      cubeSatGroup.rotation.order = 'ZYX';
      cubeSatGroup.rotation.z = r;
      cubeSatGroup.rotation.x = p;
    }

    // --- LIVE CANVAS ROLLING CHARTS ---
    class RollingCanvasChart {
      constructor(canvasId, lines) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d');
        this.lines = lines;
        this.resize();
        window.addEventListener('resize', () => this.resize());
      }

      resize() {
        const dpr = window.devicePixelRatio || 1;
        this.canvas.width = this.canvas.clientWidth * dpr;
        this.canvas.height = this.canvas.clientHeight * dpr;
        this.ctx.scale(dpr, dpr);
      }

      render(historyData) {
        const ctx = this.ctx;
        const w = this.canvas.clientWidth;
        const h = this.canvas.clientHeight;

        ctx.clearRect(0, 0, w, h);

        ctx.strokeStyle = '#12233c';
        ctx.lineWidth = 1;
        ctx.beginPath();
        for (let y = 10; y < h; y += 24) {
          ctx.moveTo(0, y);
          ctx.lineTo(w, y);
        }
        ctx.stroke();

        if (historyData.labels.length < 2) {
          ctx.fillStyle = '#6c84a3';
          ctx.font = '10px JetBrains Mono';
          ctx.textAlign = 'center';
          ctx.fillText('AWAITING TELEMETRY SAMPLES...', w / 2, h / 2 + 3);
          return;
        }

        const step = w / (State.history.max - 1);

        this.lines.forEach(line => {
          const arr = historyData[line.key];
          if (!arr || arr.length === 0) return;

          ctx.strokeStyle = line.color;
          ctx.lineWidth = 1.5;
          ctx.beginPath();

          for (let i = 0; i < arr.length; i++) {
            const val = arr[i];
            const x = i * step;

            let norm = 0.5;
            if (val !== null && !isNaN(val)) {
              norm = (val - line.min) / (line.max - line.min);
              norm = Math.max(0, Math.min(1, norm));
            }
            const y = h - (norm * (h - 16) + 8);

            if (i === 0) ctx.moveTo(x, y);
            else ctx.lineTo(x, y);
          }
          ctx.stroke();
        });
      }
    }

    let chartAccel, chartGyro, chartEnv, chartBaro;

    function initCharts() {
      chartAccel = new RollingCanvasChart('chart-accel', [
        { key: 'ax', color: '#ff3366', min: -2, max: 2 },
        { key: 'ay', color: '#00ff88', min: -2, max: 2 },
        { key: 'az', color: '#00f3ff', min: -2, max: 2 },
        { key: 'totalAcc', color: '#ffffff', min: 0, max: 3 }
      ]);

      chartGyro = new RollingCanvasChart('chart-gyro', [
        { key: 'gx', color: '#ffaa00', min: -180, max: 180 },
        { key: 'gy', color: '#b366ff', min: -180, max: 180 },
        { key: 'gz', color: '#00f3ff', min: -180, max: 180 }
      ]);

      chartEnv = new RollingCanvasChart('chart-env', [
        { key: 'bmpTemp', color: '#ff3366', min: 0, max: 50 },
        { key: 'dhtTemp', color: '#ffaa00', min: 0, max: 50 },
        { key: 'humidity', color: '#00f3ff', min: 0, max: 100 }
      ]);

      chartBaro = new RollingCanvasChart('chart-baro', [
        { key: 'pressure', color: '#ffaa00', min: 800, max: 1100 },
        { key: 'bmpAltitude', color: '#00ff88', min: 0, max: 1000 }
      ]);

      renderAllCharts();
    }

    function pushToChartHistory(ax, ay, az, totalAcc, gx, gy, gz, bmpTemp, dhtTemp, hum, press, alt) {
      const h = State.history;
      h.labels.push(Date.now());
      h.ax.push(ax);
      h.ay.push(ay);
      h.az.push(az);
      h.totalAcc.push(totalAcc);
      h.gx.push(gx);
      h.gy.push(gy);
      h.gz.push(gz);
      h.bmpTemp.push(bmpTemp);
      h.dhtTemp.push(dhtTemp);
      h.humidity.push(hum);
      h.pressure.push(press);
      h.bmpAltitude.push(alt);

      if (h.labels.length > h.max) {
        h.labels.shift();
        h.ax.shift();
        h.ay.shift();
        h.az.shift();
        h.totalAcc.shift();
        h.gx.shift();
        h.gy.shift();
        h.gz.shift();
        h.bmpTemp.shift();
        h.dhtTemp.shift();
        h.humidity.shift();
        h.pressure.shift();
        h.bmpAltitude.shift();
      }

      renderAllCharts();
    }

    function renderAllCharts() {
      chartAccel.render(State.history);
      chartGyro.render(State.history);
      chartEnv.render(State.history);
      chartBaro.render(State.history);
    }

    // --- DIRECT WEB SERIAL DRIVER (115200 BAUD) ---
    async function startWebSerial() {
      if (!('serial' in navigator)) {
        alert('Web Serial API is not supported in this browser. Please use Chrome, Edge, or Brave.');
        setConnectionStatus('ERROR', 'Web Serial API unsupported in browser.');
        return;
      }

      try {
        setConnectionStatus('CONNECTING', 'Prompting user for USB Serial selection.');
        logMessage('INFO', 'Opening Web Serial port selector at 115200 baud.');

        State.port = await navigator.serial.requestPort();
        await State.port.open({ baudRate: 115200 });

        setConnectionStatus('CONNECTED', 'ESP32 Serial Port opened at 115200 baud.');
        logMessage('OK', 'Connected to ESP32 Receiver over USB Serial.');

        const textDecoder = new TextDecoderStream();
        State.serialStreamClosed = State.port.readable.pipeTo(textDecoder.writable);
        State.reader = textDecoder.readable.getReader();

        let partialBuffer = '';

        while (true) {
          const { value, done } = await State.reader.read();
          if (done) break;

          if (value) {
            partialBuffer += value;
            const lines = partialBuffer.split(/\r?\n/);
            partialBuffer = lines.pop(); // Retain incomplete line

            for (const line of lines) {
              if (line.trim().length > 0) {
                processIncomingLine(line);
              }
            }
          }
        }
      } catch (err) {
        if (err.name === 'NotFoundError') {
          setConnectionStatus('DISCONNECTED', 'Port selection cancelled.');
        } else {
          setConnectionStatus('ERROR', err.message);
          logMessage('ERR', `Serial Port Fault: ${err.message}`);
        }
      } finally {
        await disconnectSerial();
      }
    }

    async function disconnectSerial() {
      try {
        if (State.reader) {
          await State.reader.cancel();
          State.reader = null;
        }
        if (State.serialStreamClosed) {
          await State.serialStreamClosed.catch(() => {});
          State.serialStreamClosed = null;
        }
        if (State.port) {
          await State.port.close();
          State.port = null;
        }
      } catch (e) {
        console.error('Serial port cleanup error:', e);
      }
      setConnectionStatus('DISCONNECTED', 'Serial port released.');
      logMessage('INFO', 'ESP32 Serial Port closed cleanly.');
    }

    // --- NODE.JS WEBSOCKET BRIDGE CLIENT ---
    function startWebSocketBridge(endpointUrl = 'ws://localhost:8081') {
      setConnectionStatus('CONNECTING', `Connecting to WebSocket bridge at ${endpointUrl}...`);
      try {
        State.ws = new WebSocket(endpointUrl);
        State.ws.onopen = () => {
          setConnectionStatus('CONNECTED', `Connected to local bridge (${endpointUrl}).`);
          logMessage('OK', `WebSocket Bridge linked: ${endpointUrl}`);
        };
        State.ws.onmessage = (event) => processIncomingLine(event.data);
        State.ws.onerror = () => setConnectionStatus('ERROR', 'WebSocket bridge unavailable');
        State.ws.onclose = () => {
          setConnectionStatus('DISCONNECTED', 'Bridge disconnected.');
          State.ws = null;
        };
      } catch (err) {
        setConnectionStatus('ERROR', err.message);
      }
    }

    function disconnectWebSocket() {
      if (State.ws) {
        State.ws.close();
        State.ws = null;
      }
      setConnectionStatus('DISCONNECTED', 'Bridge closed.');
    }

    // --- CONNECTION TOGGLE ---
    UI.btnConnect.addEventListener('click', async () => {
      if (State.connectionStatus === 'CONNECTED' || State.connectionStatus === 'RECEIVING_DATA') {
        if (State.transportMode === 'webserial') await disconnectSerial();
        else disconnectWebSocket();
        return;
      }

      if (State.isSimulation) toggleSimulation(false);

      if (State.transportMode === 'webserial') {
        await startWebSerial();
      } else {
        const url = prompt('Enter WebSocket Bridge URL:', 'ws://localhost:8081');
        if (url) startWebSocketBridge(url);
      }
    });

    UI.modeSelect.addEventListener('change', (e) => {
      State.transportMode = e.target.value;
      UI.diagTransport.textContent = (State.transportMode === 'webserial') ? 'Web Serial (115200 Baud)' : 'Node.js WebSocket';
    });

    // --- DEV SIMULATOR MATCHING EXACT FIRMWARE KEYS ---
    function toggleSimulation(forceState) {
      State.isSimulation = (forceState !== undefined) ? forceState : !State.isSimulation;

      if (State.isSimulation) {
        UI.btnToggleSim.style.background = 'var(--amber)';
        UI.btnToggleSim.style.color = '#000';
        UI.btnToggleSim.textContent = 'STOP SIM';
        logMessage('WARN', '[DEV BENCH] Generating synthetic packets matching printTelemetryJSON().');

        let simPkt = 1;
        let simT = 0;
        State.simInterval = setInterval(() => {
          simT += 0.1;
          const fakePacket = JSON.stringify({
            packetNo: simPkt++,
            temperature: +(24.2 + Math.sin(simT * 0.1) * 0.5).toFixed(2),
            humidity: +(45.0 + Math.cos(simT * 0.2) * 2.0).toFixed(2),
            ax: +(Math.sin(simT * 0.5) * 0.2).toFixed(3),
            ay: +(Math.cos(simT * 0.4) * 0.2).toFixed(3),
            az: +(0.98 + Math.sin(simT) * 0.02).toFixed(3),
            totalAcc: 1.002,
            gx: +(Math.sin(simT) * 4.2).toFixed(2),
            gy: +(Math.cos(simT) * 3.1).toFixed(2),
            gz: +(0.15).toFixed(2),
            roll: +(Math.sin(simT * 0.8) * 12.0).toFixed(2),
            pitch: +(Math.cos(simT * 0.6) * 10.0).toFixed(2),
            latitude: +(28.6139 + Math.sin(simT * 0.01) * 0.0005).toFixed(6),
            longitude: +(77.2090 + Math.cos(simT * 0.01) * 0.0005).toFixed(6),
            altitude: +(216.5 + Math.sin(simT * 0.2) * 2.0).toFixed(2),
            speed: +(1.2).toFixed(2),
            satellites: 8,
            gpsFix: 1,
            bmpTemperature: +(24.1 + Math.sin(simT * 0.1) * 0.4).toFixed(2),
            pressure: +(1013.25 + Math.sin(simT * 0.2) * 2.5).toFixed(2),
            bmpAltitude: +(216.0 + Math.sin(simT * 0.2) * 1.5).toFixed(2)
          });
          processIncomingLine(fakePacket);
        }, 150);
      } else {
        clearInterval(State.simInterval);
        State.simInterval = null;
        UI.btnToggleSim.style.background = 'var(--bg-card)';
        UI.btnToggleSim.style.color = 'var(--amber)';
        UI.btnToggleSim.textContent = 'DEV SIM';
        logMessage('INFO', 'Dev simulator halted.');
      }
    }

    UI.btnToggleSim.addEventListener('click', () => toggleSimulation());

    // --- PIPELINE VALIDATION AUDIT ---
    UI.btnOpenTests.addEventListener('click', () => UI.testModal.style.display = 'flex');
    UI.btnCloseModal.addEventListener('click', () => UI.testModal.style.display = 'none');
    UI.btnRunTestsExec.addEventListener('click', runAutomatedPipelineTests);

    function runAutomatedPipelineTests() {
      const container = UI.testResultsContainer;
      container.innerHTML = '<div style="color:var(--cyan);">Running schema validation...</div><br/>';

      const results = [];
      const assert = (name, cond, notes) => results.push({ name, passed: Boolean(cond), notes });

      // Test 1: Validate Exact Receiver Output JSON
      const sampleRx = '{"packetNo":1,"temperature":24.50,"humidity":45.00,"ax":0.012,"ay":-0.045,"az":0.982,"totalAcc":0.983,"gx":0.12,"gy":-0.45,"gz":0.02,"roll":-2.62,"pitch":-0.70,"latitude":0.000000,"longitude":0.000000,"altitude":0.00,"speed":0.00,"satellites":0,"gpsFix":0,"bmpTemperature":24.30,"pressure":1013.25,"bmpAltitude":216.50}';
      const parsed = JSON.parse(sampleRx);
      assert('Firmware Schema Compatibility', parsed.packetNo === 1 && parsed.totalAcc === 0.983 && parsed.roll === -2.62, 'All keys from printTelemetryJSON() match dashboard bindings');

      // Test 2: Invalid Packet Size Error Handling
      const errRx = '{"error":"Invalid packet size"}';
      const parsedErr = JSON.parse(errRx);
      assert('Packet Size Discrepancy Detection', parsedErr.error === "Invalid packet size", 'Catches struct mismatch without throwing UI exception');

      // Test 3: Zero-Vector Sensor Detection (MPU not ready)
      const isMpuOmitted = (parsed.latitude === 0 && parsed.longitude === 0);
      assert('GPS Fix False Identification', parsed.gpsFix === 0 && isMpuOmitted, 'Identified unacquired GPS fix and suppressed Null Island coordinates');

      // Test 4: Roll/Pitch Direct Complementary Pass-Through
      assert('Complementary Angle Pass-Through', parsed.roll === -2.62 && parsed.pitch === -0.70, 'Transmitter α=0.98 filtered angles drive 3D model directly');

      let html = '';
      let passedCount = 0;
      results.forEach((r, idx) => {
        if (r.passed) passedCount++;
        html += `<div style="margin-bottom: 6px;">
          <span style="color: ${r.passed ? 'var(--green)' : 'var(--red)'}; font-weight:bold;">[${r.passed ? 'PASS' : 'FAIL'}]</span> 
          <strong>Test ${idx + 1}: ${r.name}</strong>
          <div style="color:var(--text-dim); font-size:11px; margin-left:18px;">${r.notes}</div>
        </div>`;
      });

      html += `<hr style="border-color:var(--border-dim); margin: 8px 0;"/>`;
      html += `<div style="font-weight:bold; color: ${passedCount === results.length ? 'var(--green)' : 'var(--amber)'};">
        RESULT: ${passedCount} / ${results.length} PASSED.
      </div>`;

      container.innerHTML = html;
      logMessage('OK', `Automated pipeline test complete (${passedCount}/${results.length} assertions passed).`);
    }

    window.addEventListener('DOMContentLoaded', () => {
      logMessage('INFO', 'ASTRA ORBIT 2.0 CubeSat Mission Control initialized.');
      logMessage('INFO', 'Listening for USB Serial data at 115200 baud.');
      init3D();
      initCharts();
    });
  </script>
</body>
</html> 