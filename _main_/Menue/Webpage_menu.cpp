#include "Webpage_menu.h"
#include "../hardware.h"
#include <Adafruit_SSD1327.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>

extern Adafruit_SSD1327 display;
extern int menu_cursor;
extern int selected_menu;
extern bool Button_input_Pressed[4];

// Web server variables
WebServer server(80);
bool serverRunning = false;

void drawMenuWithCursor(const char* menuItems[], int menuSize, int& cursor, const char* title);

// Forward declarations
void handleRoot();
void handleSetProgram();
void handleSetColor();
void handleSetBrightness();
void handleStatus();

// Simple web page content - Fixed raw string literal
const char webPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Protogen Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; margin: 20px; background: #1a1a1a; color: white; }
        .container { max-width: 600px; margin: 0 auto; }
        button { padding: 10px 20px; margin: 5px; font-size: 16px; }
        .program-btn { background: #4CAF50; color: white; border: none; border-radius: 5px; }
        .color-btn { background: #008CBA; color: white; border: none; border-radius: 5px; }
        .status { background: #333; padding: 10px; border-radius: 5px; margin: 10px 0; }
    </style>
</head>
<body>
    <div class="container">
        <h1>🤖 Protogen Control Panel</h1>
        
        <div class="status">
            <h3>Current Status</h3>
            <p>Program: <span id="currentProgram">Loading...</span></p>
            <p>IP Address: <span id="ipAddress">Loading...</span></p>
        </div>
        
        <h3>Programs</h3>
        <button class="program-btn" onclick="setProgram(0)">Test Mode</button>
        <button class="program-btn" onclick="setProgram(1)">Smile Static</button>
        <button class="program-btn" onclick="setProgram(2)">Gray Fill</button>
        <button class="program-btn" onclick="setProgram(3)">Smile Blink</button>
        <br>
        <button class="program-btn" onclick="setProgram(4)">Smile 1</button>
        <button class="program-btn" onclick="setProgram(5)">Smile 2</button>
        <button class="program-btn" onclick="setProgram(6)">Blink Fast</button>
        <button class="program-btn" onclick="setProgram(7)">Blink Medium</button>
        <br>
        <button class="program-btn" onclick="setProgram(8)">Blink Slow</button>
        <button class="program-btn" onclick="setProgram(9)">Blink Fast 2</button>
        
        <h3>Colors</h3>
        <button class="color-btn" onclick="setColor(1)">White</button>
        <button class="color-btn" onclick="setColor(2)">Red</button>
        <button class="color-btn" onclick="setColor(3)">Green</button>
        <button class="color-btn" onclick="setColor(4)">Blue</button>
        <br>
        <button class="color-btn" onclick="setColor(5)">Yellow</button>
        <button class="color-btn" onclick="setColor(6)">Cyan</button>
        <button class="color-btn" onclick="setColor(7)">Magenta</button>
        <button class="color-btn" onclick="setColor(8)">Orange</button>
        
        <h3>Brightness</h3>
        <button onclick="setBrightness(64)">25%</button>
        <button onclick="setBrightness(128)">50%</button>
        <button onclick="setBrightness(192)">75%</button>
        <button onclick="setBrightness(255)">100%</button>
    </div>
    
    <script>
        function setProgram(prog) {
            fetch('/setProgram?value=' + prog);
            updateStatus();
        }
        
        function setColor(color) {
            fetch('/setColor?value=' + color);
        }
        
        function setBrightness(brightness) {
            fetch('/setBrightness?value=' + brightness);
        }
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('currentProgram').textContent = data.program + ' (' + data.programName + ')';
                    document.getElementById('ipAddress').textContent = data.ip;
                });
        }
        
        setInterval(updateStatus, 5000);
        updateStatus();
    </script>
</body>
</html>
)rawliteral";

// Web server handlers
void handleRoot() {
    server.send_P(200, "text/html", webPage);
}

void handleSetProgram() {
    if (server.hasArg("value")) {
        int program = server.arg("value").toInt();
        if (program >= 0 && program <= 9) {
            currentProgram = program;
            Serial.print("Web: Program set to ");
            Serial.println(program);
        }
    }
    server.send(200, "text/plain", "OK");
}

void handleSetColor() {
    if (server.hasArg("value")) {
        int colorIndex = server.arg("value").toInt();
        if (colorIndex >= 0 && colorIndex < 16) {
            // Get color RGB values (using external function)
            extern void getColorRGB(int colorIndex, uint8_t* r, uint8_t* g, uint8_t* b);
            getColorRGB(colorIndex, &selected_colour_value[0], &selected_colour_value[1], &selected_colour_value[2]);
            Serial.print("Web: Color set to index ");
            Serial.println(colorIndex);
        }
    }
    server.send(200, "text/plain", "OK");
}

void handleSetBrightness() {
    if (server.hasArg("value")) {
        int brightness = server.arg("value").toInt();
        if (brightness >= 0 && brightness <= 255) {
            dma_display->setBrightness8(brightness);
            Serial.print("Web: Brightness set to ");
            Serial.println(brightness);
        }
    }
    server.send(200, "text/plain", "OK");
}

void handleStatus() {
    extern const char* getProgramName(int programIndex);
    String json = "{";
    json += "\"program\":" + String(currentProgram) + ",";
    json += "\"programName\":\"" + String(getProgramName(currentProgram)) + "\",";
    json += "\"ip\":\"" + WiFi.softAPIP().toString() + "\"";
    json += "}";
    server.send(200, "application/json", json);
}

void startWebServer() {
    if (!serverRunning) {
        // Store current display brightness to restore later
        static uint8_t originalBrightness = 50;
        
        // Temporarily reduce display brightness during WiFi setup to reduce power consumption
        originalBrightness = 50; // Store current brightness if needed
        dma_display->setBrightness8(30); // Reduce brightness temporarily
        
        // Set up WiFi AP mode with specific power settings
        WiFi.mode(WIFI_AP);
        
        // Configure WiFi with lower power to reduce interference
        esp_wifi_set_max_tx_power(44); // Reduce WiFi power (44 = 11dBm, default is 84 = 21dBm)
        
        // Set up access point with specific channel to avoid interference
        WiFi.softAP("Protogen-Control", "protogen123", 6, 0, 4); // Channel 6, visible, max 4 connections
        
        // Add small delay for WiFi to stabilize
        delay(100);
        
        // Set up web server routes
        server.on("/", handleRoot);
        server.on("/setProgram", handleSetProgram);
        server.on("/setColor", handleSetColor);
        server.on("/setBrightness", handleSetBrightness);
        server.on("/status", handleStatus);
        
        server.begin();
        serverRunning = true;
        
        // Restore original brightness
        dma_display->setBrightness8(originalBrightness);
        
        Serial.println("Web server started!");
        Serial.print("Access Point: Protogen-Control");
        Serial.print(" | IP: ");
        Serial.println(WiFi.softAPIP());
        Serial.println("WiFi power reduced to minimize display interference");
    }
}

void stopWebServer() {
    if (serverRunning) {
        server.stop();
        WiFi.softAPdisconnect(true);
        serverRunning = false;
        Serial.println("Web server stopped!");
    }
}

void Webpage_menu() {
    static unsigned long lastServerCheck = 0;
    const unsigned long SERVER_CHECK_INTERVAL = 5; // Check every 5ms instead of every loop
    
    if (Button_input_Pressed[0]) { // Enter - execute selected action
        switch (menu_cursor) {
            case 0: // Start Server
                startWebServer();
                break;
            case 1: // Stop Server
                stopWebServer();
                break;
            case 2: // Show IP
                Serial.println("=== Network Info ===");
                if (serverRunning) {
                    Serial.print("AP SSID: Protogen-Control");
                    Serial.print(" | IP: ");
                    Serial.println(WiFi.softAPIP());
                    Serial.println("Password: protogen123");
                } else {
                    Serial.println("Server is not running");
                }
                break;
        }
        selected_menu = 0; // Return to home menu
        menu_cursor = 3; // Return to Webpage menu position
    }
    
    // Handle web server requests with throttling to reduce interference
    if (serverRunning) {
        unsigned long currentTime = millis();
        if (currentTime - lastServerCheck >= SERVER_CHECK_INTERVAL) {
            server.handleClient();
            lastServerCheck = currentTime;
        }
    }
    
    // Display menu with server status
    const char* webpageMenu[] = {"Start Server", "Stop Server", "Show IP"};
    drawMenuWithCursor(webpageMenu, 3, menu_cursor, serverRunning ? "Webpage (Running)" : "Webpage");
}