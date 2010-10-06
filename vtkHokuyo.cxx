#include "vtkHokuyo.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"
#include "vtkDataObject.h"
#include "vtkSmartPointer.h"
#include "vtkVertexGlyphFilter.h"

vtkStandardNewMacro(vtkHokuyo);

vtkHokuyo::vtkHokuyo()
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(1);

  const char device[] = "/dev/ttyACM0";

  if (!this->urg.connect(device))
    {
    //printf("UrgCtrl::connect: %s\n", urg.what());
    std::cout << "Failed to connect: " << this->urg.what() << std::endl;;
    exit(-1);
    }
  else
    {
    std::cout << "Connected successfully!" << std::endl;;
    }

  this->min_distance = urg.minDistance();
  this->max_distance = urg.maxDistance();

}

int vtkHokuyo::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // Get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // Get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
            outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkPolyData> polydata =
    vtkSmartPointer<vtkPolyData>::New();
  vtkSmartPointer<vtkPoints> points =
    vtkSmartPointer<vtkPoints>::New();

  std::cout << "Collecting points..." << std::endl;

  std::vector<long> data;
  long timestamp = 0;

  int n = urg.capture(data, &timestamp);

  std::cout << "Collected " << n << " points at " << timestamp << std::endl;
  if (n < 0)
    {
    std::cout << "Captured a negative number of points! UrgCtrl::capture: " <<  urg.what() << std::endl;
    return -1;
    }

  // Convert the acquired point from an angle and distance to a (x,y) coordinate
  for (int i = 0; i < n; ++i)
    {
    long l = data[i];
    if ((l <= min_distance) || (l >= max_distance))
      {
      continue;
      }
    double rad = urg.index2rad(i);
    long x = static_cast<long>(l * cos(rad));
    long y = static_cast<long>(l * sin(rad));
    //std::cout << "(" << x << ", " << y << "), " << std::endl; // (x [mm], y [mm])
    points->InsertNextPoint(x,y,0);
    }

  polydata->SetPoints(points);

  // Add vertices to the points
  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter =
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
  glyphFilter->SetInputConnection(polydata->GetProducerPort());
  glyphFilter->Update();

  output->ShallowCopy(glyphFilter->GetOutput());

  std::cout << "vtkHokuyo: Exit RequestData()" << std::endl;
  return 1;
}