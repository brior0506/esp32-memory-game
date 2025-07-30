#include <WiFi.h>
#include <WebServer.h>
#include <vector>

//defines each pin corresponding to color of led
#define blueLEDpin 22
#define redLEDpin 4
#define greenLEDpin 18
#define yellowLEDpin 23


const char* user = "ASUS";
const char* password = "122276112470";

WebServer server(80);

String htmlPage = R"rawliteral(
  <html>
  <head>
  <title>Memory Game ESP32</title>
  </head>
  <body>
  <h1>Buttons</h1>
  <button onclick="sendClick('/yellow')" style = "background-color: yellow; border-radius: 12px; padding: 10px 20px; font-size: 16px; border: none;">Yellow</button>
  <button onclick="sendClick('/green')" style = "background-color: green; border-radius: 12px; padding: 10px 20px; font-size: 16px; border: none;">Green</button>
  <button onclick="sendClick('/red')" style = "background-color: red; border-radius: 12px; padding: 10px 20px; font-size: 16px; border: none;">Red</button>
  <button onclick="sendClick('/blue')" style = "background-color: blue; border-radius: 12px; padding: 10px 20px; font-size: 16px; border: none;">Blue</button><br><br>
  <button onclick="fetch('/reset')">Reset Game</button>
  </body>
  </html>

  <script>
  let clickLocked = false;
  function sendClick(path){
    if (clickLocked) return;
    clickLocked = true;
    fetch(path).then(() => {
      setTimeout(() => {clickLocked = false }, 300);
    });
  }
  </script>
)rawliteral";

std::vector<int> memory;
std::vector<int> pins = {yellowLEDpin, greenLEDpin, redLEDpin, blueLEDpin};
volatile int answer = 0;
volatile bool clicked = false;
volatile bool clicked_lock = false;
volatile bool game_over = false;
volatile bool accepting_clicks = false;


//handle root of server

void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void handleYellow(){
  if(accepting_clicks && !clicked_lock && answer == -1){
  answer = yellowLEDpin;
  clicked = true;
  clicked_lock = true;
  digitalWrite(answer, HIGH);
  delay(200);
  digitalWrite(answer, LOW);
  Serial.println("Yellow clicked");
  Serial.print("Answer set to: ");
  Serial.println(answer);
  }
  else{
    Serial.println("Yellow not registered");
  }

  server.send(200, "text/plain", "OK");
}
void handleGreen(){
  if(accepting_clicks && !clicked_lock && answer == -1){
  answer = greenLEDpin;
  clicked = true;
  clicked_lock = true;
  digitalWrite(answer, HIGH);
  delay(200);
  digitalWrite(answer, LOW);
  Serial.println("Green clicked");
  Serial.print("Answer set to: ");
  Serial.println(answer);
  }
  else{
    Serial.println("Green not registered");
  }

  server.send(200, "text/plain", "OK");
}
void handleRed(){
  if(accepting_clicks && !clicked_lock && answer == -1){
  answer = redLEDpin;
  clicked = true;
  clicked_lock = true;
  digitalWrite(answer, HIGH);
  delay(200);
  digitalWrite(answer, LOW);
  Serial.println("Red clicked");
  Serial.print("Answer set to: ");
  Serial.println(answer);
  }
  else{
    Serial.println("Red not registered");
  }

  server.send(200, "text/plain", "OK");
}
void handleBlue(){
  if(accepting_clicks && !clicked_lock && answer == -1){
  answer = blueLEDpin;
  clicked = true;
  clicked_lock = true;
  digitalWrite(answer, HIGH);
  delay(200);
  digitalWrite(answer, LOW);
  Serial.println("Blue clicked");
  Serial.print("Answer set to: ");
  Serial.println(answer);
  }
  else{
    Serial.println("Blue not registered");
  }

  server.send(200, "text/plain", "OK");
}

void handleReset(){
  game_over = false;
  clicked = false;
  memory.clear();
  digitalWrite(blueLEDpin, LOW);
  digitalWrite(redLEDpin, LOW);
  digitalWrite(yellowLEDpin, LOW);
  digitalWrite(greenLEDpin, LOW);
}


void gameOver(){
  
  digitalWrite(blueLEDpin, HIGH);
  digitalWrite(yellowLEDpin, HIGH);
  digitalWrite(redLEDpin, HIGH);
  digitalWrite(greenLEDpin, HIGH);
  delay(100);
  digitalWrite(blueLEDpin, LOW);
  digitalWrite(redLEDpin, LOW);
  digitalWrite(yellowLEDpin, LOW);
  digitalWrite(greenLEDpin, LOW);
  delay(100);
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  // begin wifi server
  WiFi.begin(user, password);


  server.on("/", handleRoot);

  server.on("/yellow", handleYellow);

  server.on("/green", handleGreen);

  server.on("/red", handleRed);

  server.on("/blue", handleBlue);

  server.on("/reset", handleReset);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(500);
  }


  Serial.println("\nConnected to WiFi");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());

  

  server.begin();

  pinMode(blueLEDpin, OUTPUT);
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  pinMode(yellowLEDpin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  int id = random(0, 4);
  memory.push_back(pins[id]);
  delay(500);
  for(int i = 0; i< memory.size(); i += 1){
    digitalWrite(memory[i], HIGH);
    delay(700);
    digitalWrite(memory[i], LOW);
    delay(500);
  }
  delay(1000);

  int i = 0;
  game_over = false;
  while(i < memory.size()){
    answer = -1;
    clicked = false;
    clicked_lock = false;
    accepting_clicks = true;
    while(!clicked){
      server.handleClient();
      delay(10);
      Serial.println(memory[i]);
    }

    accepting_clicks = false;

    Serial.print("Excpected: ");
    Serial.println(memory[i]);
    Serial.print("Got: ");
    Serial.println(answer);
    if (answer != memory[i]){
        game_over = true;
      }
    
    delay(300);
    i += 1;
    delay(500);
    if(game_over){
      break;
    }
    
  }
  while(game_over){
    server.handleClient();
    gameOver();
  }
  
}
