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

#include "tlv.h"

Tlv::Tlv(quint8 type, const QByteArray& value)
    :   m_type(type),
        m_value(value)
{
}

Tlv::~Tlv()
{
}

quint8 Tlv::type() const
{
    return m_type;
}

quint64 Tlv::length() const
{
    switch (m_type)
    {
        case Tlv::Null:
        case Tlv::Terminator:
            return 0;

        default:
            return m_value.count();
    }
}

QByteArray Tlv::value() const
{
    switch (m_type)
    {
        case Tlv::Null:
        case Tlv::Terminator:
            return QByteArray();

        default:
            return m_value;
    }
}

QByteArray Tlv::toByteArray() const
{
    QByteArray buffer;

    switch (m_type)
    {
        case Tlv::Null:
        case Tlv::Terminator:
            buffer.append(m_type);
            break;

        default:
        {
            buffer.append(m_type);
            quint16 length = this->length();

            if (length <= 0xFE)
            {
                buffer.append((quint8)length);
            }
            else
            {
                buffer.append((quint8)0xFF);
                buffer.append((quint8)(length >> 8));
                buffer.append((quint8)(length & 0xFF));
            }

            if (this->length() > 0)
                buffer.append(m_value);
        }
        break;
    }

    return buffer;
}

TlvList Tlv::fromByteArray(const QByteArray& data, quint64 offset)
{
    TlvList list;

    QByteArray buffer = data.left(data.count() - offset);
    qint32 count = buffer.count();
    qint32 index = 0;

    while (count > index)
    {
        quint8 type = buffer.at(index);
        ++index;

        switch (type)
        {
            case Tlv::Null:
                break;

            case Tlv::Terminator:
            {
                list.append(Tlv::createTerminatorTlv());
                index = count;
            }
            break;

            default:
            {
                if ((count - index) > 0)
                {
                    quint16 length = buffer.at(index);
                    ++index;

                    if (length > 0xFE)
                    {
                        length = 0;

                        if ((count - index) > 2)
                        {
                            length = (quint8(buffer.at(index)) << 8) | quint8(buffer.at(index + 1));
                            index += 2;
                        }
                    }

                    if ((count - index) >= length)
                    {
                        list.append(Tlv(type, buffer.mid(index, length)));
                        index += length;
                    }
                    else
                    {
                        index = count;
                    }
                }
            }
            break;
        }
    }

    return list;
}

Tlv Tlv::createNullTlv()
{
    return Tlv(Tlv::Null);
}

Tlv Tlv::createTerminatorTlv()
{
    return Tlv(Tlv::Terminator);
}

Tlv Tlv::createNDEFMessageTlv(const NDEFMessage& msg)
{
    return Tlv(Tlv::NDEF, msg.toByteArray());
}
