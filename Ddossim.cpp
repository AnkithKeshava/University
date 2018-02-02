/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
*/
    
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/netanim-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/point-to-point-layout-module.h"
    
// Network topology (default)
//
//        n2 n3 n4              .
//         \ | /                .
//          \|/                 .
//     n1--- n0---n5            .
//          /|\                 .
//         / | \                .
//        n8 n7 n6              .
//
    
using namespace ns3;
    
NS_LOG_COMPONENT_DEFINE ("Star");
    
int main (int argc, char *argv[])
{
  // Set up some default values for the simulation.
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", UintegerValue (137));

  // ??? try and stick 15kb/s into the data rate
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("14kb/s"));

  // Default number of nodes in the star.  Overridable by command line argument.
  uint32_t nSpokes = 6;
  uint32_t nCsma = 3;

  CommandLine cmd;
  cmd.AddValue ("nSpokes", "Number of nodes to place in the star", nSpokes);
  cmd.Parse (argc, argv);

  //Building two star topologies
  NS_LOG_INFO ("Build star topology.");
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointStarHelper star1 (nSpokes, pointToPoint);
  PointToPointStarHelper star2 (nSpokes, pointToPoint);
  PointToPointStarHelper star3 (nSpokes, pointToPoint);
  PointToPointStarHelper star4 (nSpokes, pointToPoint);
  PointToPointStarHelper star5 (nSpokes, pointToPoint);

  //Nodes for Network backbone
  NodeContainer p2pNodes, p2pNodes1,p2pNodes2, p2pNodes3,p2pNodes4, p2pNodes5, p2pBB;
  p2pNodes.Create (2);
  
  p2pNodes1.Add(star1.GetHub());
  p2pNodes1.Add(p2pNodes.Get(1));
  
  p2pNodes2.Add(star2.GetHub());
  p2pNodes2.Add(p2pNodes.Get(0));
  
  p2pNodes3.Add(star3.GetHub());
  p2pNodes3.Add(p2pNodes.Get(0));
  
  p2pNodes4.Add(star4.GetHub());
  p2pNodes4.Add(p2pNodes.Get(1));
  
  p2pBB.Add(p2pNodes.Get(1));
  p2pBB.Create(1);
  
  p2pNodes5.Add(star5.GetHub());
  p2pNodes5.Add(p2pBB.Get(1));

  //Install point to point services in backbone nodes
  NetDeviceContainer p2pDevices;
  p2pDevices = pointToPoint.Install (p2pNodes);
  p2pDevices = pointToPoint.Install (p2pNodes1);
  p2pDevices = pointToPoint.Install (p2pNodes2);
  p2pDevices = pointToPoint.Install (p2pNodes3);
  p2pDevices = pointToPoint.Install (p2pNodes4);
  p2pDevices = pointToPoint.Install (p2pNodes5);
  p2pDevices = pointToPoint.Install (p2pBB);

  //Building csma topology connected to star1 network
  NodeContainer csmaNodes;
  csmaNodes.Add (star1.GetHub());
  csmaNodes.Create (nCsma);
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("1Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);
    
  //Installing internet stack in nodes
  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  star1.InstallStack (internet);
  star2.InstallStack (internet);
  star3.InstallStack (internet);
  star4.InstallStack (internet);
  star5.InstallStack (internet);
  InternetStackHelper stack;
  stack.Install (p2pNodes.Get(0));
  stack.Install (p2pNodes.Get(1));
  stack.Install (p2pBB.Get(1));

  //stack.Install (star.GetHub());
  for(uint32_t i=1; i<=nCsma; i++)
  {
  stack.Install (csmaNodes.Get(i));
  }    
        
  //Assigning ip addresses
  NS_LOG_INFO ("Assign IP Addresses.");
  star1.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.252"));
  star2.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.2.0", "255.255.255.252"));
  star3.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.3.0", "255.255.255.252"));
  star4.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.4.0", "255.255.255.252"));
  star5.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.5.0", "255.255.255.252"));
  Ipv4AddressHelper address;
  address.SetBase ("10.1.10.0", "255.255.255.252");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (p2pDevices);
  address.SetBase ("10.2.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);
    
  NS_LOG_INFO ("Create applications.");
  // Create a packet sink on the star "hub" to receive packets.
  uint16_t port = 50000;
  Address hubLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", hubLocalAddress);
  ApplicationContainer hubApp1 = packetSinkHelper.Install (star1.GetHub ());
  ApplicationContainer hubApp2 = packetSinkHelper.Install (star2.GetHub ());
  ApplicationContainer hubApp3 = packetSinkHelper.Install (star3.GetHub ());
  ApplicationContainer hubApp4 = packetSinkHelper.Install (star4.GetHub ());
  ApplicationContainer hubApp5 = packetSinkHelper.Install (star5.GetHub ());
  hubApp1.Start (Seconds (1.0));
  hubApp2.Start (Seconds (1.0));
  hubApp3.Start (Seconds (1.0));
  hubApp4.Start (Seconds (1.0));
  hubApp5.Start (Seconds (1.0));
  hubApp1.Stop (Seconds (10.0));
  hubApp2.Stop (Seconds (10.0));
  hubApp3.Stop (Seconds (10.0));
  hubApp4.Stop (Seconds (10.0));
  hubApp5.Stop (Seconds (10.0));
    
  // Create OnOff applications to send TCP to the hub, one on each spoke node.
  OnOffHelper onOffHelper ("ns3::TcpSocketFactory", Address ());
  onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  ApplicationContainer spokeApps;
    
  for (uint32_t i = 0; i < star1.SpokeCount (); ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star1.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add (onOffHelper.Install (star1.GetSpokeNode(i)));
  }
  for (uint32_t i = 0; i < star2.SpokeCount (); ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star2.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add (onOffHelper.Install (star2.GetSpokeNode(i)));
  }
  for (uint32_t i = 0; i < star3.SpokeCount (); ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star3.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add (onOffHelper.Install (star3.GetSpokeNode(i)));
  }
  for (uint32_t i = 0; i < star4.SpokeCount (); ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star4.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add (onOffHelper.Install (star4.GetSpokeNode(i)));
  }
  for (uint32_t i = 0; i < star5.SpokeCount (); ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star5.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add (onOffHelper.Install (star5.GetSpokeNode(i)));
  }
  for (uint32_t i = 1; i <= nCsma; ++i)
  {
    AddressValue remoteAddress (InetSocketAddress (star1.GetHubIpv4Address (i), port));
    onOffHelper.SetAttribute ("Remote", remoteAddress);
    spokeApps.Add(onOffHelper.Install (csmaNodes.Get(i)));
  }
  spokeApps.Start (Seconds (2.0));
  spokeApps.Stop (Seconds (10.0));
      
  NS_LOG_INFO ("Enable static global routing.");
  // Turn on global static routing so we can actually be routed across the star.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  NS_LOG_INFO ("Enable pcap tracing.");
  // Do pcap tracing on all point-to-point devices on all nodes.
  pointToPoint.EnablePcapAll ("star");

  // Setting up NetAnim
  AnimationInterface anim("practice.xml");
  anim.SetConstantPosition(star1.GetHub(),5.0,15.0);
  anim.SetConstantPosition(star2.GetHub(),5.0,5.0);
  anim.SetConstantPosition(star3.GetHub(),25.0,5.0);
  anim.SetConstantPosition(star4.GetHub(),25.0,15.0);
  anim.SetConstantPosition(star5.GetHub(),5.0,25.0);

  for(uint32_t i=0; i<star1.SpokeCount(); i++)
    anim.SetConstantPosition(star1.GetSpokeNode(i),double(i),20.0);   
  for(uint32_t i=0; i<star2.SpokeCount(); i++)
    anim.SetConstantPosition(star2.GetSpokeNode(i),double(i),10.0); 
  for(uint32_t i=0; i<star3.SpokeCount(); i++)
    anim.SetConstantPosition(star3.GetSpokeNode(i),double(i+20),10.0);   
  for(uint32_t i=0; i<star4.SpokeCount(); i++)
    anim.SetConstantPosition(star4.GetSpokeNode(i),double(i+20),20.0);
  for(uint32_t i=0; i<star5.SpokeCount(); i++)
    anim.SetConstantPosition(star5.GetSpokeNode(i),double(i),30.0);
  for (uint32_t j=1;j<=nCsma;j++)
    anim.SetConstantPosition(csmaNodes.Get(j),double(2*j+5),15.0);

  anim.SetConstantPosition(p2pNodes.Get(0),15.0,3.0);
  anim.SetConstantPosition(p2pNodes.Get(1),15.0,13.0);
  anim.SetConstantPosition(p2pBB.Get(1),15.0,23.0);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
   
  return 0;
}