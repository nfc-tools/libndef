##
# This file is part of the libndef project.
#
# Copyright (C) 2009, Emanuele Bertoldi (Card Tech srl).
#
# This program is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your
# option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>
##

VERSION=1.2.0
NDEF_INCDIR = ../include/ndef
NDEF_SRCDIR = ../libndef

PUBLIC_HEADERS = $$NDEF_INCDIR/libndef_global.h \
    $$NDEF_INCDIR/ndefrecord.h \
    $$NDEF_INCDIR/ndefmessage.h \
    $$NDEF_INCDIR/ndefrecordtype.h \
    $$NDEF_INCDIR/tlv.h

QT -= gui
TARGET = ndef
TEMPLATE = lib
DEFINES += NDEF_LIBRARY
INCLUDEPATH += $$NDEF_INCDIR
HEADERS += $$PUBLIC_HEADERS
SOURCES += $$NDEF_SRCDIR/ndefrecord.cpp \
    $$NDEF_SRCDIR/ndefmessage.cpp \
    $$NDEF_SRCDIR/ndefrecordtype.cpp \
    $$NDEF_SRCDIR/tlv.cpp

unix: {
    # install library and headers
    isEmpty(PREFIX) {
      PREFIX = /usr/local
    }
    target.path = $$PREFIX/lib
    INSTALLS += target

    incfiles.path = $$PREFIX/include/ndef
    incfiles.files = $$PUBLIC_HEADERS
    INSTALLS += incfiles

    # install pkg-config file (libndef.pc)
    CONFIG += create_pc create_prl
    QMAKE_PKGCONFIG_REQUIRES = QtCore
    QMAKE_PKGCONFIG_LIBDIR = $$target.path
    QMAKE_PKGCONFIG_INCDIR = $$incfiles.path
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
