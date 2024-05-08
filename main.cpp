#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <SDL.h>

typedef int32_t i32;
typedef uint32_t u32;
typedef int32_t b32;

#define WinWidth 1920
#define WinHeight 1080

void glShadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]);
void drawPyramid();

int main(int ArgCount, char **Args) {
    SDL_Init(SDL_INIT_EVERYTHING);
    u32 WindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_Window *Window = SDL_CreateWindow("OpenGL Test", 100, 100, WinWidth, WinHeight, WindowFlags);
    assert(Window);
    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    SDL_GLContext Context = SDL_GL_CreateContext(Window);
    assert(Context);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    b32 Running = 1;
    b32 FullScreen = 1;

    double cameraX = 8.0f, cameraY = 5.0f, cameraZ = 10.0f;
    double cameraSpeed = 0.1;
    double cameraRotationX = 1.0f, cameraRotationY = 0.0f;

    while (Running) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, (double)WinWidth / (double)WinHeight, 0.1, 100.0);
        glMatrixMode(GL_MODELVIEW);

        glLoadIdentity();
        glRotatef(cameraRotationY, 1.0f, 0.0f, 0.0f); // Rotate the camera up and down
        glRotatef(cameraRotationX, 0.0f, 1.0f, 0.0f); // Rotate the camera left and right
        gluLookAt(cameraX, cameraY, cameraZ, cameraX + sin(cameraRotationX * M_PI / 180.0f), cameraY, cameraZ - cos(cameraRotationX * M_PI / 180.0f), 0.0f, 1.0f, 0.0f);

        SDL_Event Event;
        while (SDL_PollEvent(&Event)) {
            if (Event.type == SDL_KEYDOWN) {
                switch (Event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        Running = 0;
                    break;
                    case 'w':
                        cameraX += cameraSpeed * sin(cameraRotationX * M_PI / 180.0f);
                    cameraY -= cameraSpeed * sin(cameraRotationY * M_PI / 180.0f);
                    cameraZ -= cameraSpeed * cos(cameraRotationX * M_PI / 180.0f);
                    break;
                    case 's':
                        cameraX -= cameraSpeed * sin(cameraRotationX * M_PI / 180.0f);
                    cameraY += cameraSpeed * sin(cameraRotationY * M_PI / 180.0f);
                    cameraZ += cameraSpeed * cos(cameraRotationX * M_PI / 180.0f);
                    break;
                    case 'a':
                        cameraX -= cameraSpeed * cos(cameraRotationX * M_PI / 180.0f);
                    cameraZ -= cameraSpeed * sin(cameraRotationX * M_PI / 180.0f);
                    break;
                    case 'd':
                        cameraX += cameraSpeed * cos(cameraRotationX * M_PI / 180.0f);
                    cameraZ += cameraSpeed * sin(cameraRotationX * M_PI / 180.0f);
                    break;
                    case 'f':
                        FullScreen = !FullScreen;
                    if (FullScreen) {
                        SDL_SetWindowFullscreen(Window, WindowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    } else {
                        SDL_SetWindowFullscreen(Window, WindowFlags);
                    }
                    break;
                    case 'h':
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Help", "f: toggle fullscreen\nh: show this help\nesc: quit", Window);
                    break;
                    default:
                        break;
                }
            } else if (Event.type == SDL_QUIT) {
                Running = 0;
            }  else if (Event.type == SDL_MOUSEMOTION) {
                // Handle mouse motion for camera rotation
                cameraRotationX += Event.motion.xrel * 0.1f;
                cameraRotationY += Event.motion.yrel * 0.1f;
            }
        }

        // Enable lighting
        glEnable(GL_LIGHTING);

        // Define a light source
        GLfloat lightPosition[] = {2.0f, 3.0f, 2.0f, 1.0f}; // Positional light
        GLfloat whiteLight[] = {1.0f, 1.0f, 1.0f, 1.0f}; // White light
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
        glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);

        // Enable the light source
        glEnable(GL_LIGHT0);

        // Compute the shadow matrix
        GLfloat groundPlane[4] = {0.0f, 1.0f, 0.0f, 0.0f}; // Plane equation for the ground (y = 0)
        GLfloat shadowMatrix[4][4];
        glShadowMatrix(shadowMatrix, groundPlane, lightPosition);



        drawPyramid();


        // Multiply the current matrix with the shadow matrix
        glPushMatrix();

        // Translate the shadow slightly along the Y-axis
        glTranslatef(0.0f, 0.0005f, 0.0f); // Adjust the value as needed
        glMultMatrixf((GLfloat *)shadowMatrix);


        // Draw the shadow of the pyramid
        // Disable lighting
        glDisable(GL_LIGHTING);
        glColor3f(0.0f, 0.0f, 0.0f); // Shadow color
        drawPyramid();

        // Enable lighting
        glEnable(GL_LIGHTING);

        glPopMatrix();

        // Draw the room
        glBegin(GL_QUADS);

        // Floor
        GLfloat floorColor[] = {0.5f, 0.5f, 0.5f, 1.0f}; // Gray
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floorColor);
        glVertex3f(-10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, 10.0f);
        glVertex3f(-10.0f, 0.0f, 10.0f);

        // Ceiling
        GLfloat ceilingColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // White
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ceilingColor);
        glVertex3f(-10.0f, 10.0f, -10.0f);
        glVertex3f(10.0f, 10.0f, -10.0f);
        glVertex3f(10.0f, 10.0f, 10.0f);
        glVertex3f(-10.0f, 10.0f, 10.0f);

        // Front wall
        GLfloat frontWallColor[] = {1.0f, 0.0f, 0.0f, 1.0f}; // Red
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, frontWallColor);
        glVertex3f(-10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 10.0f, -10.0f);
        glVertex3f(-10.0f, 10.0f, -10.0f);

        // Back wall
        GLfloat backWallColor[] = {0.0f, 1.0f, 0.0f, 1.0f}; // Green
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, backWallColor);
        glVertex3f(-10.0f, 0.0f, 10.0f);
        glVertex3f(10.0f, 0.0f, 10.0f);
        glVertex3f(10.0f, 10.0f, 10.0f);
        glVertex3f(-10.0f, 10.0f, 10.0f);

        // Left wall
        GLfloat leftWallColor[] = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, leftWallColor);
        glVertex3f(-10.0f, 0.0f, -10.0f);
        glVertex3f(-10.0f, 0.0f, 10.0f);
        glVertex3f(-10.0f, 10.0f, 10.0f);
        glVertex3f(-10.0f, 10.0f, -10.0f);

        // Right wall
        GLfloat rightWallColor[] = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, rightWallColor);
        glVertex3f(10.0f, 0.0f, -10.0f);
        glVertex3f(10.0f, 0.0f, 10.0f);
        glVertex3f(10.0f, 10.0f, 10.0f);
        glVertex3f(10.0f, 10.0f, -10.0f);

        glEnd();







        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << ("OpenGL error: %d\n", error) << std::endl;
        }

        SDL_GL_SwapWindow(Window);
    }

    return 0;
}

void drawPyramid() {
    glBegin(GL_TRIANGLES); // Begin drawing triangles for the pyramid sides

    // Front
    GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f}; // Red
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    // Right
    GLfloat green[] = {0.0f, 1.0f, 0.0f, 1.0f}; // Green
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    // Back
    GLfloat blue[] = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    // Left
    GLfloat yellow[] = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
    glVertex3f(0.0f, 2.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    glEnd(); // End of drawing the pyramid sides

    glBegin(GL_QUADS); // Begin drawing quads for the pyramid base

    // Base
    GLfloat orange[] = {1.0f, 0.5f, 0.0f, 1.0f}; // Orange
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, orange);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    glEnd(); // End of drawing the pyramid base
}


void glShadowMatrix(GLfloat shadowMat[4][4], GLfloat groundplane[4], GLfloat lightpos[4]) {
    GLfloat dot = groundplane[0] * lightpos[0] +
                  groundplane[1] * lightpos[1] +
                  groundplane[2] * lightpos[2] +
                  groundplane[3] * lightpos[3];

    shadowMat[0][0] = dot - lightpos[0] * groundplane[0];
    shadowMat[1][0] = 0.f - lightpos[0] * groundplane[1];
    shadowMat[2][0] = 0.f - lightpos[0] * groundplane[2];
    shadowMat[3][0] = 0.f - lightpos[0] * groundplane[3];

    shadowMat[0][1] = 0.f - lightpos[1] * groundplane[0];
    shadowMat[1][1] = dot - lightpos[1] * groundplane[1];
    shadowMat[2][1] = 0.f - lightpos[1] * groundplane[2];
    shadowMat[3][1] = 0.f - lightpos[1] * groundplane[3];

    shadowMat[0][2] = 0.f - lightpos[2] * groundplane[0];
    shadowMat[1][2] = 0.f - lightpos[2] * groundplane[1];
    shadowMat[2][2] = dot - lightpos[2] * groundplane[2];
    shadowMat[3][2] = 0.f - lightpos[2] * groundplane[3];

    shadowMat[0][3] = 0.f - lightpos[3] * groundplane[0];
    shadowMat[1][3] = 0.f - lightpos[3] * groundplane[1];
    shadowMat[2][3] = 0.f - lightpos[3] * groundplane[2];
    shadowMat[3][3] = dot - lightpos[3] * groundplane[3];
}
