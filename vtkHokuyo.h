#ifndef __vtkHokuyo_h
#define __vtkHokuyo_h

#include "vtkPolyDataAlgorithm.h"

#include "urg/UrgCtrl.h"
using namespace qrk;

class vtkHokuyo : public vtkPolyDataAlgorithm
{
public:
  vtkTypeRevisionMacro(vtkHokuyo,vtkPolyDataAlgorithm);
  static vtkHokuyo *New();

protected:
  vtkHokuyo();
  ~vtkHokuyo(){}

  int FillOutputPortInformation( int port, vtkInformation* info );
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkHokuyo(const vtkHokuyo&);  // Not implemented.
  void operator=(const vtkHokuyo&);  // Not implemented.

  UrgCtrl urg;
  long min_distance;
  long max_distance;
};

#endif