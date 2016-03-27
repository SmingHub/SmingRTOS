#include <SmingCore.h>
#include <libraries/rfid_mfrc522/rfid_mfrc522.h>

#define RST_PIN	5 // RST-PIN für RC522 - RFID - SPI - Modul GPIO5
#define SS_PIN	16  // SDA-PIN für RC522 - RFID - SPI - Modul GPI16

MFRC522 mfrc522(SS_PIN, RST_PIN);	// Create MFRC522 instance

//----------------------------------
// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : ":");
    Serial.print(buffer[i], HEX);
  }
}

void IRAM_ATTR
scanNfc(void *pvParameters ){
	Serial.println("\n------NFC Task is started------\n");
	Serial.print("Module Version: ");
	byte readReg = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
	Serial.println(readReg, HEX);
	Serial.println("\n---------------------------------\n");
	while(1){
	static int cnt;
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
	    cnt++;
	    if(cnt>20){
	      cnt=0;
	     // Serial.print(".");
	    }

	  }else if ( ! mfrc522.PICC_ReadCardSerial()) {	// Select one of the cards
		  	    Serial.println("Selecting card failed...");

		 }else{
			  // Show some details of the PICC (that is: the tag/card)
			  Serial.print("Card UID:");
			  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
			  Serial.println();

			  vTaskDelay(100);
		 }
	vTaskDelay(10);
	}

}

void init()
{
	Serial.begin(115200); // 115200 by default

	//----- NFC
	SPI.begin();
	mfrc522.PCD_Init();    // Init MFRC522

	xTaskCreate(scanNfc, (const signed char*)"readrfid", 256*2, NULL, tskIDLE_PRIORITY, NULL);
}
