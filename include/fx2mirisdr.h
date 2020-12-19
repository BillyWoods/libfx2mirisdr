/*
 * Copyright (C) 2012 by Steve Markgraf <steve@steve-m.de>
 * Copyright (C) 2012 by Dimitri Stolnikov <horiz0n@gmx.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __FX2MIRISDR_H
#define __FX2MIRISDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <fx2mirisdr_export.h>

typedef struct fx2mirisdr_dev fx2mirisdr_dev_t;

FX2MIRISDR_API uint32_t fx2mirisdr_get_device_count(void);

FX2MIRISDR_API const char* fx2mirisdr_get_device_name(uint32_t index);

/*!
 * Get USB device strings.
 *
 * NOTE: The string arguments must provide space for up to 256 bytes.
 *
 * \param index the device index
 * \param manufact manufacturer name, may be NULL
 * \param product product name, may be NULL
 * \param serial serial number, may be NULL
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_get_device_usb_strings(uint32_t index,
					       char *manufact,
					       char *product,
					       char *serial);

FX2MIRISDR_API int fx2mirisdr_open(fx2mirisdr_dev_t **dev, uint32_t index);

FX2MIRISDR_API int fx2mirisdr_close(fx2mirisdr_dev_t *dev);

/* configuration functions */

/*!
 * Get USB device strings.
 *
 * NOTE: The string arguments must provide space for up to 256 bytes.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param manufact manufacturer name, may be NULL
 * \param product product name, may be NULL
 * \param serial serial number, may be NULL
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_get_usb_strings(fx2mirisdr_dev_t *dev, char *manufact,
					char *product, char *serial);

/*!
 * Set the frequency the device is tuned to.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param freq frequency in Hz the device should be tuned to
 * \return 0 on error, frequency in Hz otherwise
 */
FX2MIRISDR_API int fx2mirisdr_set_center_freq(fx2mirisdr_dev_t *dev, uint32_t freq);

/*!
 * Get the actual frequency the device is tuned to.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \return 0 on error, frequency in Hz otherwise
 */
FX2MIRISDR_API uint32_t fx2mirisdr_get_center_freq(fx2mirisdr_dev_t *dev);

/*!
 * Get a list of gains supported by the tuner.
 *
 * NOTE: The gains argument must be preallocated by the caller. If NULL is
 * being given instead, the number of available gain values will be returned.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param gains array of gain values. In tenths of a dB, 115 means 11.5 dB.
 * \return <= 0 on error, number of available (returned) gain values otherwise
 */
FX2MIRISDR_API int fx2mirisdr_get_tuner_gains(fx2mirisdr_dev_t *dev, int *gains);

/*!
 * Set the gain for the device.
 * Manual gain mode must be enabled for this to work.
 *
 * Valid gain values may be queried with \ref fx2mirisdr_get_tuner_gains function.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param gain in tenths of a dB, 115 means 11.5 dB.
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_set_tuner_gain(fx2mirisdr_dev_t *dev, int gain);

/*!
 * Get actual gain the device is configured to.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \return 0 on error, gain in tenths of a dB, 115 means 11.5 dB.
 */
FX2MIRISDR_API int fx2mirisdr_get_tuner_gain(fx2mirisdr_dev_t *dev);

/*!
 * Set the gain mode (automatic/manual) for the device.
 * Manual gain mode must be enabled for the gain setter function to work.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param manual gain mode, 1 means manual gain mode shall be enabled.
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_set_tuner_gain_mode(fx2mirisdr_dev_t *dev, int manual);

/*!
 * Set the sample rate for the device.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param rate the sample rate in Hz
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_set_sample_rate(fx2mirisdr_dev_t *dev, uint32_t rate);

/*!
 * Get the sample rate the device is configured to.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \return 0 on error, sample rate in Hz otherwise
 */
FX2MIRISDR_API uint32_t fx2mirisdr_get_sample_rate(fx2mirisdr_dev_t *dev);

/* streaming functions */

FX2MIRISDR_API int fx2mirisdr_reset_buffer(fx2mirisdr_dev_t *dev);

FX2MIRISDR_API int fx2mirisdr_read_sync(fx2mirisdr_dev_t *dev, void *buf, int len, int *n_read);

typedef void(*fx2mirisdr_read_async_cb_t)(unsigned char *buf, uint32_t len, void *ctx);

/*!
 * Read samples from the device asynchronously. This function will block until
 * it is being canceled using fx2mirisdr_cancel_async()
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \param cb callback function to return received samples
 * \param ctx user specific context to pass via the callback function
 * \param buf_num optional buffer count, buf_num * buf_len = overall buffer size
 *		  set to 0 for default buffer count (32)
 * \param buf_len optional buffer length, must be multiple of 512,
 *		  set to 0 for default buffer length (16 * 32 * 512)
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_read_async(fx2mirisdr_dev_t *dev,
				 fx2mirisdr_read_async_cb_t cb,
				 void *ctx,
				 uint32_t buf_num,
				 uint32_t buf_len);

/*!
 * Cancel all pending asynchronous operations on the device.
 *
 * \param dev the device handle given by fx2mirisdr_open()
 * \return 0 on success
 */
FX2MIRISDR_API int fx2mirisdr_cancel_async(fx2mirisdr_dev_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* __FX2MIRISDR_H */
