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

#ifndef NDEFMESSAGE_H
#define NDEFMESSAGE_H

#include "ndefrecord.h"

class LIBNDEFSHARED_EXPORT NDEFMessage
{
protected:
    NDEFRecordList m_records;
    
public:
    NDEFMessage();
    NDEFMessage(const QByteArray& data, const NDEFRecordType& type = NDEFRecordType(), int offset = 0);
    NDEFMessage(const NDEFRecord& record);
    NDEFMessage(const NDEFRecordList& records);
    virtual ~NDEFMessage();
    
    void appendRecord(const NDEFRecord& record);
    void insertRecord(const NDEFRecord& record, int index = -1);
    void removeRecord(int index = 0);
    void setRecord(const NDEFRecord& record, int index = 0);
    NDEFRecord record(const QByteArray& id) const;
    NDEFRecord record(int index = 0) const;
    NDEFRecordList record(const NDEFRecordType& type) const;
    NDEFRecordList records() const;
    int recordCount() const;
    bool isValid() const;
    QByteArray toByteArray() const;

    static NDEFMessage fromByteArray(const QByteArray& data, int offset = 0);
};

#endif // NDEFMESSAGE_H
