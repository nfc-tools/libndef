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

#include "ndefmessage.h"

NDEFMessage::NDEFMessage()
{
}

NDEFMessage::NDEFMessage(const QByteArray& data, const NDEFRecordType& type, int offset)
{
    m_records.append(NDEFRecord(data, type, offset));
}

NDEFMessage::NDEFMessage(const NDEFRecord& record)
{
    m_records.append(record);
}

NDEFMessage::NDEFMessage(const NDEFRecordList& records)
{
    m_records = records;
}

NDEFMessage::~NDEFMessage()
{
}

void NDEFMessage::appendRecord(const NDEFRecord& record)
{
    m_records.append(record);
}

void NDEFMessage::insertRecord(const NDEFRecord& record, int index)
{
    if (index == -1)
        index = m_records.count();
    m_records.insert(index, record);
}

void NDEFMessage::removeRecord(int index)
{
    Q_ASSERT(index < m_records.count());
    m_records.removeAt(index);
}

void NDEFMessage::setRecord(const NDEFRecord& record, int index)
{
    Q_ASSERT(index < m_records.count());
    m_records[index] = record;
}

NDEFRecord NDEFMessage::record(const QByteArray& id) const
{
    foreach (const NDEFRecord& record, m_records)
        if (record.id() == id)
            return record;
    
    return NDEFRecord();
}

NDEFRecord NDEFMessage::record(int index) const
{
    Q_ASSERT(index < m_records.count());
    return m_records[index];
}

NDEFRecordList NDEFMessage::record(const NDEFRecordType& type) const
{
    NDEFRecordList o;
    foreach (const NDEFRecord& record, m_records)
        if (record.type() == type)
            o.append(record);
    
    return o;
}

NDEFRecordList NDEFMessage::records() const
{
    return m_records;
}

int NDEFMessage::recordCount() const
{
    return m_records.count();
}

bool NDEFMessage::isValid() const
{
    if (m_records.isEmpty())
        return false;

    foreach (NDEFRecord record, this->records())
        if (!record.isValid())
            return false;

    return true;
}

QByteArray NDEFMessage::toByteArray() const
{
    QByteArray output;
    
    int record_count = this->recordCount();
    int flags = 0;
    for (int i = 0; i < record_count; i++)
    {
        flags = 0;
        
        if (i == 0)
            flags |= NDEFRecord::NDEF_MB;
        if (i == record_count-1)
            flags |= NDEFRecord::NDEF_ME;
        
        output.append(m_records[i].toByteArray(flags));
    }
    
    return output;
}

NDEFMessage NDEFMessage::fromByteArray(const QByteArray& data, int offset)
{
    NDEFMessage msg;

    QByteArray buffer = data.right(data.count() - offset);
    while (!buffer.isEmpty())
    {
        NDEFRecord record = NDEFRecord::fromByteArray(buffer);
        if (record.type().id() == NDEFRecordType::NDEF_Invalid)
            break;

        msg.appendRecord(record);
        QByteArray record_buffer = record.toByteArray();
        buffer.remove(0, record_buffer.count());
    }

    return msg;
}
