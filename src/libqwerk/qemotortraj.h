//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

#ifndef QEMOTORTRAJ_H
#define QEMOTORTRAJ_H

#include <pthread.h>
#include "qemotoruser.h"
#include "qemotorrec.h"

#define QEM_BIT(n)                 (1<<n)
#define QEM_POS_SCALE              8

class CQwerkHardware;

class CQEMotorTraj : public CQEMotorUser, public CQEMotorRec
{
public:
  CQEMotorTraj(int axis0, int axis1=-1, int axis2=-1, int axis3=-1);
  ~CQEMotorTraj();

  bool Done(unsigned int axis);
  void Stop(unsigned int axis);
  virtual void Move(unsigned int axis,
		    Axis_position endPosition, int velocity, 
		    unsigned int acceleration, bool absolute=false);
  virtual void MoveVelocity(unsigned int axis,
			    int velocity, unsigned int acceleration);

  // trajectory generating methods
  void TrapezoidTrajectory();
  void RecordTrajectory();

  // write trajectory waypoints to device
  void WriteTrajectory();

  // read positions from motors
  void ReadPosition();

  // record methods
  virtual void Record();
  virtual void Play();

  bool m_threadRun;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
  
protected:
  pthread_t m_thread;
  unsigned char  m_operAxes;
  unsigned short m_trajectory;
  unsigned short m_velocityTrajectory;

  static void *TrajThread(void *arg);
  Axis_position m_trajectoryEndPosition[QEMOT_NUM_MOTORS]; // enc << posScale
  int m_trajectoryVelocity[QEMOT_NUM_MOTORS]; // enc/period << posScale
  unsigned int m_trajectoryAcceleration[QEMOT_NUM_MOTORS]; // enc/period/period
 
  // Trajectory generator output   
  int           m_generatedTrajectoryVelocity[QEMOT_NUM_MOTORS];   
  int           m_generatedTrajectoryVelocityUs[QEMOT_NUM_MOTORS];   
  long long     m_generatedTrajectoryPosition[QEMOT_NUM_MOTORS];
  Axis_position m_generatedTrajectoryPositionUs[QEMOT_NUM_MOTORS]; 
  int           m_accIntegral[QEMOT_NUM_MOTORS]; // enc << posScale
  Axis_position m_readPositionUs[QEMOT_NUM_MOTORS];
};

#endif
