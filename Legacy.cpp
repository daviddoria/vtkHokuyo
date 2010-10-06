#include "vtkHokuyo.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <vector>
#include <sstream>

#include <vtkSmartPointer.h>
#include <vtkProperty.h>
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
#include <vtkCommand.h>
#include <vtkRendererCollection.h>
#include <vtkXMLPolyDataWriter.h>

class vtkTimerCallback : public vtkCommand
{
  public:
    static vtkTimerCallback *New()
    {
      vtkTimerCallback *cb = new vtkTimerCallback;
      
      return cb;
    }
    
    vtkTimerCallback()
    {
      std::cout << "Constructor called." << std::endl;
      this->hokuyo = vtkSmartPointer<vtkHokuyo>::New();
      this->TimerCount = 0;
      
      this->glyphFilter = vtkSmartPointer<vtkVertexGlyphFilter>::New();
      this->actor = vtkSmartPointer<vtkActor>::New();
      this->points = vtkSmartPointer<vtkPoints>::New();
      this->polydata = vtkSmartPointer<vtkPolyData>::New();
      this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    
      
      this->actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
      this->actor->GetProperty()->SetPointSize(3);
      
      this->glyphFilter->SetInputConnection(this->polydata->GetProducerPort());
      this->mapper->SetInputConnection(this->glyphFilter->GetOutputPort());
      
    }
 
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void *vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent != eventId)
        {
	return;
	}
	
      this->actor->SetMapper(this->mapper);
      this->actor->Modified();
      this->points->Modified();
      this->polydata->Modified();
      std::cout << "Points has " << this->points->GetNumberOfPoints() << " points." << std::endl;
      
      this->polydata->SetPoints(this->points);
      this->glyphFilter->Update();
      this->mapper->Update();
          
      /*
      vtkSmartPointer<vtkXMLPolyDataWriter> writer = 
	vtkSmartPointer<vtkXMLPolyDataWriter>::New();
      writer->SetInputConnection(this->glyphFilter->GetOutputPort());
      std::stringstream ss; 
      ss << this->TimerCount << ".vtp";
      writer->SetFileName(ss.str().c_str());
      writer->Write();
      */
      //this->renderer->ResetCamera();
      
      this->renderer->Render();
      this->renderWindow->Render();
      ++this->TimerCount;
        
        
    }
 
  private:
    vtkSmartPointer<vtkVertexGlyphFilter> glyphFilter;
    unsigned int TimerCount;
    vtkSmartPointer<vtkHokuyo> hokuyo;
	
  public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkPolyData> polydata;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;
};

int main(int argc, char *argv[])
{
  
  
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1,1,1); // Background color white
  
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);

   vtkSmartPointer<vtkInteractorStyleImage> style = 
    vtkSmartPointer<vtkInteractorStyleImage>::New();
  renderWindowInteractor->SetInteractorStyle( style );
 
  // Sign up to receive TimerEvent
  vtkSmartPointer<vtkTimerCallback> cb = 
    vtkSmartPointer<vtkTimerCallback>::New();
  renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
 
  cb->renderer = renderer;
  cb->renderWindow = renderWindow;
  cb->renderWindowInteractor = renderWindowInteractor;
  
  cb->renderer->AddActor(cb->actor);
  
  // Initialize must be called prior to creating timer events.
  renderWindowInteractor->Initialize();
  int timerId = renderWindowInteractor->CreateRepeatingTimer(500);
  //std::cout << "timerId: " << timerId << std::endl;
  
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
