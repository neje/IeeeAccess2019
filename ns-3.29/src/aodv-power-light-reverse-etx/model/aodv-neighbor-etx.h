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
 
#ifndef AODVNEIGHBORETX_H
#define AODVNEIGHBORETX_H

#include <map>
#include "ns3/ipv4-address.h"
#include "ns3/aodv-packet.h"

namespace ns3
{
namespace aodv
{

class NeighborEtx
{
public:
  NeighborEtx ();
  struct Etx
  {
    uint16_t m_lppMyCnt10bMap;
    double m_rxPower;
    Etx () : m_lppMyCnt10bMap (0), m_rxPower (0.0) {}
  };
  
  // Returns current time stamp (it is needed for sending LPP packet; it is used as LPP ID)
  uint8_t GetLppTimeStamp () const {return m_lppTimeStamp; }
  // This function is used to prepare for new cycle of sending LPP packets.
  // It clears oldest LPP count data and moves to the new time stamp.
  // These two, the oldest and this new current time stamp are not used for
  // calculation of ETX metric (previous 10 time stamps is used for ETX calculations) 
  void GotoNextTimeStampAndClearOldest ();
  // When node receive LPP from a neighbor, it updates my lpp count for that neighbor.
  bool UpdateNeighborEtx (Ipv4Address addr, uint8_t lppTimeStamp, double power);
  // Look for neighbor and return its ETX, return etx ->oo (UINT8_MAX) if there
  // is no neighbor in the map (this is unlikely since it will receive at least
  // one LPP packet from this neighbor and therefore neighbor will be in the map).
  uint8_t GetEtxForNeighbor (Ipv4Address addr) const;
  // Return max possible ETX value, it is max (uint32_t)
  static uint8_t EtxMaxValue () { return UINT8_MAX; };
  // Clear ETX for all neighbors.
  void Clear ()
  {
    m_neighborEtx.clear ();
  }
  void SetCoeficients (double a, double b) { m_a = a; m_b = b; }
private:
  std::map<Ipv4Address, Etx> m_neighborEtx;
  uint8_t m_lppTimeStamp; // has to be incremented every lpp time period; holds last 10 events  
  
  uint8_t CalculateBinaryShiftedEtx (struct Etx etxStruct) const;
  uint8_t Lpp10bMapToCnt (uint16_t lpp10bMap) const;
  void GotoNextLppTimeStamp ();
  static uint8_t CalculateNextLppTimeStamp (uint8_t currTimeStamp);
  double m_a, m_b; // etx metric parameters
};

} // namespace aodv
} // namespace ns3


#endif /* AODVNEIGHBORETX_H */

