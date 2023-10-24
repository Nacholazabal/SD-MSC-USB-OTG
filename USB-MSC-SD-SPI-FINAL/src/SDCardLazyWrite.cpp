#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "SDCardLazyWrite.h"

SDCardLazyWrite::SDCardLazyWrite(Stream &debug, const char *mount_point, gpio_num_t miso, gpio_num_t mosi, gpio_num_t clk, gpio_num_t cs)
: SDCardIdf(debug, mount_point, miso, mosi, clk, cs)
{
}

bool SDCardLazyWrite::writeSectors(uint8_t *src, size_t start_sector, size_t sector_count) {
  // Write data to SD card
  esp_err_t res = sdmmc_write_sectors(m_card, src, start_sector, sector_count);
  if (res != ESP_OK) {
    // Handle error
    return false;
  }
  return true;
}

bool SDCardLazyWrite::readSectors(uint8_t *dst, size_t start_sector, size_t sector_count) {
  // Read data from SD card
  esp_err_t res = sdmmc_read_sectors(m_card, dst, start_sector, sector_count);
  if (res != ESP_OK) {
    // Handle error
    return false;
  }
  return true;
}
