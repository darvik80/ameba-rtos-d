/**
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2020, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */

#include <platform_opts.h>
#if defined(CONFIG_EXAMPLE_USBH_CDC_ACM) && CONFIG_EXAMPLE_USBH_CDC_ACM
#include <platform_stdlib.h>
#include "osdep_service.h"
#include "usbh_cdc_acm.h"
#include "usbh.h"

/* Private defines -----------------------------------------------------------*/

#define CONFIG_USBH_CDC_ACM_HOT_PLUG_TEST 0     /* Hot plug / memory leak test */
#define CONFIG_USBH_CDC_ACM_STRESS_TEST   0     /* Stress / long run test */

#define USBH_CDC_ACM_LOOPBACK_BUF_SIZE    1024  /* Buffer size for loopback test, which should match with device loopback buffer size */
#define USBH_CDC_ACM_LOOPBACK_CNT         100   /* Loopback test round */

/* Private types -------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

static u8 cdc_acm_cb_init(void);
static u8 cdc_acm_cb_deinit(void);
static u8 cdc_acm_cb_attach(void);
static u8 cdc_acm_cb_detach(void);
static u8 cdc_acm_cb_setup(void);
static u8 cdc_acm_cb_transmit(usbh_urb_state_t state);
static u8 cdc_acm_cb_receive(u8 *pbuf, u32 Len);
static u8 cdc_acm_cb_line_coding_changed(usbh_cdc_acm_line_coding_t *line_coding);
static u8 cdc_acm_cb_process(usb_host_t *host, u8 id);

/* Private variables ---------------------------------------------------------*/

static u8 cdc_acm_loopback_tx_buf[USBH_CDC_ACM_LOOPBACK_BUF_SIZE];
static u8 cdc_acm_loopback_rx_buf[USBH_CDC_ACM_LOOPBACK_BUF_SIZE];

static _sema cdc_acm_detach_sema;
static _sema cdc_acm_attach_sema;
static _sema cdc_acm_receive_sema;
static _sema cdc_acm_send_sema;

static __IO int cdc_acm_total_rx_len = 0;
static __IO int cdc_acm_is_ready = 0;

static usbh_config_t usbh_cfg = {
	.pipes = 5U,
	.speed = USB_SPEED_HIGH,
	.dma_enable = FALSE,
	.main_task_priority = 3U,
	.isr_task_priority = 4U,
	.rx_fifo_size = 0x200U,
	.nptx_fifo_size = 0x100U,
	.ptx_fifo_size = 0x100U,
};

static usbh_cdc_acm_cb_t cdc_acm_usr_cb = {
	.init = cdc_acm_cb_init,
	.deinit = cdc_acm_cb_deinit,
	.attach = cdc_acm_cb_attach,
	.detach = cdc_acm_cb_detach,
	.setup = cdc_acm_cb_setup,
	.transmit = cdc_acm_cb_transmit,
	.receive = cdc_acm_cb_receive,
	.line_coding_changed = cdc_acm_cb_line_coding_changed
};

static usbh_user_cb_t usbh_usr_cb = {
	.process = cdc_acm_cb_process
};

/* Private functions ---------------------------------------------------------*/

static u8 cdc_acm_cb_init(void)
{
	printf("\n[CDC_ACM] CDC ACM INIT\n");
	return HAL_OK;
}

static u8 cdc_acm_cb_deinit(void)
{
	printf("\n[CDC_ACM] CDC ACM DEINIT\n");
	return HAL_OK;
}

static u8 cdc_acm_cb_attach(void)
{
	printf("\n[CDC_ACM] ATTACH\n");
	rtw_up_sema(&cdc_acm_attach_sema);
	return HAL_OK;
}

static u8 cdc_acm_cb_detach(void)
{
	printf("\n[CDC_ACM] DETACH\n");
#if CONFIG_USBH_CDC_ACM_HOT_PLUG_TEST
	rtw_up_sema(&cdc_acm_detach_sema);
#endif
	return HAL_OK;
}

static u8 cdc_acm_cb_setup(void)
{
	printf("\n[CDC_ACM] SETUP\n");
	cdc_acm_is_ready = 1;
	return HAL_OK;
}

static u8 cdc_acm_cb_receive(u8 *buf, u32 length)
{
	memcpy(cdc_acm_loopback_rx_buf + cdc_acm_total_rx_len, buf, length);
	if ((cdc_acm_total_rx_len + length) >= USBH_CDC_ACM_LOOPBACK_BUF_SIZE) {
		cdc_acm_total_rx_len = 0;
		rtw_up_sema(&cdc_acm_receive_sema);
	} else {
		cdc_acm_total_rx_len += length;
	}
	return HAL_OK;
}

static u8 cdc_acm_cb_transmit(usbh_urb_state_t state)
{
	if (state == USBH_URB_DONE) {
		//printf("\n[CDC_ACM] Transmit done\n");
		rtw_up_sema(&cdc_acm_send_sema);

	} else {
		printf("\n[CDC_ACM] Transmit fail: %d\n", state);
	}
	return HAL_OK;
}

static u8 cdc_acm_cb_line_coding_changed(usbh_cdc_acm_line_coding_t *line_coding)
{
	UNUSED(line_coding);
	return HAL_OK;
}

static u8 cdc_acm_cb_process(usb_host_t *host, u8 id)
{
	UNUSED(host);

	switch (id) {
	case USBH_MSG_DISCONNECTED:
		cdc_acm_is_ready = 0;
		break;

	case USBH_MSG_CONNECTED:
		break;

	default:
		break;
	}

	return HAL_OK;
}

static void cdc_acm_loopback_test(void)
{
	int i, j, k;
	int ret;

	for (i = 0; i < USBH_CDC_ACM_LOOPBACK_BUF_SIZE; i++) {
		cdc_acm_loopback_tx_buf[i] = i % 128;
	}

	printf("\n[CDC_ACM] Wait for device attached\n");

	while (1) {
		if (cdc_acm_is_ready) {
			rtw_mdelay_os(10);
			break;
		}
	}

	printf("\n[CDC_ACM] Loopback test start, loopback times:%d, size: %d\n", USBH_CDC_ACM_LOOPBACK_CNT, USBH_CDC_ACM_LOOPBACK_BUF_SIZE);

#if CONFIG_USBH_CDC_ACM_STRESS_TEST
	while (1) {
#endif
		for (i = 0; i < USBH_CDC_ACM_LOOPBACK_CNT; i++) {
			rtw_memset(cdc_acm_loopback_rx_buf, 0, USBH_CDC_ACM_LOOPBACK_BUF_SIZE);
			if (!cdc_acm_is_ready) {
				printf("\n[CDC_ACM] Device disconnected, aborted\n");
				return;
			}
			ret = usbh_cdc_acm_transmit(cdc_acm_loopback_tx_buf, USBH_CDC_ACM_LOOPBACK_BUF_SIZE);
			if (ret < 0) {
				printf("\n[CDC_ACM] Transmit fail: %d\n", ret);
				return;
			}

			if (rtw_down_sema(&cdc_acm_send_sema)) {
				usbh_cdc_acm_receive(cdc_acm_loopback_rx_buf, USBH_CDC_ACM_LOOPBACK_BUF_SIZE);
			}

			if (rtw_down_sema(&cdc_acm_receive_sema)) {
				/*check rx data*/
				for (k = 0 ; k < USBH_CDC_ACM_LOOPBACK_BUF_SIZE; k++) {
					if (cdc_acm_loopback_rx_buf[i] != i % 128) {
						printf("\n[CDC_ACM] Loopback test FAIL: %d, %d, %d\n", i, i % 128, cdc_acm_loopback_rx_buf[i]);
						for (j = i ; j < i + 200; j++) {
							printf("%d ", cdc_acm_loopback_rx_buf[j]);
						}
						return;
					}
				}
			}
		}
		printf("\n[CDC_ACM] Loopback test PASS\n");
#if CONFIG_USBH_CDC_ACM_STRESS_TEST
	}
#endif
}

static void cdc_acm_request_test(void)
{
	u8 ret;
	usbh_cdc_acm_line_coding_t line_coding;
	usbh_cdc_acm_line_coding_t new_line_coding;

	printf("\n[CDC_ACM] Wait for device attached\n");

	while (1) {
		if (cdc_acm_is_ready) {
			rtw_mdelay_os(10);
			break;
		}
	}

	printf("\n[CDC_ACM] Request test start\n");

	printf("\n[CDC_ACM] GET_LINE_CODING:\n");
	ret = usbh_cdc_acm_get_line_coding(&line_coding);
	if (ret == HAL_OK) {
		printf("DteRate: %ld\nCharFormat: %d\nParityType: %d\nDataBits: %d\n",
			   line_coding.b.dwDteRate, line_coding.b.bCharFormat, line_coding.b.bParityType, line_coding.b.bDataBits);
	} else {
		printf("\n[CDC_ACM] GET_LINE_CODING fail\n");
	}

	printf("\n[CDC_ACM] SET_LINE_CODING:\nDteRate: 38400\nCharFormat: 0\nParityType: 0\nDataBits: 8\n");
	line_coding.b.dwDteRate = 38400;
	line_coding.b.bCharFormat = CDC_ACM_LINE_CODING_CHAR_FORMAT_1_STOP_BITS;
	line_coding.b.bParityType = CDC_ACM_LINE_CODING_PARITY_NO;
	line_coding.b.bDataBits = 8;
	ret = usbh_cdc_acm_set_line_coding(&line_coding);
	if (ret != HAL_OK) {
		printf("\n[CDC_ACM] SET_LINE_CODING fail\n");
	}

	rtw_mdelay_os(10); // delay for wait set line coding done

	printf("\n[CDC_ACM] GET_LINE_CODING:\n");
	ret = usbh_cdc_acm_get_line_coding(&new_line_coding);
	if (ret == HAL_OK) {
		printf("DteRate: %ld\nCharFormat: %d\nParityType: %d\nDataBits: %d\n",
			   new_line_coding.b.dwDteRate, new_line_coding.b.bCharFormat, new_line_coding.b.bParityType, new_line_coding.b.bDataBits);
	} else {
		printf("\n[CDC_ACM] GET_LINE_CODING fail\n");
	}

	if ((new_line_coding.b.dwDteRate == line_coding.b.dwDteRate) &&
		(new_line_coding.b.bCharFormat == line_coding.b.bCharFormat) &&
		(new_line_coding.b.bParityType == line_coding.b.bParityType) &&
		(new_line_coding.b.bDataBits == line_coding.b.bDataBits)) {
		printf("[CDC_ACM] Request test PASS\n");
	} else {
		printf("[CDC_ACM] Request test FAIL\n");
	}
}

#if CONFIG_USBH_CDC_ACM_HOT_PLUG_TEST
static void cdc_acm_hotplug_thread(void *param)
{
	int ret = 0;

	UNUSED(param);

	for (;;) {
		rtw_down_sema(&cdc_acm_detach_sema);
		rtw_mdelay_os(100);
		usbh_cdc_acm_deinit();
		usbh_deinit();
		rtw_mdelay_os(10);
		printf("\n[CDC_ACM] Free heap size: 0x%lx\n", xPortGetFreeHeapSize());

		ret = usbh_init(&usbh_cfg, &usbh_usr_cb);
		if (ret != HAL_OK) {
			printf("\n[CDC_ACM] Fail to init USB host controller: %d\n", ret);
			break;
		}

		ret = usbh_cdc_acm_init(&cdc_acm_usr_cb);
		if (ret < 0) {
			printf("\n[CDC_ACM] Fail to init USB CDC ACM host driver: %d\n", ret);
			usbh_deinit();
			break;
		}
	}

	rtw_thread_exit();
}
#endif

static void example_usbh_cdc_acm_thread(void *param)
{
	int status;
#if CONFIG_USBH_CDC_ACM_HOT_PLUG_TEST
	struct task_struct task;
#endif

	UNUSED(param);
	rtw_init_sema(&cdc_acm_detach_sema, 0);
	rtw_init_sema(&cdc_acm_attach_sema, 0);
	rtw_init_sema(&cdc_acm_receive_sema, 0);
	rtw_init_sema(&cdc_acm_send_sema, 0);
	status = usbh_init(&usbh_cfg, &usbh_usr_cb);
	if (status != HAL_OK) {
		printf("\n[CDC_ACM] Fail to init USB host controller: %d\n", status);
		goto error_exit;
	}

	status = usbh_cdc_acm_init(&cdc_acm_usr_cb);  /*0 means use default transfer size, and it can not exceed 65536*/
	if (status != HAL_OK) {
		printf("\n[CDC_ACM] Fail to init USB host cdc_acm driver: %d\n", status);
		usbh_deinit();
		goto error_exit;
	}

#if CONFIG_USBH_CDC_ACM_HOT_PLUG_TEST
	status = rtw_create_task(&task, "cdc_acm_hotplug_thread", 512, tskIDLE_PRIORITY + 3, cdc_acm_hotplug_thread, NULL);
	if (status != pdPASS) {
		printf("\n[CDC_ACM] Fail to create USBH cdc_acm memory leak check thread\n");
		usbh_cdc_acm_deinit();
		usbh_deinit();
		goto error_exit;
	}
#endif

	if (rtw_down_sema(&cdc_acm_attach_sema)) {
		cdc_acm_request_test();
		cdc_acm_loopback_test();
	}

	goto example_exit;
error_exit:
	rtw_free_sema(&cdc_acm_detach_sema);
	rtw_free_sema(&cdc_acm_attach_sema);
	rtw_free_sema(&cdc_acm_receive_sema);
	rtw_free_sema(&cdc_acm_send_sema);
example_exit:
	rtw_thread_exit();
}

/* Exported functions --------------------------------------------------------*/

void example_usbh_cdc_acm(void)
{
	int status;
	struct task_struct task;

	printf("\n[CDC_ACM] USB host cdc_acm demo started...\n");

	status = rtw_create_task(&task, "example_usbh_cdc_acm_thread", 512, tskIDLE_PRIORITY + 1, example_usbh_cdc_acm_thread, NULL);
	if (status != pdPASS) {
		printf("\n[CDC_ACM] Fail to create USB host cdc_acm thread: %d\n", status);
	}
}

#endif