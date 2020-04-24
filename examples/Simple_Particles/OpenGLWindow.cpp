
#include <OpenGLWindow.h>

#include <QtGui/QOpenGLContext>
#include <geGL/geGL.h>
#include <geUtil/Text.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QDebug>
#include <QMouseEvent>

#include <iostream>

#include <geCore/Updatable.h>
#include <geParticle/ParticleEmitter.h>
#include <geParticle/ParticleAffector.h>

/*std::vector<float> ge::particle::OpenGLWindow::particleVertices = {
															 -0.01f, -0.01f, 0.0f,
															 0.01f, -0.01f, 0.0f,
															 -0.01f, 0.01f, 0.0f,
															 0.01f, 0.01f, 0.0f,
															};*/

std::vector<float> ge::particle::OpenGLWindow::particleVertices = {
															 -.5f, -.5f, 0.0f,
															 .5f, -.5f, 0.0f,
															 -.5f, .5f, 0.0f,
															 .5f, .5f, 0.0f, };

ge::particle::OpenGLWindow::OpenGLWindow(QWindow *parent)
	: QWindow(parent)
	, initialized(false)
	, context(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface); //this needs to be set otherwise makeCurrent and other gl context related functions will fail
	surfaceFormat.setVersion(4, 5);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
}

ge::particle::OpenGLWindow::~OpenGLWindow() 
{
	//delete ps;
	// TODO: do hájíèka dubového s tímto
	delete[] particlesPositions;
	delete[] particlesColors;
	//delete timer;
}

void ge::particle::OpenGLWindow::initialize()
{
	if (initialized) return;

	if (!context)
	{
		context = new QOpenGLContext(this);
		context->setFormat(surfaceFormat);
		bool success = context->create();
		if (!success)
		{
			std::cout << "Something went wrong" << endl;
			exit(-1);
		}
	}

	//let's say to the OS that we want to work with this context
	context->makeCurrent(this);

	ge::gl::init();
	gl = std::make_shared<ge::gl::Context>();

	//timer = new ge::util::Timer<double>();

	std::shared_ptr<ge::gl::Shader> vertexShader = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, ge::util::loadTextFile(VERTEX_SHADER));
	std::shared_ptr<ge::gl::Shader> fragmentShader = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, ge::util::loadTextFile(FRAGMENT_SHADER));
	shaderProgram = std::make_shared<ge::gl::Program>(vertexShader, fragmentShader);


	positions = std::make_shared<ge::gl::Buffer>(particleVertices.size() * sizeof(float), particleVertices.data());

	//glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)width() / height(), 0.1f, 1000.f); // no resizing yet
	//glm::mat4 projection = glm::ortho(0.0f, float(width()), 0.0f, float(height()));

	//glm::mat4 scale = glm::scale<float>(.3f, .3f, .3f);
	//shaderProgram->setMatrix4fv("projection", &projection[0][0]);
	/*GLuint ProjMatrixID = gl->glGetUniformLocation(shaderProgram->getId(), "projection");
	gl->glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &projection[0][0]);*/

	
	calculateMVP();

	/*GLint MVPMatrixID = gl->glGetUniformLocation(shaderProgram->getId(), "MVP");
	gl->glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &mvp[0][0]);*/

	printError();

	VAO = std::make_shared<ge::gl::VertexArray>();
	VAO->addAttrib(positions, 0, 3, GL_FLOAT);

	initializeParticles();

	initialized = true;
}

void ge::particle::OpenGLWindow::render()
{
	if (mvpChanged)
	{
		shaderProgram->setMatrix4fv("MVP", &mvp[0][0]);
		shaderProgram->set3fv("CameraUp", &cameraUp[0]);
		shaderProgram->set3fv("CameraRight", &cameraRight[0]);
		mvpChanged = false;

		const qreal retinaScale = devicePixelRatio();
		gl->glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	}

	gl->glClearColor(.392, .584, 0.929, 1.0);

	gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shaderProgram->use();
	VAO->bind();

	renderParticles();

	//gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	printError();

	context->swapBuffers(this);
}

void ge::particle::OpenGLWindow::printError() const
{
	auto err = this->gl->glGetError();
	if (err != GL_NO_ERROR)
	{

		std::cout << "chyba " << err << std::endl;
	}
}

void ge::particle::OpenGLWindow::renderNow()
{
	if (!isExposed()) return;
	if (!initialized) initialize();

	context->makeCurrent(this);
	//context never got released so no need to make it current again

	render(); //do the simple rendering

	requestUpdate();

	//release context only if necessary
}

bool ge::particle::OpenGLWindow::event(QEvent *event)
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

void ge::particle::OpenGLWindow::mouseMoveEvent(QMouseEvent *event) {
	/*
    //qDebug() << event->pos();
    float posY = (float)event->pos().y() / (height()/2);
    float posX = (float)event->pos().x() / (width()/2);
    //qDebug() << posX;
    mpf->x = posX-1;
    mpf->y = 1-posY;
	*/
}

void ge::particle::OpenGLWindow::resizeEvent(QResizeEvent *event)
{
	QWindow::resizeEvent(event);
	//std::cout << width() << "x" << height() << std::endl;

	calculateMVP();
}

void ge::particle::OpenGLWindow::exposeEvent(QExposeEvent *event)
{
	if (isExposed())
	{
		renderNow();
	}
}

void ge::particle::OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space) {
		cameraPositionsIndex = (cameraPositionsIndex + 1) % cameraPositions.size();
		calculateMVP();
	}
}

#define MAX_PARTICLES 10000

int ge::particle::OpenGLWindow::updateBuffer() {
    int particleIndex = 0;

    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particle &p = ps->getParticleContainer()[i];

        if (p.livingFlag) {
            particlesPositions[3 * particleIndex + 0] = p.pos.x;
            particlesPositions[3 * particleIndex + 1] = p.pos.y;
            particlesPositions[3 * particleIndex + 2] = p.pos.z;

			particlesColors[3 * particleIndex + 0] = p.r;
			particlesColors[3 * particleIndex + 1] = p.g;
			particlesColors[3 * particleIndex + 2] = p.b;

            particleIndex++;
        }
    }
    return particleIndex;
}

void ge::particle::OpenGLWindow::calculateViewMatrix()
{
	viewMatrix = glm::lookAt(
		//glm::vec3(0, 0, -5), // Camera is at (4,3,3), in World Space
		cameraPositions[cameraPositionsIndex],
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

void ge::particle::OpenGLWindow::calculateCameraVectors()
{
	cameraRight = glm::vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
	cameraUp = glm::vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);
}

void ge::particle::OpenGLWindow::calculateMVP()
{
	calculateViewMatrix();

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)width() / (float)height(), 0.1f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
	glm::mat4 View = viewMatrix;
	glm::mat4 Model = glm::mat4(1.0f);

	calculateCameraVectors();

	mvp = Projection * View * Model;
	mvpChanged = true;
}

// particles
void ge::particle::OpenGLWindow::initializeParticles()
{
	ps = std::make_shared<ge::particle::ParticleSystem>(MAX_PARTICLES);
	//ps = new ParticleSystem(MAX_PARTICLES);
    auto *pe = new BasicParticleEmitter(ps);
    pe->setParticlesPerSecond(100);

    mpf = new MyParticleFactory();
    Particle ptemplate;
    ptemplate.life = core::time_unit(3.f);
    mpf->setParticleTemplate(ptemplate);

    pe->particleFactory.reset(mpf);
    ps->particleEmitter.reset(pe);

    ps->particleAffectors.push_back(std::make_unique<LifeTimeAffector>());
    //ps->particleAffectors.push_back(std::make_unique<GravityAffector>());
	ps->particleAffectors.push_back(std::make_unique<FloorAffector>());

	particleCenters = std::make_shared<ge::gl::Buffer>(MAX_PARTICLES * sizeof(float) * 3);
	VAO->addAttrib(particleCenters, 1, 3, GL_FLOAT);

	particleColors = std::make_shared<ge::gl::Buffer>(MAX_PARTICLES * sizeof(float) * 3);
	VAO->addAttrib(particleColors, 2, 3, GL_FLOAT);

	particlesPositions = new float[3*MAX_PARTICLES];
	particlesColors = new float[3 * MAX_PARTICLES];

	ps->setStartTime(ge::core::time_point::clock::now());
}

void ge::particle::OpenGLWindow::renderParticles()
{
    //mpf->x = (float)QWidget::map QCursor::pos().x() / (width() * devicePixelRatio());
    //mpf->y = (float)QCursor::pos().y() / (height() * devicePixelRatio());

	//ps->update(timer->elapsedFromLast());
	ps->update(core::time_point::clock::now());
	int livingParticlesCount = updateBuffer();

	// TODO: orphaning?? https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming
	particleCenters->invalidate(MAX_PARTICLES * sizeof(float) * 3, 0);
	particleCenters->setData(particlesPositions);

	particleColors->invalidate(MAX_PARTICLES * sizeof(float) * 3, 0);
	particleColors->setData(particlesColors);

	gl->glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	gl->glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	gl->glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1
	//gl->glDrawElements(GL_POINTS, MAX_PARTICLES, GL_UNSIGNED_INT, nullptr);
	gl->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, livingParticlesCount);
	//gl->glDrawArraysInstanced(GL_POINTS, 0, 1, MAX_PARTICLES);
}
