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
#include <vtkPointSource.h>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

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
      this->TimerCount = 0;

      this->actor = vtkSmartPointer<vtkActor>::New();
      this->actor->GetProperty()->SetPointSize(3);
      this->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

      this->actor->SetMapper(this->mapper);

      this->hokuyo = vtkSmartPointer<vtkHokuyo>::New();

    }

    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void *vtkNotUsed(callData))
    {
      if (vtkCommand::TimerEvent != eventId)
        {
        return;
        }

      std::cout << "Debugging: Updating points..." << std::endl;

      hokuyo->Modified();
      hokuyo->Update();

      //this->polydata->SetPoints(this->points);
      //this->glyphFilter->Update();
      this->mapper->SetInputConnection(hokuyo->GetOutputPort());
      this->mapper->Update();

      this->actor->GetProperty()->SetColor(1.0, 0.0, 0.0);

      this->renderer->ResetCamera();
      this->renderWindow->Render();
      ++this->TimerCount;

      // Screenshot
      vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter =
        vtkSmartPointer<vtkWindowToImageFilter>::New();
      windowToImageFilter->SetInput(this->renderWindow);
      //windowToImageFilter->SetMagnification(3); //set the resolution of the output image
      windowToImageFilter->Update();

      // Construct file name
      std::stringstream padded;
      padded << std::setfill('0') << std::setw(5) << this->TimerCount << ".png";
      std::cout << "Filename: " << padded.str() << std::endl;

      vtkSmartPointer<vtkPNGWriter> writer =
	  vtkSmartPointer<vtkPNGWriter>::New();
      writer->SetFileName(padded.str().c_str());
      writer->SetInput(windowToImageFilter->GetOutput());
      writer->Write();

    }

  private:
    unsigned int TimerCount;
    vtkSmartPointer<vtkHokuyo> hokuyo;

  public:
    vtkSmartPointer<vtkActor> actor;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
    vtkSmartPointer<vtkRenderWindow> renderWindow;
    vtkSmartPointer<vtkRenderer> renderer;

};

int main(int, char *[])
{
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer =
      vtkSmartPointer<vtkRenderer>::New();
  renderer->SetBackground(1,1,1); // Background color white

  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(700,700); //(width, height)

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
  int timerId = renderWindowInteractor->CreateRepeatingTimer(700);
  //std::cout << "timerId: " << timerId << std::endl;

  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
