/**
 * Copyright (c) 2016-2017  Zubax Robotics  <info@zubax.com>
 */

#include <board/usb/usb.hpp>
#include <board/board.hpp>
#include <ch.hpp>
#include <hal.h>
#include <cstdint>
#include <cstring>


namespace board
{
namespace usb
{

using std::uint8_t;
using std::uint16_t;

namespace
{
/*
 * VID/PID
 */
constexpr unsigned USB_VID = 0x1d50;
constexpr unsigned USB_PID = 0x60c7;

/*
 * Endpoints to be used for USBD1.
 */
constexpr unsigned USBD1_SER_INTERRUPT_REQUEST_EP  = 1;
constexpr unsigned USBD1_SER_DATA_REQUEST_EP       = 2;
constexpr unsigned USBD1_SER_DATA_AVAILABLE_EP     = 2;

/*
 * USB Device Descriptor.
 */
const uint8_t vcom_device_descriptor_data[18] =
{
    USB_DESC_DEVICE       (0x0110,        // bcdUSB (1.1).
                           0x02,          // bDeviceClass (CDC).
                           0x00,          // bDeviceSubClass.
                           0x00,          // bDeviceProtocol.
                           0x40,          // bMaxPacketSize.
                           USB_VID,       // idVendor.
                           USB_PID,       // idProduct.
                           0x0200,        // bcdDevice.
                           1,             // iManufacturer.
                           2,             // iProduct.
                           3,             // iSerialNumber.
                           1)             // bNumConfigurations.
};

/*
 * Device Descriptor wrapper.
 */
static const USBDescriptor vcom_device_descriptor =
{
    sizeof(vcom_device_descriptor_data),
    vcom_device_descriptor_data
};

constexpr unsigned CONFIGURATION_DESCRIPTOR_SIZE = 67;

/*
 * Configuration Descriptor tree.
 * Relevant sources:
 *  - http://www.usbmadesimple.co.uk/ums_4.htm
 *  - http://forum.chibios.org/phpbb/viewtopic.php?f=2&t=1118&sid=e4551c88ca268e88bcb12be6aa582219&start=20
 *  - http://forum.chibios.org/phpbb/viewtopic.php?f=3&t=699
 *  - http://electronics.stackexchange.com/questions/197434/usb-descriptor-of-a-dual-powered-device
 */
static const uint8_t vcom_configuration_descriptor_data[] = {
    /*
     * Configuration Descriptor.
     */
    USB_DESC_CONFIGURATION(CONFIGURATION_DESCRIPTOR_SIZE, // wTotalLength.
                           0x02,          // bNumInterfaces.
                           0x01,          // bConfigurationValue.
                           0,             // iConfiguration.
                           0b10000000,    // bmAttributes (bus powered). See above.
                           50),           // bMaxPower. See above.
    /*
     * Interface Descriptor 0.
     */
    USB_DESC_INTERFACE    (0x00,          // bInterfaceNumber.
                           0x00,          // bAlternateSetting.
                           0x01,          // bNumEndpoints.
                           0x02,          // bInterfaceClass (Communications Interface Class, CDC section 4.2).
                           0x02,          // bInterfaceSubClass (Abstract Control Model, CDC section 4.3).
                           0x01,          // bInterfaceProtocol (AT commands, CDC section 4.4).
                           0),            // iInterface.
    // Header Functional Descriptor (CDC section 5.2.3).
    USB_DESC_BYTE         (5),            // bLength.
    USB_DESC_BYTE         (0x24),         // bDescriptorType (CS_INTERFACE).
    USB_DESC_BYTE         (0x00),         // bDescriptorSubtype (Header Functional Descriptor).
    USB_DESC_BCD          (0x0110),       // bcdCDC.
    // Call Management Functional Descriptor.
    USB_DESC_BYTE         (5),            // bFunctionLength.
    USB_DESC_BYTE         (0x24),         // bDescriptorType (CS_INTERFACE).
    USB_DESC_BYTE         (0x01),         // bDescriptorSubtype (Call Management Functional Descriptor).
    USB_DESC_BYTE         (0x00),         // bmCapabilities (D0+D1).
    USB_DESC_BYTE         (0x01),         // bDataInterface.
    // ACM Functional Descriptor.
    USB_DESC_BYTE         (4),            // bFunctionLength.
    USB_DESC_BYTE         (0x24),         // bDescriptorType (CS_INTERFACE).
    USB_DESC_BYTE         (0x02),         // bDescriptorSubtype (Abstract Control Management Descriptor).
    USB_DESC_BYTE         (0x02),         // bmCapabilities.
    // Union Functional Descriptor.
    USB_DESC_BYTE         (5),            // bFunctionLength.
    USB_DESC_BYTE         (0x24),         // bDescriptorType (CS_INTERFACE).
    USB_DESC_BYTE         (0x06),         // bDescriptorSubtype (Union Functional Descriptor).
    USB_DESC_BYTE         (0x00),         // bMasterInterface (Communication Class Interface).
    USB_DESC_BYTE         (0x01),         // bSlaveInterface0 (Data Class Interface).
    // Endpoint Descriptor.
    USB_DESC_ENDPOINT     (USBD1_SER_INTERRUPT_REQUEST_EP|0x80,
                           0x03,          // bmAttributes (Interrupt).
                           0x0008,        // wMaxPacketSize.
                           0x02),         // bInterval.
    /*
     * Interface Descriptor 1.
     */
    USB_DESC_INTERFACE    (0x01,          // bInterfaceNumber.
                           0x00,          // bAlternateSetting.
                           0x02,          // bNumEndpoints.
                           0x0A,          // bInterfaceClass (Data Class Interface, CDC section 4.5).
                           0x00,          // bInterfaceSubClass (CDC section 4.6).
                           0x00,          // bInterfaceProtocol (CDC section 4.7).
                           0x00),         // iInterface.
    // Endpoint Descriptor.
    USB_DESC_ENDPOINT     (USBD1_SER_DATA_AVAILABLE_EP,       // bEndpointAddress.
                           0x02,          // bmAttributes (Bulk).
                           0x0040,        // wMaxPacketSize.
                           0x00),         // bInterval.
    // Endpoint Descriptor.
    USB_DESC_ENDPOINT     (USBD1_SER_DATA_REQUEST_EP|0x80,    // bEndpointAddress.
                           0x02,          // bmAttributes (Bulk).
                           0x0040,        // wMaxPacketSize.
                           0x00)          // bInterval.
};

static_assert(sizeof(vcom_configuration_descriptor_data) == CONFIGURATION_DESCRIPTOR_SIZE,
              "Unexpected USB descriptor length");

/*
 * Configuration Descriptor wrapper.
 */
static const USBDescriptor vcom_configuration_descriptor =
{
    sizeof(vcom_configuration_descriptor_data),
    vcom_configuration_descriptor_data
};

/*
 * U.S. English language identifier.
 */
static const uint8_t usb_string0[] =
{
    USB_DESC_BYTE(4),                     // bLength.
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), // bDescriptorType.
    USB_DESC_WORD(0x0409)                 // wLANGID (U.S. English).
};

/*
 * Vendor string.
 */
static const uint8_t usb_string1[] =
{
    USB_DESC_BYTE(30),                    // bLength.
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), // bDescriptorType.
    'Z', 0, 'u', 0, 'b', 0, 'a', 0, 'x', 0,
    ' ', 0,
    'R', 0, 'o', 0, 'b', 0, 'o', 0, 't', 0, 'i', 0, 'c', 0, 's', 0
};

/*
 * Device Description string.
 */
static const uint8_t usb_string2[] =
{
    USB_DESC_BYTE(22),                    // bLength.
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), // bDescriptorType.
    'B', 0, 'o', 0, 'o', 0, 't', 0, 'l', 0, 'o', 0, 'a', 0, 'd', 0, 'e', 0, 'r', 0
};

/*
 * Serial Number string.
 * Will be configured at run time during initialization.
 */
static uint8_t usb_string3[] =
{
    USB_DESC_BYTE(66),                    // bLength.
    USB_DESC_BYTE(USB_DESCRIPTOR_STRING), // bDescriptorType.
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, // 8
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, // 16
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, // 24
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0  // 32
};

/*
 * Strings wrappers array.
 */
static const USBDescriptor usb_strings[] =
{
    {sizeof(usb_string0), usb_string0},
    {sizeof(usb_string1), usb_string1},
    {sizeof(usb_string2), usb_string2},
    {sizeof(usb_string3), usb_string3}
};

/*
 * Handles the GET_DESCRIPTOR callback. All required descriptors must be
 * handled here.
 */
static const USBDescriptor *get_descriptor(USBDriver *usbp, uint8_t dtype, uint8_t dindex, uint16_t lang)
{
    (void) usbp;
    (void) lang;
    switch (dtype)
    {
    case USB_DESCRIPTOR_DEVICE:
    {
        return &vcom_device_descriptor;
    }
    case USB_DESCRIPTOR_CONFIGURATION:
    {
        return &vcom_configuration_descriptor;
    }
    case USB_DESCRIPTOR_STRING:
    {
        if (dindex < 4)
        {
            return &usb_strings[dindex];
        }
        break;
    }
    default:
    {
        return nullptr;
    }
    }
    return nullptr;
}

/*
 * USB endpoints
 */
static USBInEndpointState ep1instate;           ///< IN EP1 state.
static const USBEndpointConfig ep1config =      ///< EP1 initialization structure (IN only).
{
    USB_EP_MODE_TYPE_INTR,
    nullptr,
    sduInterruptTransmitted,
    nullptr,
    0x0010,
    0x0000,
    &ep1instate,
    nullptr,
    1,
    nullptr
};

static USBInEndpointState ep2instate;           ///< IN EP1 state.
static USBOutEndpointState ep2outstate;         ///< OUT EP1 state.
static const USBEndpointConfig ep2config =      ///< EP1 initialization structure (both IN and OUT).
{
    USB_EP_MODE_TYPE_BULK,
    nullptr,
    sduDataTransmitted,
    sduDataReceived,
    0x0040,
    0x0040,
    &ep2instate,
    &ep2outstate,
    1,
    nullptr
};

/*
 * Serial over USB Driver structures.
 */
static SerialUSBDriver SDU1;

/*
 * Handles the USB driver global events.
 */
static void usb_event(USBDriver* usbp, usbevent_t event)
{
    switch (event)
    {
    case USB_EVENT_CONFIGURED:
    {
        chSysLockFromISR();

        /*
         * Enables the endpoints specified into the configuration.
         * Note, this callback is invoked from an ISR so I-Class functions
         * must be used.
         */
        usbInitEndpointI(usbp, USBD1_SER_INTERRUPT_REQUEST_EP, &ep1config);
        usbInitEndpointI(usbp, USBD1_SER_DATA_REQUEST_EP,      &ep2config);

        // Resetting the state of the CDC subsystem.
        sduConfigureHookI(&SDU1);

        chSysUnlockFromISR();
        return;
    }

    case USB_EVENT_RESET:
    case USB_EVENT_ADDRESS:
    case USB_EVENT_UNCONFIGURED:
    case USB_EVENT_SUSPEND:
    case USB_EVENT_WAKEUP:
    case USB_EVENT_STALLED:
    {
        return;
    }
    }
}

static void sof_handler(USBDriver*)
{
    chSysLockFromISR();
    sduSOFHookI(&SDU1);
    chSysUnlockFromISR();
}

/*
 * USB driver configuration.
 */
static const USBConfig usbcfg =
{
    usb_event,
    get_descriptor,
    sduRequestsHook,
    sof_handler
};

/*
 * Serial over USB driver configuration.
 */
static const SerialUSBConfig serusbcfg1 =
{
    &USBD1,
    USBD1_SER_DATA_REQUEST_EP,
    USBD1_SER_DATA_AVAILABLE_EP,
    USBD1_SER_INTERRUPT_REQUEST_EP
};

}

void init()
{
    {
        static const auto nibble2hex = [](std::uint8_t x) -> char
        {
            const char n = char((x & 0xF) + '0');
            return (n > '9') ? char(n + 'A' - '9' - 1) : n;
        };

        unsigned write_pos = 2;
        for (auto x : readUniqueID())
        {
            usb_string3[write_pos++] = nibble2hex(std::uint8_t(x >> 4));
            write_pos++;
            usb_string3[write_pos++] = nibble2hex(x);
            write_pos++;
        }
    }

    sduObjectInit(&SDU1);
    sduStart(&SDU1, &serusbcfg1);

    /*
     * Activate the USB driver and then the USB bus pull-up on D+.
     * A delay is inserted in order to not have to disconnect the cable after a reset.
     */
    usbDisconnectBus(&USBD1);
    chThdSleepSeconds(1);
    usbStart(&USBD1, &usbcfg);
    usbConnectBus(&USBD1);
}

SerialUSBDriver* getSerialUSBDriver()
{
    return &SDU1;
}

State getState()
{
    return (USBD1.state == USB_ACTIVE) ? State::Connected : State::Disconnected;
}

}
}
