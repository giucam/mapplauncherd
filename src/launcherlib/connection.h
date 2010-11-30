/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of applauncherd
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "appdata.h"
#include "protocol.h"

#include <stdint.h>
#include <string>

using std::string;

#include <map>

using std::map;

#include <vector>

using std::vector;

typedef map<string, int> PoolType;

#ifdef HAVE_CREDS
    #include <sys/creds.h>
#endif

#define IO_DESCRIPTOR_COUNT 3

/*!
 * \class Connection.
 * \brief Wrapper class for the connection between invoker and launcher.
 *
 * This class wraps up the UNIX file socket connection between the invoker
 * and the launcher daemon. The low-level communication code is mostly taken
 * from the maemo-launcher used in Maemo 5. It might need a re-write.
 */
class Connection
{
public:

    /*! \brief Constructor.
     *  \param socketId Path to the UNIX file socket to be used.
     *  \param testMode Bypass all real socket activity to help unit testing.
     */
    explicit Connection(const string socketId, bool testMode = false);

    /*! \brief Accept connection.
     * Accept a socket connection from the invoker.
     * Stores security credentials of the connected
     * peer to appData, if security is enabled. The credentials
     * in appData must be released by the caller.
     * \return true on success.
     */
    bool accept(AppData* appData);

    //! \brief Close the socket connection.
    void close();

    //! \brief Receive application data to appData.
    bool receiveApplicationData(AppData* appData);

    //! \brief Return true if invoker wait for process exit status
    bool isReportAppExitStatusNeeded() const;

    //! \brief Send application exit status to invoker
    bool sendAppExitStatus(int status);

    /*! \brief Initialize a file socket.
     * \param socketId Path to the socket file
     */
    static void initSocket(const string socketId);

    //! \brief Close all open sockets.
    static void closeAllSockets();

    //! \brief Get pid of the process on the other end of socket connection
    pid_t peerPid();

private:

    /*! \brief Receive actions.
     * This method executes the actual data-receiving loop and terminates
     * after INVOKER_MSG_END is received.
     * \return True on success
     */
    bool receiveActions();

    /*! \brief Receive and return the magic number.
     * \return The magic number received from the invoker.
     */
    int receiveMagic();

    /*! \brief Receive and return the application name.
     * \return Name string
     */
    string receiveAppName();

    /*! \brief Return initialized socket.
     * \param socketId Path to the socket file
     */
    static int findSocket(const string socketId);

    //! Disable copy-constructor
    Connection(const Connection & r);

    //! Disable assignment operator
    Connection & operator= (const Connection & r);

    //! Receive executable name
    bool receiveExec();

    //! Receive arguments
    bool receiveArgs();

    //! Receive environment
    bool receiveEnv();

    //! Receive I/O descriptors
    bool receiveIO();

    //! Receive userId and GroupId
    bool receiveIDs();

    //! Receive priority
    bool receivePriority();

    //! Receive booster respawn delay
    bool receiveDelay();

    //! Send process pid
    bool sendPid(pid_t pid);

    //! Send message to a socket. This is a virtual to help unit testing.
    virtual bool sendMsg(uint32_t msg);

    //! Receive a message from a socket. This is a virtual to help unit testing.
    virtual bool recvMsg(uint32_t *msg);

    //! Send a string. This is a virtual to help unit testing.
    virtual bool sendStr(const char * str);

    //! Receive a string. This is a virtual to help unit testing.
    virtual const char * recvStr();

    //! Pool of sockets mapped to id's
    static PoolType socketPool;

    //! Run in test mode, if true
    bool m_testMode;

    //! Socket fd
    int      m_fd;
    int      m_curSocket;
    string   m_fileName;
    uint32_t m_argc;
    const char **  m_argv;
    int      m_io[IO_DESCRIPTOR_COUNT];
    uint32_t m_priority;
    uint32_t m_delay;
    bool     m_sendPid;
    gid_t    m_gid;
    uid_t    m_uid;

#if defined (HAVE_CREDS)
    static const char * m_credsStr;
    creds_value_t m_credsValue;
    creds_type_t  m_credsType;
#endif

#ifdef UNIT_TEST
    friend class Ut_Connection;
#endif
};

#endif //CONNECTION_H