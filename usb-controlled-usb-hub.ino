#define RELAY_PIN (12)

void setup() {
  // these are the ID pins, 2-5
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);

  // relay pin
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // initialize digital pin LED_BUILTIN as an output.
//  pinMode(LED_BUILTIN, OUTPUT);
  
  // Set baud
  Serial.begin(115200);
}

// reads pins 2 3 4 5 and makes a one hot from them
// with possible return values 0-3 inclusive
// the lowest numbered pin found to be pulled low, gets returned first
// and the others are not checked
int getBoardId() {
  int id = 0;
  for(int i = 0; i < 4; i++) {
    const bool io = digitalRead(2 + i);
    if(!io) {
      return i;
    }
  }  
  return id;
}

// the loop function runs over and over again forever
void loop() {
  char buf[16];

  // current status of the relay
  static bool current = false;

  // Check if data is available to read.
  if (Serial.available() > 0) {
    // Read the incoming byte.
    char receivedChar = Serial.read();

    if (receivedChar == 'a') {
      // If 'a' was received, turn the RELAY off.
      digitalWrite(RELAY_PIN, LOW);
      current = false;
//      digitalWrite(LED_BUILTIN, HIGH);
    } else if (receivedChar == 'b') {
      // If 'b' was received, turn the RELAY on.
      digitalWrite(RELAY_PIN, HIGH);
      current = true;
    } else if (receivedChar == 'i') {
      // Write the board id
      const int id = getBoardId();
      snprintf(buf, 16, "%d", id);
      Serial.write(buf);
//      Serial.write('0');
    } else if (receivedChar == 's') {
      // Write the current relay status back, using the same 'a'/'b'
      // encoding
      snprintf(buf, 16, "%c", current?'b':'a');
      Serial.write(buf);
    }
  }
}
