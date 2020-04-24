//
// Created by forry on 24.11.2017.
//

#pragma once

#include <QtGui/QWindow>
#include <memory>
#include <geParticle/ParticleSystem.h>
#include <geParticle/ParticleFactory.h>
#include <geParticle/ParticleAffector.h>
#include <geUtil/Timer.h>

class QOpenGLPaintDevice;

namespace ge
{
	namespace gl
	{
		class Context;
		class Program;
		class VertexArray;
		class Buffer;
	}
	namespace particle
	{

        class MyParticleFactory : public BasicParticleFactory
        {
        public:
			void initParticle(ge::particle::Particle &p) override {
				BasicParticleFactory::initParticle(p);

				p.r = ((float)rand() / (RAND_MAX));
				p.g = ((float)rand() / (RAND_MAX));
				p.b = ((float)rand() / (RAND_MAX));
			}

            void place(ge::particle::Particle &p) override {
                p.pos = glm::vec3(.0f, -1.f, .0f);
                //p.pos = glm::vec3(x, y, .0f);
            }
            float x;
            float y;

            void shoot(Particle &p) override {
				float spread = 1.5f;
				glm::vec3 maindir = glm::vec3(0.0f, 5.0f, 0.0f);
				// Very bad way to generate a random direction; 
				// See for instance http://stackoverflow.com/questions/5408276/python-uniform-spherical-distribution instead,
				// combined with some user-controlled parameters (main direction, spread, etc)
				glm::vec3 randomdir = glm::vec3(
					(rand() % 2000 - 1000.0f) / 1000.0f,
					(rand() % 2000 - 1000.0f) / 1000.0f,
					(rand() % 2000 - 1000.0f) / 1000.0f
				);

                p.velocity = maindir + randomdir * spread;
            }

        };

		class FloorAffector : public GravityAffector 
		{
			void affect(core::time_unit dt, Particle &p) {
				if (p.pos.y > -2.f) GravityAffector::affect(dt, p);
			}
		};

		/**
		 * Basic QWindow subclass for simple OpenGL rendering.
		 */
		class OpenGLWindow : public QWindow
		{
			Q_OBJECT
		public:
			explicit OpenGLWindow(QWindow *parent = 0);

			~OpenGLWindow();

			virtual void initialize();
			virtual void render();

			static const std::string fragmentShaderSrc;

		public slots:

			void renderNow();

		protected:
			bool event(QEvent *event) override;
            void mouseMoveEvent(QMouseEvent *event) override;
			void resizeEvent(QResizeEvent * event) override;
			void exposeEvent(QExposeEvent *event) override;
			void keyPressEvent(QKeyEvent * event) override;
			void printError() const;

		private:
			bool initialized;

			QOpenGLContext *context;
			QSurfaceFormat surfaceFormat;

			std::shared_ptr<ge::gl::Context> gl;
			std::shared_ptr<ge::gl::Program> shaderProgram;

			static std::vector<float> particleVertices;
			std::shared_ptr<ge::gl::Buffer> positions;
			std::shared_ptr<ge::gl::VertexArray> VAO;

			std::shared_ptr<ge::particle::ParticleSystem> ps; 
			
			//ge::util::Timer<double> *timer;

			std::shared_ptr<ge::gl::Buffer> SSBO;

			glm::mat4 viewMatrix;
			glm::mat4 mvp;
			glm::vec3 cameraUp;
			glm::vec3 cameraRight;

			void calculateMVP();
			void calculateViewMatrix();
			void calculateCameraVectors();

			bool mvpChanged;

			const std::vector<glm::vec3> cameraPositions = { glm::vec3(0,0,-5), glm::vec3(4,3,3), glm::vec3(0,-4,3) };
			int cameraPositionsIndex = 0;

			/*
			static std::vector<float> trianglePos;
			static std::vector<float> triangleCol;
			static std::vector<unsigned> indices;
			std::shared_ptr<ge::gl::Program> shaderProgram;
			std::shared_ptr<ge::gl::Buffer> SSBO;
			std::shared_ptr<ge::gl::VertexArray> VAO;
			std::shared_ptr<ge::gl::Buffer> positions;
			std::shared_ptr<ge::gl::Buffer> element;
			*/

			void initializeParticles();
			void renderParticles();
			std::shared_ptr<ge::gl::Buffer> particleCenters;
			std::shared_ptr<ge::gl::Buffer> particleColors;
			float *particlesPositions;
			float *particlesColors;
			int updateBuffer();
			MyParticleFactory *mpf;
		};
	}
}
