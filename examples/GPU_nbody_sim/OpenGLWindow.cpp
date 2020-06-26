
#include "OpenGLWindow.h"

#include <QtGui/QOpenGLContext>
#include <geGL/geGL.h>
#include <QKeyEvent>

#include <iostream>

#include "particles/PSManager.h"
#include "utils/Camera.h"

//! [ctor]

ge::examples::OpenGLWindow::OpenGLWindow(QWindow *parent)
      : QWindow(parent)
      , initialized(false)
      , context(nullptr)
{
   setSurfaceType(QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
   surfaceFormat.setVersion(4, 5);
   surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
}

//! [ctor]

ge::examples::OpenGLWindow::~OpenGLWindow()
{
}

/**
 * Create OpenGL context with Qt with appropriate surface format.
 * Then initialize geGL and creating geGL context wrapper with OpenGL
 * functions entry points. Also prints out the GL_VERSION string.
 */
void ge::examples::OpenGLWindow::initialize()
{
   if (initialized) return;
   //! [qt_context]

   if (!context)
   {
      context = new QOpenGLContext(this);
      context->setFormat(surfaceFormat);
      bool success = context->create();
      if (!success)
      {
         //fail gracefully
      }
   }

   //! [qt_context]

   //! [makeCurrent]

   //let's say to the OS that we want to work with this context
   context->makeCurrent(this);

   //! [makeCurrent]

   //! [geGL_init]

   /*GPUEngine Init*/
   ge::gl::init();
   gl = std::make_shared<ge::gl::Context>();

   //! [geGL_init]
   //check the version you got - might be different of what you wanted
   std::cout << "OpenGL version:\n  " << gl->glGetString(GL_VERSION) << std::endl;

   psManager = std::make_shared<PSManager>();
   psManager->initialize();

   Camera::getInstance().setWindowDimension((float)width(), (float)height());

   cameraPos = glm::vec3(0, 0, -20);
   Camera::getInstance().setCameraPos(cameraPos);

   printError();

   initialized = true;
}

//! [render]

void ge::examples::OpenGLWindow::render()
{
   const qreal retinaScale = devicePixelRatio();
   gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
   gl->glClearColor(1, 0.5, 0.2, 1.0);
   gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   psManager->update();

   printError();

   context->swapBuffers(this);
}

//! [render]

//! [renderNow]

void ge::examples::OpenGLWindow::renderNow()
{
   if (!isExposed()) return;
   if (!initialized) initialize();

   context->makeCurrent(this);
   //context never got released so no need to make it current again

   render(); //do the simple rendering

   requestUpdate();

   //release context only if necessary
}

//! [renderNow]

//! [eventFilter]

bool ge::examples::OpenGLWindow::event(QEvent *event)
{
   switch (event->type())
   {
      case QEvent::UpdateRequest:
         renderNow();
         return true;
      default:
         return QWindow::event(event);
   }
}

//! [eventFilter]

//! [expose]

void ge::examples::OpenGLWindow::exposeEvent(QExposeEvent *event)
{
   if (isExposed())
   {
      renderNow();
   }
}

//! [expose]

void ge::examples::OpenGLWindow::resizeEvent(QResizeEvent * event)
{
	QWindow::resizeEvent(event);
	Camera::getInstance().setWindowDimension((float)width(), (float)height());
}

void ge::examples::OpenGLWindow::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Left) {
		//std::cout << "LeftArrow" << std::endl;
		cameraPos.x -= 1.f;
		Camera::getInstance().setCameraPos(cameraPos);
	}
	if (event->key() == Qt::Key_Right) {
		cameraPos.x += 1.f;
		Camera::getInstance().setCameraPos(cameraPos);
	}
	if (event->key() == Qt::Key_Up) {
		cameraPos.y += 1.f;
		Camera::getInstance().setCameraPos(cameraPos);
	}
	if (event->key() == Qt::Key_Down) {
		cameraPos.y -= 1.f;
		Camera::getInstance().setCameraPos(cameraPos);
	}
}

void ge::examples::OpenGLWindow::printError() const
{
	auto err = this->gl->glGetError();
	if (err != GL_NO_ERROR)
	{

		std::cout << "chyba " << err << std::endl;
	}
}