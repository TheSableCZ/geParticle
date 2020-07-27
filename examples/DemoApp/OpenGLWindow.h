//
// Created by forry on 24.11.2017.
//

#pragma once

#include <imgui.h>
#include <QtGui/QWindow>
#include <memory>

class QOpenGLPaintDevice;

namespace ge
{
	namespace gl
	{
		class Context;
		class Program;
		class VertexArray;
	}

	namespace particle
	{
		class ParticleSystemManager;
	}

	namespace examples
	{
		class PSManager;
		class ComponentSystemPSManager;
		class ExampleManager;

		/**
		 * Basic QWindow subclass for simple OpenGL rendering.
		 */
		class OpenGLWindow : public QWindow
		{
		Q_OBJECT
		public:
			explicit OpenGLWindow(QWindow* parent = 0);

			~OpenGLWindow();

			virtual void render();
			virtual void initialize();

		public slots:

			void renderNow();

		protected:
			bool event(QEvent* event) override;
			void exposeEvent(QExposeEvent* event) override;
			void resizeEvent(QResizeEvent* event) override;
			void keyPressEvent(QKeyEvent * event) override;
			void wheelEvent(QWheelEvent * event) override;
			void printError() const;

		private:
			bool initialized;

			std::shared_ptr<ExampleManager> exampleManager;

			std::shared_ptr<ge::gl::Context> gl;
			QOpenGLContext* context;
			QSurfaceFormat surfaceFormat;

		private:
			bool show_test_window = true;
			bool show_another_window = false;
			ImVec4 clear_color = ImColor(193, 210, 216);
		};
	}
}
