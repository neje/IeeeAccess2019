/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2019 University of Belgrade, Faculty of Traffic and Transport Engineering
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
 * Authors: Nenad Jevtic <n.jevtic@sf.bg.ac.rs>, <nen.jevtc@gmail.com>
 *          Marija Malnar <m.malnar@sf.bg.ac.rs>
 */
 
#include "aodv-neighbor-etx.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include <cmath>
#include <stdint.h>

namespace ns3
{
  
NS_LOG_COMPONENT_DEFINE ("AodvNeighborEtx");

namespace aodv
{

NeighborEtx::NeighborEtx () : m_lppTimeStamp (0) {}

// ETX time stamp has 12 different values: 0, 1, 2, ..., 11, 0, 1, ...
// The values of time stamp changes cyclicly but 2 values are not included
// in etx (lpp count): current and next (since this is the oldest value and must be
// cleared to prepare for the next cycle)
uint8_t
NeighborEtx::CalculateNextLppTimeStamp (uint8_t currTimeStamp)
{
  uint8_t nextTimeStamp = currTimeStamp + 1;
  if (nextTimeStamp > 11)
    {
      nextTimeStamp = 0;
    }
  return nextTimeStamp;
}

void
NeighborEtx::GotoNextLppTimeStamp ()
{
  m_lppTimeStamp = CalculateNextLppTimeStamp (m_lppTimeStamp);
}

// ETX time stamp has 12 different values: 0, 1, 2, ..., 11, 0, 1, ...
// but 2 values are not included in etx (lpp count):
// 1. current time stamp, because of jitter introduced in sending lpp packets some nodes 
//    where transmitted lpp before this node and some nodes will transmit lpp after, 
//    so lpp packet count would not be fair (nodes that have transmitted lpp before
//    would be having higher lpp count by 1)
// 2. next time stamp, which is the oldest time stamp and will be cleared to prepare for
//    the next cycle, so it should not be included in calculation of current lpp count.

uint8_t 
NeighborEtx::Lpp10bMapToCnt (uint16_t lpp10bMap) const
{
  uint8_t lpp = 0;
  for (int j=0; j<12; ++j)
    {
      if ((j!=m_lppTimeStamp) && 
          (j!=CalculateNextLppTimeStamp(m_lppTimeStamp))) // bug fixed thanks to Oscar Bautista
        {
          lpp += (lpp10bMap & ((uint16_t)0x0001 << j)) ? 1 : 0;
        }
    }
  return lpp;
}

// This function prepare for the next time stamp.
// It is called by RoutingProtocol::SendLpp () every second ie. when the LPP packet is sent.
// The oldest (next, 12th) field is necessary for synchronization. Since delivery of LPPs
// is not guaranteed we have to clear oldest field when we send next LPP packet and this field
// is not used in calculation of ETX.
void
NeighborEtx::GotoNextTimeStampAndClearOldest ()
{
  GotoNextLppTimeStamp (); // go to  next time stamp which becomes current time stamp
  // Clear oldest time stamp lpp count values, this is next time stamp compared to current
  //NS_LOG_UNCOND ("ETX: " << Simulator::Now ().GetSeconds () << ", " << (uint16_t)m_lppTimeStamp << ", " << (uint16_t)(~((uint16_t)0x0001 << CalculateNextLppTimeStamp (m_lppTimeStamp))));
  for (std::map<Ipv4Address, Etx>::iterator i = m_neighborEtx.begin (); i != m_neighborEtx.end (); )
    {
      Etx etx = i->second;
      uint16_t lppMyCnt10bMap = etx.m_lppMyCnt10bMap;
      // Delete oldest time stamp lpp count (this is next time stamp compared to current)
      // Only lower 12 bits are used
      lppMyCnt10bMap &= (uint16_t)(~((uint16_t)0x0001 << CalculateNextLppTimeStamp (m_lppTimeStamp)) & (uint16_t)0x0FFF);
      etx.m_lppMyCnt10bMap = lppMyCnt10bMap;
      i->second = etx;
      // Clear former neighbors from the list.
      // These nodes are recognized based on m_lppMyCnt10bMap value.
      // If this value is 0, then this node did not receive any LPP from former neighbor node (entry for iterator i),
      // so it is obvious that neighbor node i is no longer a neighbor and should be deleted from
      // the list to release memory.
      if ((i->second.m_lppMyCnt10bMap & (uint16_t)0x0FFF) == 0) // not neighbors any more (no LPPs received lately)
        {
          i = m_neighborEtx.erase (i);
        }
      else // still neighbors, so just continue...
        {
          i++;
        }
    }
}

void
NeighborEtx::FillLppCntData (LppHeader &lppHeader)
{
  for (std::map<Ipv4Address, Etx>::const_iterator i = m_neighborEtx.begin (); i != m_neighborEtx.end (); ++i)
    {
      uint8_t lpp = Lpp10bMapToCnt (i->second.m_lppMyCnt10bMap);
      if (lpp > 0)
        {
          lppHeader.AddToNeighborsList (i->first, lpp);
          //NS_LOG_UNCOND ("           IP=" << i->first << ", lpp=" << (uint16_t)lpp << ", rev=" << (uint16_t)(i->second.m_lppForward) << ", ETX-bin-shift=" << CalculateBinaryShiftedEtx (i->second));
        }
    }
}

// This function is called by RoutingProtocol::RecvLpp when LPP packet is received
bool 
NeighborEtx::UpdateNeighborEtx (Ipv4Address addr, uint8_t lppTimeStamp, uint8_t lppForward)
{
  std::map<Ipv4Address, Etx>::iterator i = m_neighborEtx.find (addr);
  if (i == m_neighborEtx.end ())
    {
      // No address, insert new entry
      Etx etx;
      etx.m_lppForward = lppForward;
      etx.m_lppMyCnt10bMap = (uint16_t)0x0001 << lppTimeStamp;
      std::pair<std::map<Ipv4Address, Etx>::iterator, bool> result = m_neighborEtx.insert (std::make_pair (addr, etx));
      return result.second;
    }
  else
    {
      // Address found, update existing entry
      i->second.m_lppForward = lppForward;
      (i->second.m_lppMyCnt10bMap) |= ((uint16_t)0x0001 << lppTimeStamp);
      return true;
    }
}

uint8_t
NeighborEtx::CalculateBinaryShiftedEtx (Etx etxStruct) const
{
  uint8_t etx = EtxMaxValue ();
  if ((Lpp10bMapToCnt (etxStruct.m_lppMyCnt10bMap)!=0) && (etxStruct.m_lppForward!=0))
    {
      double b = 60.0;
      double etxd = std::round (b * std::log10 (100.0 / (Lpp10bMapToCnt (etxStruct.m_lppMyCnt10bMap) * etxStruct.m_lppForward)));
      if (etxd < EtxMaxValue ())
        {
          etx = (uint8_t) etxd;
        }
    }
  //NS_LOG_UNCOND ("ETX binary: " << etx);
  return etx;
}

uint8_t
NeighborEtx::GetEtxForNeighbor (Ipv4Address addr) const
{
  uint8_t etx;
  std::map<Ipv4Address, Etx>::const_iterator i = m_neighborEtx.find (addr);
  if (i == m_neighborEtx.end ())
    {
      // No address, ETX -> oo (ETX max value)
      etx = EtxMaxValue ();
      return etx;
    }
  else
    {
      // Address found, calculate and return current ETX value
      return CalculateBinaryShiftedEtx (i->second);
    }
}

} // namespace aodv
} // namespace ns3
