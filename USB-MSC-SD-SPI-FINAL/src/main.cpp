#include <Arduino.h>
#include "USB.h"
#include "USBMSC.h"

#include "SDCardArduino.h"
#include "SDCardLazyWrite.h"

#define BOOT_BUTTON 0

USBMSC msc;
//USBCDC Serial;

SDCard *card;

static int32_t onWrite(uint32_t lba, uint32_t offset, uint8_t *buffer, uint32_t bufsize)
{
  // This writes a complete sector so we should return sector size on success
  if (card->writeSectors(buffer, lba, bufsize / card->getSectorSize()))
  {
    return bufsize;
  }
  return -1;
}

static int32_t onRead(uint32_t lba, uint32_t offset, void *buffer, uint32_t bufsize)
{
  // This reads a complete sector so we should return sector size on success
  if (card->readSectors((uint8_t *)buffer, lba, bufsize / card->getSectorSize()))
  {
    return bufsize;
  }
  return -1;
}

static bool onStartStop(uint8_t power_condition, bool start, bool load_eject)
{
  Serial.printf("StartStop: %d %d %d\n", power_condition, start, load_eject);
  if (load_eject)
  {
    msc.end();
  }
  return true;
}

bool isBootButtonClicked()
{
  return digitalRead(BOOT_BUTTON) == LOW;
}

void setup()
{
  pinMode(GPIO_NUM_2, OUTPUT);
  card = new SDCardLazyWrite(Serial, "/sd", SD_CARD_MISO, SD_CARD_MOSI, SD_CARD_CLK, SD_CARD_CS);
  msc.vendorID("ESP32");
  msc.productID("USB_MSC");
  msc.productRevision("1.0");
  msc.onRead(onRead);
  msc.onWrite(onWrite);
  msc.onStartStop(onStartStop);
  msc.mediaPresent(true);

  msc.begin(card->getSectorCount(), card->getSectorSize());
  Serial.begin(115200);
  USB.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(200);
}
