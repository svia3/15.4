#ifndef _FifteenDotFour_h
#define _FifteenDotFour_h

//#include <Energia.h>
#include <stdint.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <software_stack/ti15_4stack/radio_configuration/mac_user_config.h>
#include <utils/util_timer.h>
#include <application/sensor/smsgs.h>
//#include <uart_impl.h>
#include <xdc/runtime/System.h>
#include <string.h>
#include <buffer.h>

/*
 * TODO: Some of these come from jdllc.c.
 * - Should they be moved to elsewhere?
 * - Should they be configurable?
 */
#define EXTADDR_OFFSET 0x2F0
#define JDLLC_BEACON_ORDER_NON_BEACON 15
#define CONFIG_LINKQUALITY           1
#define CONFIG_PERCENTFILTER         0xFF
#define POLL_TIMEOUT_VALUE          30000
#define POLLING_INTERVAL            2000
#define RX154_MAX_BUFF_SIZE         255 // bytes

#define JOIN_EVENT (1 << 0)
#define ACCOCIATE_EVENT (1 << 1)
#define IDLE_EVENT (1 << 2)
#define POLL_EVENT (1 << 3)
#define RECV_EVENT (1 << 4)

/******************************************************************************
 Security constants and definitions
 *****************************************************************************/

#define KEY_TABLE_ENTRIES 1
#define KEY_ID_LOOKUP_ENTRIES 1
#define KEY_DEVICE_TABLE_ENTRIES 8
#define KEY_USAGE_TABLE_ENTRIES 1
#define SECURITY_LEVEL_ENTRIES 1

#define MAC_FRAME_TYPE_DATA 1
#define MAC_DATA_REQ_FRAME 4

#define AUTO_REQUEST_SEC_LEVEL 0x00

/*Serial Buffer Testing*/
//UART_Handle uart;
//Semaphore_Handle SEM_UART_rx; // handles UART recieving interrupts
//void UART00_IRQHandler(UART_Handle handle, void *buffer, size_t num);
// --------

class FifteenDotFour
{
	public:
		FifteenDotFour(void);
		void begin(bool autoJoin = false);
		void process(void);
		/* TODO: provide callback option vs polling connected()? */
		void connect();
		bool connected();
		void setChannel(uint8_t c) {channel = c;};
		uint8_t getChannel(void) {return channel;};
		void setPanID(uint16_t id) {panID = id;};
		uint16_t getPanID(void) {return panID;};
		void setPanIdMatch(bool match) {panIdMatch = match;};
		bool getPanIdMatch(void) {return panIdMatch;};
		void setCoordShortAddr(uint16_t addr) {coordShortAddr = addr;};
		uint16_t getCoordShortAddr(void) {return coordShortAddr;};
		void setSignalStrength(uint8_t signal) {signalStrength = signal;}; // kill
		uint8_t getSignalStrength(void) { return signalStrength; };

		/* ------------------------------------------------- */
		/* Buffer Handling */
		/* ------------------------------------------------- */
		/**
		 * How many bytes are available in the RX Buffer? Calculated by subtracting
		 * the (head - tail) pointers if the result is positive. If the write pointer
		 * is before the read in the circular buffer, take the length of the first and
		 * last chunk and add them together for total available bytes.
		 *
		 * @return  [description]
		 */
		uint8_t available(void);

		/**
		 * If there are bytes available in the read buffer, default extract one
		 * byte and move read pointer over by one byte (one index).
		 *
		 * @return  The read byte
		 */
		uint8_t read(void);

		/**
		 * If there are bytes available in the read buffer, read the desired amount
		 * (size) into the user passed buffer. Adjust the read pointer.
		 *
		 * @param  user_buf 	User buffer pass by pointer
		 * @param  size 			Amount of bytes to be read from rx_buffer
		 * @return      			Number of bytes read
		 */
		uint8_t read(uint8_t* user_buf, size_t size);    // pop a certain amount of bytes from queue

		/**
		 * Write a single byte to the tx_buffer. The user does not have access to the
		 * buffer as it is a 15.4 private variable.
		 *
		 * @param  w_byte Written byte
		 * @return true is successful, 0 if not
		 */
		bool write(uint8_t w_byte);

		/**
		 * Write a user defined char array to the tx_buffer. The user does not have
		 * access to the buffer as it is a 15.4 private variable. The size to write
		 * must be smaller than the amount of vacant bytes.
		 * @param  user_buf  Char array to pull bytes from to fill tx_buffer
		 * @param  size size of the bytes to push onto tx_buffer
		 * @return      [description]
		 */
		bool write(uint8_t* user_buf, size_t size);

		/**
		 * Flush, memset 0, the 15.4 class variable rx_buffer.
		 */
		void rx_flush(void);

		/**
		 * Flush, memset 0, the 15.4 class variable tx_buffer.
		 */
		void tx_flush(void);
    /* ------------------------------------------------- */

		/* static callback functions for the MAC */
		static void dataCnfCB(ApiMac_mcpsDataCnf_t *pDataCnf);
		static void dataIndCB(ApiMac_mcpsDataInd_t *pDataInd);
		static void scanCnfCB(ApiMac_mlmeScanCnf_t *pScanCnf);
		static void beaconNotifyIndCb(ApiMac_mlmeBeaconNotifyInd_t *pData);
		static void associateCnfCB(ApiMac_mlmeAssociateCnf_t *pAssocCnf);
		static void pollCnfCb(ApiMac_mlmePollCnf_t *pData);
		static void processPollTimeoutCallback(UArg instance);

	private:
		/*
		 * Private Methods
		 */
		void initializePollClock(void);
		void setPollClock(uint32_t pollTime);
        bool checkBeaconOrder(uint16_t superframeSpec);
//        void UART_initialization(Uarg arg0, Uarg arg1);
        /*
         * Bookkeeping variables
         */
		bool fh = false;
		uint8_t channel = 0;
		uint16_t panID = 0;
		/*! API MAC Callback table */
        uint16_t revents = 0;   // states
		uint16_t states = 0;    // ??
		uint8_t _macTaskId = 0;
		bool panIdMatch = false;
		uint16_t coordShortAddr = 0;
		bool _connected = false;
		Clock_Struct pollClkStruct; // Polling Clock passed into Timer API
		Clock_Handle pollClkHandle; // Polling Handle used for callback
		uint8_t signalStrength;  /* The received RF power in units dBm */

		/*
		 * Output -> Message Handling
		 */
//		Smsgs_sensorMsg_t sensor;   // Message Packet?
//		Smsgs_configReqMsg_t configSettings;    // Config for packet?

		/* Buffer Handling buffer_c API */
		buffer_t rx_buffer;
		buffer_t tx_buffer;

	protected:
		macUserCfg_t macUser0Cfg[];
		Semaphore_Handle sem;
};

#endif
