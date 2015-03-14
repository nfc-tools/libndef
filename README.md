# Introduction

This is a C++/Qt library to encode and decode messages based on **NDEF (NFC Data Exchange Format) Specification**.

For more info please visit: http://www.nfc-forum.org/specs/spec_list/

It depends on **Qt Core** module (from **Qt Framework**, visit: http://qt.nokia.com).

Source code available at GitHub: https://github.com/nfc-tools/libndef

# Installation

  * Checkout the source tree:

> ` git clone https://github.com/nfc-tools/libndef.git `

  * Follow the instructions in the https://github.com/nfc-tools/libndef/blob/master/INSTALL file.

# Examples

## Create & serialize a NDEF message

```
#include <ndefmessage.h>

// First we create a valid NDEF message object...
NDEFMessage msg;
msg.appendRecord(NDEFRecord::createUriRecord("http://code.google.com/p/libndef"));
msg.appendRecord(NDEFRecord::createTextRecord("Hello, world!", "en-US"));

// ...and then we can serialize it and send everywhere.
QByteArray output = msg.toByteArray();
```

## Parse a NDEF message stream

```
#include <ndefmessage.h>

// Imagine we've read a byte stream from a NFC tag.
QByteArray input = read_from_nfc_tag();

// Now we can parse it...
NDEFMessage msg = NDEFMessage::fromByteArray(input);

// ...and then we can use it.
if (msg.isValid())
    printf("Num of records: %d", msg.recordCount());
```

## Encapsulate a NDEF message into a TLV record

```
#include <tlv.h>

// Imagine we've a valid NDEF message.
NDEFMessage msg;

// Now we can easily encapsulate it inside a TLV record...
Tlv tlv = Tlv::createNDEFMessageTlv(msg);

// ...and then we can serialize it and send everywhere.
QByteArray output = tlv.toByteArray();
```

## Extract a NDEF Message from a TLV record
```
#include <tlv.h>

// Imagine we've a TLV byte stream.
QByteArray input;

// We can obtain the list of TLV records...
TlvList list = Tlv::fromByteArray(input);

// ...and then we can search for a NDEF one.
foreach (Tlv tlv, list)
{
    if (tlv.type() == Tlv::NDEF)
    {
        NDEFMessage msg = NDEFMessage::fromByteArray(tlv.value());
        if (msg.isValid())
            printf("Num of records: %d", msg.recordCount());
    }
}
```
