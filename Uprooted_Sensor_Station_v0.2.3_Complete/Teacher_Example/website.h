#pragma once
#include <Arduino.h>

// UPROOTED: This is your website. Change headings, colors, and HTML below.
// Keep the JavaScript: /api/status identifies the board's mode and /api/nodes
// provides temperature readings from other Heltec boards.
// Leave the R"UPROOTEDHTML( and )UPROOTEDHTML"; delimiters untouched.
static const char STUDENT_HTML[] PROGMEM = R"UPROOTEDHTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Uprooted | Sensor Network</title>
  <style>
    * { box-sizing: border-box; }
    body { font-family: system-ui, sans-serif; background:#10231f; color:#f5fff9;
           margin:0; padding:20px; }
    main { max-width:780px; margin:auto; }
    h1 { margin-bottom:4px; }
    .muted { color:#b6cfc0; }
    .card { background:#1b382f; border:1px solid #355a48; border-radius:14px;
            padding:18px; margin:14px 0; }
    .mode { background:#134539; border-color:#5eb795; }
    .big { font-size:2.8rem; font-weight:700; }
    .grid { display:grid; gap:12px; grid-template-columns:repeat(auto-fit,minmax(185px,1fr)); }
    .peer { background:#254a3b; border:1px solid #48725d; border-radius:10px; padding:14px; }
    .temp { font-size:1.7rem; font-weight:700; margin:8px 0; }
    #modeCard[hidden], #localCard[hidden] { display:none; }
  </style>
</head>
<body>
  <main>
    <h1>Uprooted Sensor Network</h1>
    <!-- TODO: Students, add a short introduction to your project. -->
    <p class="muted">Heltec V4 &bull; PT100 &bull; LoRa</p>

    <section class="card mode" id="modeCard" hidden>
      <h2>Operating as an exit node</h2>
      <p>The local MAX31865 is unavailable. This board collects and relays remote
      LoRa sensor readings and makes them visible over its Wi-Fi access point.
      It does not provide internet access.</p>
    </section>

    <section class="card" id="fleetCard">
      <h2>Connected sensors <span id="count">(0)</span></h2>
      <p id="radioState" class="muted">Listening for LoRa packets...</p>
      <div id="sensors" class="grid" aria-live="polite">Waiting for other sensor stations...</div>
    </section>

    <section class="card" id="localCard">
      <h2>Local PT100</h2>
      <div class="big"><span id="temperature">--</span> &deg;C</div>
      <p id="status" class="muted">Waiting for sensor...</p>
    </section>
  </main>

  <script>
    // Change the HTML/CSS above. This function connects the webpage to live data.
    const byId = id => document.getElementById(id);
    const formatTemp = value => Number(value).toFixed(1) + ' °C';
    async function refresh() {
      try {
        const [statusResponse, nodesResponse] = await Promise.all([
          fetch('/api/status', {cache:'no-store'}),
          fetch('/api/nodes', {cache:'no-store'})
        ]);
        if (!statusResponse.ok || !nodesResponse.ok) throw new Error('Server unavailable');
        const status = await statusResponse.json();
        const network = await nodesResponse.json();
        const exitNode = status.mode === 'exit_node';
        byId('modeCard').hidden = !exitNode;
        byId('localCard').hidden = exitNode;
        byId('status').textContent = status.ok ? 'Sensor OK' : status.status;
        byId('temperature').textContent = status.ok && status.temperature_c !== null
          ? Number(status.temperature_c).toFixed(1) : '--';

        const active = network.peers.filter(peer => peer.online);
        byId('count').textContent = '(' + active.length + ')';
        byId('radioState').textContent = network.radio_configured
          ? 'LoRa listening · ' + network.rx + ' received · ' + network.relayed + ' relayed'
          : 'LoRa radio unavailable';
        const panel = byId('sensors');
        panel.replaceChildren();
        if (!active.length) {
          panel.textContent = network.radio_configured
            ? 'Waiting for sensor broadcasts from other Heltec boards...'
            : 'Start LoRa in setupLoRa() to receive other sensors.';
        }
        for (const peer of active) {
          const card = document.createElement('div');
          card.className = 'peer';
          const name = document.createElement('strong');
          name.textContent = 'Sensor ' + peer.id;
          const value = document.createElement('div');
          value.className = 'temp';
          value.textContent = peer.valid ? formatTemp(peer.temperature_c)
            : 'Sensor fault 0x' + Number(peer.fault).toString(16).toUpperCase();
          const detail = document.createElement('div');
          detail.className = 'muted';
          detail.textContent = 'RSSI ' + peer.rssi + ' dBm · ' + peer.age_s + ' s ago'
            + (peer.via_relay ? ' · via relay' : ' · direct');
          card.append(name, value, detail);
          panel.appendChild(card);
        }
      } catch (error) {
        byId('radioState').textContent = 'Connection error: ' + error.message;
      }
    }
    refresh();
    setInterval(refresh, 2000);
  </script>
</body>
</html>
)UPROOTEDHTML";
