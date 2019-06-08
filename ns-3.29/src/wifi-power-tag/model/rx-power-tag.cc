/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "rx-power-tag.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

namespace ns3 {

//NS_OBJECT_ENSURE_REGISTERED (RxPowerTag);

TypeId 
RxPowerTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::RxPowerTag")
    .SetParent<Tag> ()
    .SetGroupName("Wifi")
    .AddConstructor<RxPowerTag> ()
    .AddAttribute ("Hops",
                   "Number of hops",
                   UintegerValue (0),
                   MakeUintegerAccessor (&RxPowerTag::GetHops), //za ovu funkciju je potrebno #include uinteger.h
                   MakeUintegerChecker<uint8_t> ())
    .AddAttribute ("RxPower",
                   "Received packet power",
                   DoubleValue (0),
                   MakeDoubleAccessor (&RxPowerTag::GetRxPower), //za ovu funkciju je potrebno #include double.h
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

TypeId 
RxPowerTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

uint32_t 
RxPowerTag::GetSerializedSize (void) const //ovde definisem velicinu taga u bajtima (jedno uint8_t i jedno double)
{
  return 1 + sizeof (double); //velicina za uint8_t je uvek 1 bajt, a za double proveravam pomocu operatora sizeof (double)
}

void 
RxPowerTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_hops);
  i.WriteDouble (m_rxPower);
}

void 
RxPowerTag::Deserialize (TagBuffer i)
{
  m_hops = i.ReadU8 ();
  m_rxPower = i.ReadDouble ();
}

void 
RxPowerTag::Print (std::ostream &os) const
{
  os << "hops =" << (uint32_t)m_hops << ", RxPower =" << m_rxPower; //mora da se izvrsi konverzija iz uint8_t u uint32_t da se ne bi vrsila konverzija u asci kod pri stampanju
}

void 
RxPowerTag::SetHops (uint8_t value)
{
  m_hops = value;
}

uint8_t 
RxPowerTag::GetHops (void) const
{
  return m_hops;
}

void
RxPowerTag::SetRxPower (double value)
{
  m_rxPower = value;
}

double
RxPowerTag::GetRxPower (void) const
{
  return m_rxPower;
}

} // namespace ns3
