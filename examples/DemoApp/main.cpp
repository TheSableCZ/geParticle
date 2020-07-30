/** @file main.cpp
 *  @brief DemoApp main.
 *  @author Jan Sobol xsobol04
 */

#include <QtGui/QGuiApplication>
#include <OpenGLWindow.h>
#include <sstream>
#include <QtGui/QOpenGLContext>

//! [main]

int main(int argc, char** argv)
{
	QGuiApplication app(argc, argv);

	ge::examples::OpenGLWindow window;

	int w, h;
	if (argc > 2)
	{
		std::stringstream sw(argv[1]);
		sw >> w;

		std::stringstream sh(argv[2]);
		sh >> h;
	}
	else
	{
		w = 1280;
		h = 720;
	}

	window.resize(w, h);
	window.show();

	return app.exec();
}

//! [main]
