/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 Telum (www.telum.ru)
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
 * Author: Kirill Andreev <andreev@telum.ru>
 */
  
#ifndef IMPORT_STATIONARY_LOSS_MODEL_H
#define IMPORT_STATIONARY_LOSS_MODEL_H

#include<iostream> 
#include<fstream> 
#include<string> 
#include<vector>
#include<cmath>   
#include "ns3/propagation-loss-model.h"
#include "ns3/propagation-cache.h"
#include "ns3/jakes-process.h"

using namespace std;

namespace ns3
{
/**
 * \ingroup propagation
 *
 * \brief a  Import narrowband propagation model.
 * Symmetrical cache for ImportProcess
 */

class ImportPropagationLossModel : public PropagationLossModel
{
  
public:
  static vector<vector<double>> pathloss;
  static double lastIndex; 
  static double distanceLastNext;
  
  void readCSV(string path);
  

  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId ();
  ImportPropagationLossModel ();
  virtual ~ImportPropagationLossModel ();

  // Delete copy constructor and assignment operator to avoid misuse
  ImportPropagationLossModel (const ImportPropagationLossModel &) = delete;
  ImportPropagationLossModel & operator = (const ImportPropagationLossModel &) = delete;

protected:
  virtual void DoDispose () override;

private:
  friend class JakesProcess;

  double DoCalcRxPower (double txPowerDbm,
                        Ptr<MobilityModel> a,
                        Ptr<MobilityModel> b) const override;

  int64_t DoAssignStreams (int64_t stream) override;
  
  static double interpolatedPathLoss(double x, double y);
  static void splitCSVLine(string line);
  static double distance(double x, double y, double x1, double y1);
  
  /**
   * Get the underlying RNG stream
   * \return the RNG stream
   */
  Ptr<UniformRandomVariable> GetUniformRandomVariable () const;

  Ptr<UniformRandomVariable> m_uniformVariable; //!< random stream
  mutable PropagationCache<JakesProcess> m_propagationCache; //!< Propagation cache
};

} // namespace ns3

#endif /* JAKES_STATIONARY_LOSS_MODEL_H */

