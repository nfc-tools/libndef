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

#ifndef NDEFRECORD_H
#define NDEFRECORD_H

#include "ndefrecordtype.h"
#include <QtCore/QList>

class LIBNDEFSHARED_EXPORT NDEFRecord
{
public:
    enum NDEFRecordFlag
    {
        NDEF_IL = 0x08,  // ID_LENGTH is present.
        NDEF_SR = 0x10,  // Short record.
        NDEF_CF = 0x20,  // Chunk flag.
        NDEF_ME = 0x40,  // Message end.
        NDEF_MB = 0x80   // Message begin.
    };

    enum NDEFRecordTextCodec
    {
        NDEF_UTF8 = 0x00,
        NDEF_UTF16 = 0x80
    };

    enum NDEFRecordAction
    {
        Do,     // Exec default action.
        Save,   // Store it.
        Open    // Open for editing.
    };

    enum NDEFRecordGenericControlConfig
    {
        CheckExitCondition = 0x02,
        ExitOnFailure = 0x04
    };
    
protected:
    NDEFRecordType m_type;
    QByteArray m_id;
    QByteArray m_payload;
    bool m_chuncked;
    
public:
    NDEFRecord();
    NDEFRecord(const QByteArray& data, const NDEFRecordType& type = NDEFRecordType(), int offset = 0, bool chuncked = false);
    NDEFRecord(const NDEFRecordType& type, const QByteArray& id = QByteArray(), const QByteArray& payload = QByteArray(), bool chuncked = false);
    virtual ~NDEFRecord();
    
    void setId(const QByteArray& id);
    QByteArray id() const;
    
    quint8 flags() const;
    bool isShort() const;
    void setChuncked(bool flag);
    bool isChuncked() const;
    bool isEmpty() const;
    bool isValid() const;
    
    void setType(const NDEFRecordType& type);
    NDEFRecordType type() const;
    
    void setPayload(const QByteArray& payload);
    void appendPayload(const QByteArray& payload);
    QByteArray payload() const;
    int payloadLength() const;
    
    QByteArray toByteArray(int flags = 0) const;

    static NDEFRecord fromByteArray(const QByteArray& data, int offset = 0);

protected:
    void checkConsistency();

    // MIME records.
public:
    static NDEFRecord createMimeRecord(const QString& mime_type, const QByteArray& payload);

    // Text records.
public:
    static NDEFRecord createTextRecord(const QString& text, const QString& locale, NDEFRecordTextCodec codec = NDEFRecord::NDEF_UTF8);
    static QByteArray textLocale(const QByteArray& payload);
    static QString textText(const QByteArray& payload);

    // URI records.
public:
    static NDEFRecord createUriRecord(const QString& uri);
    static QByteArray uriProtocol(const QByteArray& payload);

    // Smart Poster records.
public:
    static NDEFRecord createSmartPosterRecord(const QString& uri);
    static NDEFRecord createSmartPosterRecord(const QString& uri, const QString& title, const QString& locale, NDEFRecordTextCodec codec = NDEFRecord::NDEF_UTF8);
    static NDEFRecord createSmartPosterRecord(const QString& uri, const QList<NDEFRecord>& records);

    static NDEFRecord createSpActionRecord(NDEFRecordAction action);
    static NDEFRecord createSpSizeRecord(quint32 size);
    static NDEFRecord createSpTypeRecord(const QString& type);

    // Generic Control records.
public:
    static NDEFRecord createGenericControlRecord(quint8 config_byte, const NDEFRecord& target, NDEFRecordAction action, const NDEFRecord& data = NDEFRecord());
    static NDEFRecord createGenericControlRecord(quint8 config_byte, const NDEFRecord& target, const NDEFRecord& action = NDEFRecord(), const NDEFRecord& data = NDEFRecord());
    static NDEFRecord getGcTargetRecord(const NDEFRecord& record);
    static NDEFRecord getGcActionRecord(const NDEFRecord& record);
    static NDEFRecord getGcDataRecord(const NDEFRecord& record);
protected:
    static NDEFRecord createGcTargetRecord(const NDEFRecord& record);
    static NDEFRecord createGcActionRecord(const NDEFRecord& record);
    static NDEFRecord createGcActionRecord(NDEFRecordAction action);
    static NDEFRecord createGcDataRecord(const NDEFRecord& data);
};

typedef QList<NDEFRecord> NDEFRecordList;

#endif // NDEFRECORD_H
