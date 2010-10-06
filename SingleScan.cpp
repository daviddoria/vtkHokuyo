#include "UrgCtrl.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>

#include <vtkSmartPointer.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph2D.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleImage.h>

using namespace qrk;

//global
UrgCtrl urg;

int main(int argc, char *argv[])
{
  const char device[] = "/dev/ttyACM0";
  
  if (!urg.connect(device)) 
    {
    //printf("UrgCtrl::connect: %s\n", urg.what());
    std::cout << "Failed to connect: " << urg.what() << std::endl;;
    exit(-1);
    }
  else
    {
    std::cout << "Connected successfully!" << std::endl;;
    }
    
  std::cout << "Collecting points..." << std::endl;
  std::vector<long> data;
  long timestamp = 0;
  
  int n = urg.capture(data, &timestamp);
  //printf("n = %d\n", n);
  std::cout << "Collected " << n << " points." << std::endl;
  if (n < 0) 
    {
    //printf("UrgCtrl::capture: %s\n", urg.what());
    std::cout << "UrgCtrl::capture: " <<  urg.what() << std::endl;
    exit(-1);
    }

  long min_distance = urg.minDistance();
  long max_distance = urg.maxDistance();

  vtkSmartPointer<vtkPoints> points = 
    vtkSmartPointer<vtkPoints>::New();
    
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
  std::cout << "Points has " << points->GetNumberOfPoints() << " points." << std::endl;
  
  vtkSmartPointer<vtkPolyData> polydata = 
    vtkSmartPointer<vtkPolyData>::New();
  polydata->SetPoints(points);

  vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter = 
    vtkSmartPointer<vtkVertexGlyphFilter>::New();
  glyphFilter->SetInputConnection(polydata->GetProducerPort());
  glyphFilter->Update();

  vtkSmartPointer<vtkPolyDataMapper> mapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(glyphFilter->GetOutputPort());
  mapper->Update();

  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  renderer->AddActor(actor);
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

   vtkSmartPointer<vtkInteractorStyleImage> style = 
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  renderWindowInteractor->SetInteractorStyle( style );
 
  
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
