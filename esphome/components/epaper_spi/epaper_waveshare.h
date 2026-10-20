#pragma once
#include "epaper_spi.h"
#include "epaper_spi_mono.h"

namespace esphome::epaper_spi {
/**
 * An epaper display that needs LUTs to be sent to it.
 */
class EpaperWaveshare final : public EPaperMono {
 public:
  EpaperWaveshare(const char *name, uint16_t width, uint16_t height, const uint8_t *init_sequence,
                  size_t init_sequence_length, const uint8_t *lut, size_t lut_length, const uint8_t *partial_lut,
                  uint16_t partial_lut_length, const uint8_t *full_sequence = nullptr, size_t full_sequence_length = 0,
                  const uint8_t *partial_sequence = nullptr, size_t partial_sequence_length = 0,
                  bool base_image_required = false, uint8_t partial_refresh_mode = 0x0F,
                  bool sleep_between_partial_updates = true)
      : EPaperMono(name, width, height, init_sequence, init_sequence_length),
        lut_(lut),
        partial_lut_(partial_lut),
        full_sequence_(full_sequence),
        partial_sequence_(partial_sequence),
        lut_length_(lut_length),
        full_sequence_length_(full_sequence_length),
        partial_sequence_length_(partial_sequence_length),
        partial_lut_length_(partial_lut_length),
        base_image_required_(base_image_required),
        partial_refresh_mode_(partial_refresh_mode),
        sleep_between_partial_updates_(sleep_between_partial_updates) {
    this->send_red_as_image_ = base_image_required;
  }

 protected:
  bool initialise(bool partial) override;
  void set_window() override;
  void refresh_screen(bool partial) override;
  void deep_sleep() override;
  bool reset() override;
  // True while the base image in the controller RAM must be preserved for the next update.
  bool must_keep_base_image_() const { return this->base_image_required_ && this->update_count_ != 0; }
  const uint8_t *lut_;
  const uint8_t *partial_lut_;
  // Commands sent after the LUT, e.g. voltage settings that differ between full and partial updates.
  const uint8_t *full_sequence_;
  const uint8_t *partial_sequence_;
  size_t lut_length_;
  size_t full_sequence_length_;
  size_t partial_sequence_length_;
  uint16_t partial_lut_length_;
  bool base_image_required_;
  // Display update control 2 (0x22) value for a partial refresh; it differs between Waveshare's samples.
  uint8_t partial_refresh_mode_;
  bool sleep_between_partial_updates_;
};
}  // namespace esphome::epaper_spi
