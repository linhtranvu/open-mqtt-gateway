/*  
  OpenMQTTGateway Addon  - ESP8266 or Arduino program for home automation 

   Act as a wifi or ethernet gateway between your 433mhz/infrared IR signal  and a MQTT broker 
   Send and receiving command by MQTT
 
    HC SR-501 reading Addon
  
    Copyright: (c)Florian ROBERT
    
    Contributors:
    - 1technophile
  
    This file is part of OpenMQTTGateway.
    
    OpenMQTTGateway is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenMQTTGateway is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef ZsensorLightMod

void setupLightMod() {
  pinMode(LightMod_PIN, INPUT);     // declare HC SR-501 pin as input
}
//Time used to wait for an interval before resending data from LightMod
unsigned long timeLightMod = 0;

void MeasureLightMod(){
  if (millis() > (timeLightMod + TimeBeforeStartLightMod)) {//let time to init the PIR sensor
  timeLightMod = millis();
  trc(F("Creating LightMod buffer"));
    const int JSON_MSG_CALC_BUFFER = JSON_OBJECT_SIZE(1);
    StaticJsonBuffer<JSON_MSG_CALC_BUFFER> jsonBuffer;
  JsonObject& LightModdata = jsonBuffer.createObject();
  static int lighModState = LOW;
  static int firstUse = 1;
  int PresenceValue = digitalRead(LightMod_PIN);
  #if defined(ESP8266) || defined(ESP32)
    yield();
  #endif
  if (PresenceValue == HIGH) { 
		if (lighModState == LOW ||firstUse == 1) {
		  //dark light
		  LightModdata.set("LightMod", "false");
		  lighModState = HIGH;
		  firstUse = 0;
		}
    } else {
		if (lighModState == HIGH ||firstUse == 1){
		  // bright light
		  LightModdata.set("LightMod", "true");
		  trc(F("LightMod ended"));
		  lighModState = LOW;
		  firstUse = 0;
		}
    }
    if(LightModdata.size()>0) pub(subjectLightModtoMQTT,LightModdata);
  }
}
#endif
