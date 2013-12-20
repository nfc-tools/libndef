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

#ifndef TLV_H
#define TLV_H

#include "ndefmessage.h"

class Tlv;

typedef QList<Tlv> TlvList;

class LIBNDEFSHARED_EXPORT Tlv
{
public:
    enum TlvType
    {
        Null        = 0x00,
        NDEF        = 0x03,
        Terminator  = 0xFE
    };

protected:
    quint8 m_type;
    QByteArray m_value;

public:
    Tlv(quint8 type, const QByteArray& value = QByteArray());
    virtual ~Tlv();

    quint8 type() const;
    quint64 length() const;
    QByteArray value() const;

    QByteArray toByteArray() const;

    static TlvList fromByteArray(const QByteArray& data, quint64 offset = 0);

    static Tlv createNullTlv();
    static Tlv createTerminatorTlv();
    static Tlv createNDEFMessageTlv(const NDEFMessage& msg);
};

#endif // TLV_H
