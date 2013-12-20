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

typedef QList<NDEFRecord> NDEFRecordList;

void print_usage(const QString& appName)
{
        err << "Usage: " << appName << " [OUTPUT] OPTIONS" << endl;
        err << "Encode a NDEF Message from OPTIONS." << endl;
        err << "If OUTPUT is not specified, the result is outputed on stdout" << endl << endl;
        err << "Options:" << endl;
        err << "  -t TEXT LOCALE		create new TextRecord" << endl;
        err << "  -u URI			create new UriRecord" << endl;
        err << "  -m MIME-TYPE FILE		create new MimeRecord" << endl;
        err << "  -sp URI			create and open a new SmartPosterRecord" << endl;
        err << "  -s-				close current SmartPoster" << endl;
        err << "  -sa ACTION			create new SpActionRecord" << endl;
        err << "  -ss SIZE			create new SpSizeRecord" << endl;
        err << "  -st TYPE			create new SpTypeRecord" << endl << endl;
        err << "Examples:" << endl;
        err << "  Create a NDEF Message than contains an URL:" << endl;
        err << "    " << appName << " libndef_website.ndef -sp \"http://libndef.googlecode.com\" -t \"libndef\" \"en-US\" -s-" << endl;
        err << "  Create a NDEF Message than contains an electronic card (vCard):" << endl;
        err << "    " << appName << " myvcard.ndef -m \"text/x-vCard\" ./my_vcard.vcf" << endl;
}

typedef enum {
  NDEF_MESSAGE,
  NDEF_SMARTPOSTER,
  NDEF_GENERIC_RECORD,
} ndef_container_type;

int main(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);
   
    QStringList arguments = app.arguments();

    if (arguments.count() == 1)
    {
        print_usage(arguments.at(0));
        return 1;
    }

    QFile input;

    QList<NDEFRecordList> ndef_containers;
    QList<ndef_container_type> ndef_containers_type;

    ndef_containers.append(NDEFRecordList());
    ndef_containers_type.append(NDEF_MESSAGE);

    QString current_sp_uri;

    for (int i=1; i<arguments.count(); i++)
    {
        if (arguments.at(i).at(0) == '-')
        {
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
            char option = arguments.at(i).at(1).toAscii();
#else
            char option = arguments.at(i).at(1).toLatin1();
#endif
            switch (option)
            {
            case 't': // TextRecord
            {
                if ((i+2) >= arguments.size())
                {
                    err << "-t option requires two arguments: text and locale (e.g. \"Hello world\" \"en-US\")" << endl;
                    return 1;
                }
                i++;
                const QString text = arguments.at(i);
                i++;
                const QString locale = arguments.at(i);
                ndef_containers.last().append(NDEFRecord::createTextRecord(text, locale, NDEFRecord::NDEF_UTF8));
            }
                break;
            case 'u': // UriRecord
            {
                if ((i+1) >= arguments.size())
                {
                    err << "-u option requires an URI as argument (e.g. \"http://libnfc.org\")" << endl;
                    return 1;
                }
                i++;
                const QString uri = arguments.at(i);
                ndef_containers.last().append(NDEFRecord::createUriRecord(uri));
            }
                break;
            case 'm': // MimeRecord
            {
                if ((i+2) >= arguments.size())
                {
                    err << "-m option requires two arguments: MIME-TYPE and FILE (e.g. \"text/x-vCard\" ./my_vcard.vcf)" << endl;
                    return 1;
                }
                i++;
                const QString mimetype = arguments.at(i);
                i++;
                const QString mimefilename = arguments.at(i);
                QFile mimefile;
                mimefile.setFileName(mimefilename);
                mimefile.open (QIODevice::ReadOnly );
                if (!mimefile.isOpen())
                {
                    err << "Unable to load MIME file: " << mimefilename << endl;
                    return 1;
                }
                ndef_containers.last().append(NDEFRecord::createMimeRecord(mimetype, mimefile.readAll()));
            }
                break;
            case 's': // SmartPosterRecord
            {
 #if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
                char sp_option = arguments.at(i).at(2).toAscii();
#else
                char sp_option = arguments.at(i).at(2).toLatin1();
#endif
                switch (sp_option)
                {
                case 'p': // -sp: open a new SmartPoster
                {
                    if ((i+1) >= arguments.size())
                    {
                        err << "-sp option requires an URI as argument (e.g. \"http://libnfc.org\")" << endl;
                        return 1;
                    }
                    i++;
                    current_sp_uri = arguments.at(i);
                    ndef_containers.append(NDEFRecordList());
                    ndef_containers_type.append(NDEF_SMARTPOSTER);
                    break;
                }
                case 'a': // -sa: SpActionRecord
                {
                    if ((i+1) >= arguments.size())
                    {
                        err << "-sa option requires an action code (integer) (ie. 0: Do action, 1: Save for later, 2: Open for editing)" << endl;
                        return 1;
                    }
                    if (ndef_containers.count() <= 1)
                    {
                        err << "-sa option can only be used inside a SmartPoster" << endl;
                        return 1;
                    }
                    i++;
                    bool ok;
                    int action_code = arguments.at(i).toInt(&ok);
                    if(!ok)
                    {
                        err << "-sa option requires an action code (integer) (ie. 0: Do action, 1: Save for later, 2: Open for editing)" << endl;
                        return 1;
                    }
                    ndef_containers.last().append(NDEFRecord::createSpActionRecord((NDEFRecord::NDEFRecordAction)action_code));
                    break;
                }
                case 's': // -ss: SpSizeRecord
                {
                    if ((i+1) >= arguments.size())
                    {
                        err << "-ss option requires a size (32bits) as argument (e.g. 1234567890)" << endl;
                        return 1;
                    }
                    if (ndef_containers.count() <= 1)
                    {
                        err << "-ss option can only be used inside a SmartPoster" << endl;
                        return 1;
                    }
                    i++;
                    bool ok;
                    long size = arguments.at(i).toLong(&ok);
                    if(!ok)
                    {
                        err << "-ss option requires a size (32bits) as argument (e.g. 1234567890)" << endl;
                        return 1;
                    }
                    ndef_containers.last().append(NDEFRecord::createSpSizeRecord(size));
                    break;
                }
                case 't': // -st: SpTypeRecord
                {
                    if ((i+1) >= arguments.size())
                    {
                        err << "-st option requires an type as argument (e.g. \"text/html\")" << endl;
                        return 1;
                    }
                    if (ndef_containers.count() <= 1)
                    {
                        err << "-st option can only be used inside a SmartPoster" << endl;
                        return 1;
                    }
                    i++;
                    const QString type = arguments.at(i);
                    ndef_containers.last().append(NDEFRecord::createSpTypeRecord(type));
                    break;
                }
                case '-': // Close SmartPoster
                {
                    if ((ndef_containers.size() < 2) || (ndef_containers_type.last() != NDEF_SMARTPOSTER))
                    {
                        err << "-s- option present but no SmartPoster to close" << endl;
                        return 1;
                    }
                    NDEFRecord sp = NDEFRecord::createSmartPosterRecord(current_sp_uri, ndef_containers.last());
                    ndef_containers.removeLast();
                    ndef_containers_type.removeLast();
                    ndef_containers.last().append(sp);
                    break;
                }
                default:
                    err << "-s option need a suffix (e.g. -sp (new SmartPosterRecord), -sa (new SpActionRecord), -st (new SpTypeRecord), -ss (new SpSizeRecord) or -s- (close current SmartPoster)" << endl;
                    return 1;
                }
            }
                break;
            case 'h':
            {
                print_usage (arguments.at(0));
                return 0;
            }
            default:
                err << "Unknown option: " << option << endl;
                return 1;
            }
        }
        else
        {
             output.setFileName(arguments.at(i));
             output.open(QIODevice::WriteOnly);
             if (!output.isOpen())
             {
                 err << "Unable to open output file." << endl;
                 return 1;
             }
        }
    }

    if (ndef_containers.count() == 0)
    {   // Should not happend...
        err << "Parsing error." << endl;
        return 1;
    }
    if (ndef_containers.count() > 1)
    {
        switch (ndef_containers_type.last())
        {
        case NDEF_SMARTPOSTER:
            err << "A SmartPoster is still opened." << endl;
            break;
        case NDEF_GENERIC_RECORD:
            err << "A GenericControlRecord is still opened." << endl;
            break;
        case  NDEF_MESSAGE:
            err << "You should not see this..." << endl;
            break;
        }
        return 1;
    } // Only root message remains
    if (ndef_containers.at(0).count() == 0)
    {
        err << "There is no NDEF Record to encode." << endl;
        print_usage(arguments.at(0));
        return 1;
    }
    if (!output.isOpen())
    {
        output.open ( stdout, QIODevice::WriteOnly );
    }
    if (output.isOpen ()) {
        // NDEFMessage contains data
        NDEFMessage msg(ndef_containers.last());
        output.write(msg.toByteArray());
    }
    return 0;
}

