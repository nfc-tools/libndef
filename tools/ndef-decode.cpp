/**
 * This file is part of the libndef project.
 *
 * Copyright (C) 2011, Romuald Conty
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

#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QFile>
 
#include <ndef/ndefmessage.h>

QTextStream out(stdout);
QTextStream err(stderr);
QTextStream info(stderr);

QFile output;

const QString toTypeNameFormat (int id)
{
    switch(id)
    {
        case NDEFRecordType::NDEF_Empty: 	return QString("Empty"); break;
        case NDEFRecordType::NDEF_NfcForumRTD: 	return QString("NFC Forum well-known type"); break;
        case NDEFRecordType::NDEF_MIME: 	return QString("Media-type as defined in RFC 2046"); break;
        case NDEFRecordType::NDEF_URI: 		return QString("Absolute URI as defined in RFC 3986"); break;
        case NDEFRecordType::NDEF_ExternalRTD: 	return QString("NFC Forum external type"); break;
        case NDEFRecordType::NDEF_Unknown: 	return QString("Unknown"); break;
        case NDEFRecordType::NDEF_Unchanged: 	return QString("Unchanged"); break;
        case 7: 				return QString("Reserved"); break;
    }
    return QString("Invalid");
}

void decodeNDEFMessage (const QByteArray data, int depth = 0)
{
    NDEFMessage msg = NDEFMessage::fromByteArray (data);
    QString prefix("");
    for (int d=0; d<depth; d++) prefix.append("    ");
    
    if (msg.isValid()) {
        info << prefix << "NDEF message is valid and contains " << msg.recordCount() << " NDEF record(s)." << endl;
        int i = 0;
        foreach (const NDEFRecord& record, msg.records())
        {
            i++;
            info << prefix << "NDEF record (" << i << ") type name format: " << toTypeNameFormat(record.type().id()) << endl;
            const QString type_name = record.type().name();
            info << prefix << "NDEF record (" << i << ") type: " << type_name << endl;
            
            switch (record.type().id())
            {
                case NDEFRecordType::NDEF_NfcForumRTD:
                    if (type_name == QString("Sp"))
                    {
                        decodeNDEFMessage (record.payload(), ++depth);
                    }
                    else if (type_name == QString("T"))
                    {
                        const QString locale_string = NDEFRecord::textLocale(record.payload()).replace('-', '_');
                        // const QString locale_string = NDEFRecord::textLocale(record.payload());
                        // const QString locale_string = record.payload().toHex();
                        QLocale locale(locale_string);
                        info << prefix << "NDEF record (" << i << ") payload (language): " << QLocale::languageToString (locale.language()) << " (" << locale_string << ")" << endl;
                        info << prefix << "NDEF record (" << i << ") payload (text): " << NDEFRecord::textText(record.payload()) << endl;
                    }
                    else if (type_name == QString("U"))
                    {
                        info << prefix << "NDEF record (" << i << ") payload (uri): " << NDEFRecord::uriProtocol (record.payload()) << record.payload().mid(1) << endl;
                    }
                    else if ((depth > 0) && (type_name == QString("act")))
                    {
                        quint8 action = record.payload().at(0);
                        info << prefix << "NDEF record (" << i << ") payload (action code): " << action << endl;
                    }
                    else if ((depth > 0) && (type_name == QString("s")))
                    {
                        QDataStream stream(record.payload());
                        qint32 size;
                        stream >> size;
                        info << prefix << "NDEF record (" << i << ") payload (size): " << size << endl;
                    }
                    else if ((depth > 0) && (type_name == QString("t")))
                    {
                        info << prefix << "NDEF record (" << i << ") payload (type): " << QString::fromUtf8(record.payload()) << endl;
                    }
                    else
                    {
                        info << prefix << "NDEF record (" << i << ") payload (hex): " << record.payload().toHex() << endl;
                    }
                break;
                case NDEFRecordType::NDEF_MIME:
                    if (output.isOpen())
                    {
                        output.write (record.payload());
                        output.close();
                    }
                default:
                    info << prefix << "NDEF record (" << i << ") payload (hex): " << record.payload().toHex() << endl;
            }
        }
    }
    else
    {
        err << "Invalid NDEF message." << endl;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);
   
    QStringList arguments = app.arguments();

    QFile input;

    for (int i=1; i<arguments.count(); i++)
    {
        if (arguments.at(i).at(0) == '-')
        {
            if (arguments.at(i).at(1) == 'o')
            {
                if ((i+1) >= arguments.size())
                {
                    err << "-o option requires an argument" << endl;
                    return 1;
                }
                i++;
                const QString filename = arguments.at(i);
                output.setFileName (filename);
                output.open (QIODevice::WriteOnly);
                if (!output.isOpen())
                {
                    err << "Unable to open \"" << filename << "\"." << endl;
                    return 1;
                }
            }
            else
            {
                err << "Unknown option: " << arguments.at(i).at(1) << endl;
                return 1;
            }
        }
        else
        {
             const QString filename = arguments.at(i);
             input.setFileName(filename);
             input.open(QIODevice::ReadOnly);
             if (!input.isOpen())
             {
                 err << "Unable to read input file \"" << filename << "\"." << endl;
                 return 1;
             }
        }
    }

    if (!input.isOpen())
    {
        qDebug() << "Use stdin as input file";
        input.open ( stdin, QIODevice::ReadOnly );
    }

    if (input.isOpen ()) {
        QByteArray data = input.readAll();
        if (data.isEmpty())
        {
            err << "No data to decode." << endl;
            return 1;
        }
        decodeNDEFMessage (data);
    }
    return 0;
}

