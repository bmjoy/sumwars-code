/*
	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "servernetwork.h"

NetworkSlot::~NetworkSlot()
{
	while (!m_received_packets.empty())
	{
		m_server_network->deallocatePacket(m_received_packets.front());
		m_received_packets.pop();
	}
}

NetworkPacket* NetworkSlot::popReceivedPacket()
{
	NetworkPacket* data;
	
	if (!m_received_packets.empty())
	{
		data = m_received_packets.front();
		m_received_packets.pop();
	}
	else
	{
		data = 0;
	}
	return data;
}

///////////////////////////////////////////
// ServerNetwork functions

ServerNetwork::ServerNetwork(int max_slots)
	: Network()
{
    DEBUG("Max slots %d", max_slots);
	m_slots.resize(max_slots);
	m_max_slots = max_slots;
	
	for (int i=0;i< m_max_slots;i++)
	{
		m_slots[i] =0;
	}
}


ServerNetwork::~ServerNetwork()
{
	
}

void ServerNetwork::kill()
{
	for (int i=0;i< m_max_slots;i++)
	{
		if (m_slots[i]!=0)
		{
			delete m_slots[i];
			m_slots[i] = 0;
		}
	}
}

int ServerNetwork::popNewLoginSlot()
{
	if (m_new_login_slots.empty())
	{
		return NOSLOT;
	}
	else
	{
		int tmp = m_new_login_slots.front();
		m_new_login_slots.pop();
		return tmp;
	}
}


