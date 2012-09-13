/*
 * Copyright 2012 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors:
 *      Renato Araujo Oliveira Filho <renato@canonical.com>
 */

#include "qstateaction.h"

/*!
    \qmlclass QStateAction
    \inherits QAction

    \brief A QStateAction implementation to be used with \l QDBusActionGroup

    \bold {This component is under heavy development.}

     This class can be used as a proxy for an action that is exported over D-Bus

    \code
    QDBusActionGroup {
        id: actionGroup
        busType: 1
        busName: "com.ubuntu.menu"
        objectPath: "com/ubuntu/menu/actions"
    }

    Button {
        visible: actionGroup.getAction("button.bvisible").status
    }
    \endcode
*/

/*! \internal */
QStateAction::QStateAction(const QString &text, QObject *parent)
    :QAction(text, parent)
{
}

/*!
    \qmlproperty int QStateAction::state
    This property holds the current action state
*/
QVariant QStateAction::state() const
{
    return m_state;
}

/*!
    \qmlproperty int QStateAction::isValid
    This property return if the current Action is valid or not
    A valid Action is a action which has a DBus action linked
*/
bool QStateAction::isValid() const
{
    return m_valid;
}

/*! \internal */
void QStateAction::setValid(bool valid)
{
    if (m_valid != valid) {
        m_valid = valid;
        Q_EMIT validChanged(m_valid);
    }
}

/*! \internal */
void QStateAction::setState(const QVariant &state)
{
    if (m_state != state) {
        m_state = state;
        Q_EMIT stateChanged(m_state);
    }
}