#include <SocketIOClient.h>

#define LedPin LED_BUILTIN
#define D1 5
#define D2 4
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define R1 D2
#define R2 D5
#define R3 D1
#define R4 D6
#define ButtonPin 0

const char* ssid = "JimKingsmount";
const char* password = "Jim24King";

String host = "192.168.0.3";
int port = 3000;
bool clicked = false;

SocketIOClient socket;

void setupNetwork() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void click() {
  clicked = true;
}

void login (String message) {
  Serial.println("Server Login");
  socket.emit("login", "\"username\":1,\"password\":1");

}
void updateDevices(String message) {
  Serial.println(message);
  int index = message.indexOf(',');
  String device = message.substring(10, index - 1);
  String state = message.substring(index + 9, message.length());
  // Serial.println("Light:" + device + " State:" + state);
  if (device == "1") {
    if (state == "0") {
      digitalWrite(R1, HIGH);
    }
    else if (state == "1") {
      digitalWrite(R1, LOW);
    }
  }
  else  if (device == "2") {
    if (state == "0") {
      digitalWrite(R2, HIGH);
    }
    else if (state == "1") {
      digitalWrite(R2, LOW);
    }
  }
  else  if (device == "3") {
    if (state == "0") {
      digitalWrite(R3, HIGH);
    }
    else if (state == "1") {
      digitalWrite(R3, LOW);
    }
  }
  else  if (device == "4") {
    if (state == "0") {
      digitalWrite(R4, HIGH);
    }
    else if (state == "1") {
      digitalWrite(R4, LOW);
    }
  }
}

void setup() {

  pinMode(LedPin, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  pinMode(ButtonPin, INPUT);

  digitalWrite(LedPin, HIGH);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);
  Serial.begin(115200);

  setupNetwork();

  attachInterrupt(digitalPinToInterrupt(ButtonPin), click, FALLING);

  socket.on("update", updateDevices);
  socket.on("connect", login);

  socket.connect(host, port);
}

void clickCheck() {
  if (clicked) {
    Serial.println("[click]");
    socket.emit("toggle", "{\"state\":true}");
    clicked = false;
  }
}

void loop() {
  socket.monitor();
  clickCheck();
}


