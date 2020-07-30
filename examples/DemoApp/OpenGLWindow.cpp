//
// Created by forry on 24.11.2017.
//

#include <OpenGLWindow.h>

#include <QtGui/QOpenGLContext>
#include <geGL/geGL.h>

#include <iostream>
#include <glm/glm.hpp>

#include <QKeyEvent>
#include <geParticle/ParticleSystemManager.h>

#include "CameraSingleton.h"
#include "ExampleManager.h"
#include "lib/qtimgui/QtImGui.h"

//! [ctor]

ge::examples::OpenGLWindow::OpenGLWindow(QWindow* parent)
	: QWindow(parent)
	  , initialized(false)
	  , context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
	//this needs to be set otherwise makeCurrent and other gl context related functions will fail
	surfaceFormat.setVersion(4, 5);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
}

//! [ctor]

ge::examples::OpenGLWindow::~OpenGLWindow()
= default;

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

	exampleManager = std::make_shared<ExampleManager>();

	CameraSingleton::getInstance().setWindowDimension((float)width(), (float)height());
	//CameraSingleton::getInstance().setPosition(glm::vec3(0, 0, -5));
	
	QtImGui::initialize(this);

	initialized = true;
}

//! [render]

void ge::examples::OpenGLWindow::render()
{
	QtImGui::newFrame();

	const qreal retinaScale = devicePixelRatio();
	gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	gl->glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	gl->glClear(GL_COLOR_BUFFER_BIT);

	//ImGui::ColorEdit3("clear color", (float*)&clear_color);

	exampleManager->render();

	ImGui::Render();
	
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

bool ge::examples::OpenGLWindow::event(QEvent* event)
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

void ge::examples::OpenGLWindow::exposeEvent(QExposeEvent* event)
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
	CameraSingleton::getInstance().setWindowDimension(static_cast<float>(width()), static_cast<float>(height()));
}

void ge::examples::OpenGLWindow::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Left) {
		CameraSingleton::getInstance().moveXY(-.1f, 0);
	}
	if (event->key() == Qt::Key_Right) {
		CameraSingleton::getInstance().moveXY(.1f, 0);
	}
	if (event->key() == Qt::Key_Up) {
		CameraSingleton::getInstance().moveXY(0, .1f);
	}
	if (event->key() == Qt::Key_Down) {
		CameraSingleton::getInstance().moveXY(0, -.1f);
	}

	if (event->key() == Qt::Key_F1) {
		exampleManager->toggleShowGui();
	}
	if (event->key() == Qt::Key_Space)
	{
		exampleManager->togglePaused();
	}
	if (event->key() == Qt::Key_Escape)
	{
		exampleManager->stopEmitting();
	}
}

void ge::examples::OpenGLWindow::wheelEvent(QWheelEvent * event)
{
	QPoint numDegrees = event->angleDelta() / 8;
	QPoint numSteps = numDegrees / 15;
	CameraSingleton::getInstance().moveZ(static_cast<float>(-numSteps.y()));
}

void ge::examples::OpenGLWindow::printError() const
{
	auto err = this->gl->glGetError();
	if (err != GL_NO_ERROR)
	{
		std::cout << "chyba " << err << std::endl;
	}
}
