#include <QtGui/QGuiApplication>
#include <OpenGLWindow.h>
#include <QtGui/QOpenGLContext>

//! [main]
int main(int argc, char **argv)
{
	QGuiApplication app(argc, argv);

	ge::particle::OpenGLWindow window;
	window.resize(840, 640);
	window.show();

	return app.exec();
}
//! [main]