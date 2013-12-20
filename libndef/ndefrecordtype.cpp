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

#include "ndefrecordtype.h"
#include <QtCore/QBuffer>
#include <QtCore/QDataStream>

NDEFRecordType::NDEFRecordType(NDEFRecordTypeId id, const QByteArray& name)
        :   m_id(id),
            m_name(name)
{
    switch (id)
    {
        case NDEF_Empty:
            m_name.clear();
            break;

        default:
            break;
    }
}

NDEFRecordType::NDEFRecordTypeId NDEFRecordType::id() const
{
    return m_id;
}

QByteArray NDEFRecordType::name() const
{
    return m_name;
}

bool NDEFRecordType::operator==(const NDEFRecordType& type) const
{
    return ((type.id() == m_id) && (type.name() == m_name));
}

bool NDEFRecordType::operator!=(const NDEFRecordType& type) const
{
    return ((type.id() != m_id) || (type.name() != m_name));
}

NDEFRecordType NDEFRecordType::fromByteArray(const QByteArray& data, int offset)
{
    unsigned int total_size = data.count() - offset;

    if (total_size > 2)
    {
        QByteArray data_buffer = data.right(total_size);
        QBuffer buffer(&data_buffer);
        buffer.open(QIODevice::ReadOnly);
        QDataStream stream(&buffer);
        quint8 byte;

        // 1) TNF & flags.
        stream >> byte;
        quint8 tnf = byte & 0x07;
        bool has_id = byte & 0x08;
        bool short_record = byte & 0x10;

        // 2) Type length.
        stream >> byte;
        quint8 type_length = byte;

        // 3) Payload length.
        stream.skipRawData(short_record ? 1 : 4);

        // 4) ID Length.
        stream.skipRawData(has_id ? 1 : 0);

        // 5) Type.
        QByteArray type_name;
        for (quint8 i = 0; i < type_length; i++)
        {
            stream >> byte;
            type_name.append(byte);
        }

        return NDEFRecordType((NDEFRecordTypeId)tnf, type_name);
    }

    // Invalid record.
    return NDEFRecordType(NDEFRecordType::NDEF_Invalid);
}

NDEFRecordType NDEFRecordType::textRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "T");
}

NDEFRecordType NDEFRecordType::uriRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "U");
}

NDEFRecordType NDEFRecordType::smartPosterRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "Sp");
}

NDEFRecordType NDEFRecordType::genericControlRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "Gc");
}

NDEFRecordType NDEFRecordType::spActionRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "act");
}

NDEFRecordType NDEFRecordType::spSizeRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "s");
}

NDEFRecordType NDEFRecordType::spTypeRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "t");
}

NDEFRecordType NDEFRecordType::gcTargetRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "t");
}

NDEFRecordType NDEFRecordType::gcActionRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "a");
}

NDEFRecordType NDEFRecordType::gcDataRecordType()
{
    return NDEFRecordType(NDEFRecordType::NDEF_NfcForumRTD, "d");
}

