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

#include <iostream>
#include "import-propagation-loss-model.h"
#include "ns3/double.h"
#include "ns3/log.h"

using namespace std;

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("Import");

NS_OBJECT_ENSURE_REGISTERED (ImportPropagationLossModel);

ImportPropagationLossModel::ImportPropagationLossModel()
{
  m_uniformVariable = CreateObject<UniformRandomVariable> ();
  m_uniformVariable->SetAttribute ("Min", DoubleValue (-1.0 * M_PI));
  m_uniformVariable->SetAttribute ("Max", DoubleValue (M_PI));
}

ImportPropagationLossModel::~ImportPropagationLossModel()
{}

vector<vector<double>> ImportPropagationLossModel::pathloss;
double ImportPropagationLossModel::distanceLastNext = 0;
double ImportPropagationLossModel::lastIndex = 0;

double 
ImportPropagationLossModel::distance(double x, double y, double x1, double y1) {
    return sqrt(pow(x-x1, 2) + pow(y-y1, 2));
}

double 
ImportPropagationLossModel::interpolatedPathLoss(double x, double y) {
    if(lastIndex == pathloss.size() - 1) {
      return pathloss[lastIndex][2];
    }
    double lastX = pathloss[lastIndex][0];
    double lastY = pathloss[lastIndex][1];
    double nextX = pathloss[lastIndex + 1][0];
    double nextY = pathloss[lastIndex + 1][1];
    double distance1 = distance(lastX, lastY, x, y);
    double distance2 = distance(nextX, nextY, x, y);
    distanceLastNext = distance(lastX, lastY, nextX, nextY);
    if (distance1 > distanceLastNext) {
      lastIndex += 1;
      return interpolatedPathLoss(x, y);
    }
    double lastPathLoss = pathloss[lastIndex][2];
    double nextPathLoss = pathloss[lastIndex + 1][2];
    double thisPathLoss = lastPathLoss + (distance1/(distance1 + distance2))*(nextPathLoss-lastPathLoss);
    return thisPathLoss; 
  }

void 
ImportPropagationLossModel::splitCSVLine(string line) {
    vector<double> col; 
    string s = "";
    char del = ',';
    for (auto x : line) {
      if (x == del) {
        double tmp = stod(s);
        col.push_back(tmp);
        s.clear();
      } else {
        s = s + x; 
      }
    }
    double tmp = stod(s);
    col.push_back(tmp);
    pathloss.push_back(col);
}

void 
ImportPropagationLossModel::readCSV(string path) { 
    ifstream file;
    file.open(path);
    string fileString;
    if (file.is_open()) {
      while(!file.eof()) {
        file >> fileString; 
        splitCSVLine(fileString);
      }		
    }
}

TypeId
ImportPropagationLossModel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::ImportPropagationLossModel")
    .SetParent<PropagationLossModel> ()
    .SetGroupName ("Propagation")
    .AddConstructor<ImportPropagationLossModel> ()
  ;
  return tid;
}

void
ImportPropagationLossModel::DoDispose ()
{
  m_uniformVariable = nullptr;
  m_propagationCache.Cleanup ();
}

double
ImportPropagationLossModel::DoCalcRxPower (double txPowerDbm, Ptr<MobilityModel> a, Ptr<MobilityModel> b) const 
{	
 	double currentx	= a->GetPosition().x + b->GetPosition().x;
 	double currenty	= a->GetPosition().y + b->GetPosition().y;
	
	return interpolatedPathLoss(currentx, currenty); 	
}

Ptr<UniformRandomVariable>
ImportPropagationLossModel::GetUniformRandomVariable () const
{
  return m_uniformVariable;
}

int64_t
ImportPropagationLossModel::DoAssignStreams (int64_t stream)
{
  m_uniformVariable->SetStream (stream);
  return 1;
}

} // namespace ns3

