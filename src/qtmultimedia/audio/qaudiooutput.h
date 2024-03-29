/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtMultimedia module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QAUDIOOUTPUT_H
#define QAUDIOOUTPUT_H

#include <QtCore/qiodevice.h>
#include <QtCore/qglobal.h>

#include <qtmultimedia/audio/qaudio.h>
#include <qtmultimedia/audio/qaudioformat.h>
#include <qtmultimedia/audio/qaudiodeviceinfo.h>


QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

//QT_MODULE(Multimedia)


class QAbstractAudioOutput;

class QAudioOutput : public QObject
{
    Q_OBJECT

public:
    explicit QAudioOutput(const QAudioFormat &format = QAudioFormat(), QObject *parent = 0);
    explicit QAudioOutput(const QAudioDeviceInfo &audioDeviceInfo, const QAudioFormat &format = QAudioFormat(), QObject *parent = 0);
    ~QAudioOutput();

    QAudioFormat format() const;

    QIODevice* start(QIODevice *device = 0);
    void stop();
    void reset();
    void suspend();
    void resume();

    void setBufferSize(int bytes);
    int bufferSize() const;

    int bytesFree() const;
    int periodSize() const;

    void setNotifyInterval(int milliSeconds);
    int notifyInterval() const;

    qint64 totalTime() const;
    qint64 clock() const;

    QAudio::Error error() const;
    QAudio::State state() const;

Q_SIGNALS:
    void stateChanged(QAudio::State);
    void notify();

private:
    Q_DISABLE_COPY(QAudioOutput)

    QAbstractAudioOutput* d;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif // QAUDIOOUTPUT_H
