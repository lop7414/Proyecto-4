/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server
  Basándose en los ejemplos de:
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://electropeak.com/learn
**************************************************************************************************/

//************************************************************************************************
// Librerías
//************************************************************************************************

#include <WiFi.h>
#include <WebServer.h>

//************************************************************************************************
// Variables globales
//************************************************************************************************

// SSID & Password
const char* ssid = "TRENDnet652";  // Enter your SSID here
const char* password = "";  //Enter your Password here

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

uint8_t TOTAL;
unsigned char VAR;
String Ocupado = "\t  <td  style=\"background-color: #e0120b;\"><h2>Ocupado</h2></td>\t\n";
String Vacio = "\t  <td  style=\"background-color: #3fb528;\"> <h2>Vacio</h2> </td>\t\n";

//************************************************************************************************
// Configuración
//************************************************************************************************

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  while (! Serial2);

  Serial.println("Connecting to ");
  Serial.println(ssid);

//  pinMode(LED1pin, OUTPUT);

  // Connect to your wi-fi modem
  WiFi.begin(ssid, password);

  // Check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());  //Show ESP32 IP on serial

  server.on("/", handle_OnConnect); // Directamente desde e.g. 192.168.0.8

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  delay(100);
}

//************************************************************************************************
// loop principal
//************************************************************************************************

void loop() {
  //leer UART2
  if (Serial2.available() > 0) {
    VAR = Serial2.read(); 
  }
  TOTAL = 4;

  //Cantidad de parqueos Vacios
  if (1 & VAR) {
    TOTAL -= 1;
  }
  if (2 & VAR) {
    TOTAL -= 1;
  }
  if (4 & VAR) {
    TOTAL -= 1;
  }
  if (8 & VAR) {
    TOTAL -= 1;
  }
  
  server.handleClient();
}

//************************************************************************************************
// Handler de Inicio página
//************************************************************************************************

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML2());
}

//************************************************************************************************
// Procesador de HTML
//************************************************************************************************

String SendHTML2() {
  String code = "<!DOCTYPE html> \n";
  code += "\t<html>\n";
  code += "  <head><meta name=\\\"viewport\\\" content=\\\"width=device-width, initial-scale=1.0, user-scalable=no\\\">\n";
  code += "  <title>Parqueomatic</title>\n";
  code += "  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  code += "  body{margin-top: 50px;} h1 {color: #1ac9c1;margin: 50px auto 30px;} h3 {color: #1ac9c1;margin-bottom: 50px;}\n";
  code += "  p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  code += "  div {\n";
  code += "  width: 180px;\n";
  code += "  padding: 30px;\n";
  code += "  font-family: sans-serif;\n";
  code += "  background-color: gold;\n";
  code += "    }\n";
  code += "  \n";
  code += "table {\n";
  code += "  width:70%;\n";
  code += "  \n";
  code += "}\n";
  code += "table, th, td {\n";
  code += "  border: 3px white;\n";
  code += "  border-collapse: collapse;\n";
  code += "}\n";
  code += "th, td {\n";
  code += "  padding: 5px;\n";
  code += "  text-align: center;\n";
  code += "}\n";
  code += "#t01 td:nth-child(odd) {\n";
  code += "  background-color: #552df7;\n";
  code += "}\n";
  code += "#t01 th {\n";
  code += "  background-color: black;\n";
  code += "  color: white;\n";
  code += "}\n";
  code += "  </style>\n";

//refresh rate 0.5s
  code += "<script>\n";
  code += "<!--\n";
  code += "function timedRefresh(timeoutPeriod) {\n";
  code += "\tsetTimeout(\"location.reload(true);\",timeoutPeriod);\n";
  code += "}\n";
  code += "\n";
  code += "window.onload = timedRefresh(500);\n";
  code += "\n";
  code += "//   -->\n";
  code += "</script>";

//Codigo del visualizador principal de la pagina web
  code += "  </head>\n";
  code += "  <body>\n";
  code += "  <h1>Parqueo-matic</h1>\n";
  code += "  <h3>Francisco Lopez 17414</h3>\n";
  code += "  <table id=\"t01\" align=\"center\">\n";

//Tabla

  code += "\t<tr>\n";

  code += "\t  <th><h2> Parqueo 1 </h2></th>\n";
  code += "\t  <td><h2> Parqueo 2 </h2></td>\n";
  code += "\t  <th><h2> Parqueo 3 </h2></th>\n";
  code += "\t  <td><h2> Parqueo 4 </h2></td>\n";

  code += "\t</tr>\n";
  code += "\t<tr>\n";
  
  if ((VAR & 1) == 1) {
    code += Ocupado;
  }
  else {
    code += Vacio;
  }

  if ((VAR & 2) == 2) {
    code += Ocupado;
  }
  else {
    code += Vacio;
  }

  if ((VAR & 4) == 4) {
    code += Ocupado;
  }
  else {
    code += Vacio;
  }

  if ((VAR & 8) == 8) {
    code += Ocupado;
  }
  else {
    code += Vacio;
  }
  
  code += "\t</tr>\n";
 
  code += "  </table>\n";
  code += "  <h2>Parqueos Vacios:</h2>\n";
  code += "  <font size=7><span style=\"border: 3px warning\">" + String(TOTAL) + "</span></font>\n";
  code += "\t</body>\n";
  code += "    </html>";
  return code;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}
