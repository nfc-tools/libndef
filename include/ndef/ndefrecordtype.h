/**
 * This file is part of the libndef project.
 *
 * Copyright (C) 2009, Emanuele Bertoldi (Card Tech srl).
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef NDEFRECORDTYPE_H
#define NDEFRECORDTYPE_H

#include "libndef_global.h"
#include <QtCore/QByteArray>

class LIBNDEFSHARED_EXPORT NDEFRecordType
{
public:
    enum NDEFRecordTypeId
    {
        NDEF_Empty,
        NDEF_NfcForumRTD,   // NFC Forum well-known type.
        NDEF_MIME,          // Media-type as defined in RFC 2046.
        NDEF_URI,           // Absolute URI as defined in RFC 3986.
        NDEF_ExternalRTD,   // NFC Forum esternal type.
        NDEF_Unknown,
        NDEF_Unchanged,     // Used for payload chunks.
        NDEF_Invalid        // NFC Forum reserved. It's used here for invalid types.
    };

protected:
    NDEFRecordTypeId m_id;
    QByteArray m_name;

public:
    NDEFRecordType(NDEFRecordTypeId id = NDEF_Empty, const QByteArray& name = "");

    NDEFRecordTypeId id() const;
    QByteArray name() const;

    bool operator==(const NDEFRecordType& type) const;
    bool operator!=(const NDEFRecordType& type) const;

    static NDEFRecordType fromByteArray(const QByteArray& data, int offset = 0);

    static NDEFRecordType textRecordType();
    static NDEFRecordType uriRecordType();
    static NDEFRecordType smartPosterRecordType();
    static NDEFRecordType genericControlRecordType();

    static NDEFRecordType spActionRecordType();
    static NDEFRecordType spSizeRecordType();
    static NDEFRecordType spTypeRecordType();

    static NDEFRecordType gcTargetRecordType();
    static NDEFRecordType gcActionRecordType();
    static NDEFRecordType gcDataRecordType();
};

#endif // NDEFRECORDTYPE_H
