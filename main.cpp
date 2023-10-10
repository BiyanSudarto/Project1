    #include <GL/glut.h>
    #include <cmath>

    int windowWidth = 800;
    int windowHeight = 600;
    int lastMouseX, lastMouseY;
    float rotationX = 45.0f;
    float rotationY = 45.0f;
    bool isDragging = false;

    void drawDice() {
        // Gambar dadu berwarna putih
        glColor3f(1.0f, 1.0f, 1.0f); // Warna putih (RGB: 1, 1, 1)
        glutSolidCube(1.0);

        // Gambar garis-garis hitam pada rusuk-rusuk dadu
        glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam (RGB: 0, 0, 0)
        glutWireCube(1.0);

        // Gambar lingkaran hitam pada tiap sisi dadu
        glColor3f(0.0f, 0.0f, 0.0f); // Warna hitam (RGB: 0, 0, 0)
        float circleRadius = 0.1f;
        int numSegments = 100;

        float posisiLingkaran[6][6][3] = {
            {
                {0.0f, 0.51f, 0.0f}
            },
            {
                {0.51f, 0.2f, 0.0f},
                {0.51f, -0.2f, 0.0f}
            },
            {
                {-0.51f, 0.25f, 0.2f},
                {-0.51f, 0.0f, 0.0f},
                {-0.51f, -0.25f, -0.2f}
            },
            {
                {0.2f, 0.2f, -0.51f},
                {-0.2f, 0.2f, -0.51f},
                {0.2f, -0.2f, -0.51f},
                {-0.2f, -0.2f, -0.51f}
            },
            {
                {0.2f, 0.2f, 0.51f},
                {-0.2f, 0.2f, 0.51f},
                {0.2f, -0.2f, 0.51f},
                {-0.2f, -0.2f, 0.51f},
                {0.0f, 0.0f, 0.51f}
            },
            {
                {0.2f, -0.51f, 0.25f},
                {0.2f, -0.51f, 0.0f},
                {0.2f, -0.51f, -0.25f},
                {-0.2f, -0.51f, 0.25f},
                {-0.2f, -0.51f, 0.0f},
                {-0.2f, -0.51f, -0.25f}
            }
        };

        // Sisi 1 (atas)
        for(int i = 0; i < 6; i++) {
            for(int j = 0; j <= i; j++) {
                float* posisi = posisiLingkaran[i][j];
                
                glPushMatrix();
                glTranslatef(posisi[0], posisi[1], posisi[2]);
                glBegin(GL_TRIANGLE_FAN);
                glVertex3f(0.0f, 0.0f, 0.0f); // Titik tengah
                for (int j = 0; j <= numSegments; j++) {
                    float theta = 2.0f * M_PI * float(j) / float(numSegments);
                    float x = circleRadius * cos(theta);
                    float y = circleRadius * sin(theta);
                    switch (i) {
                        case 0:
                        case 5:
                        case 6:
                            glVertex3f(x, 0.0f, y);
                            break;
                        
                        case 1:
                        case 2:
                            glVertex3f(0.0f, x, y);
                            break;

                        case 3:
                        case 4:
                            glVertex3f(x, y, 0.0f);
                            break;
                    }
                }
                glEnd();
                glPopMatrix();
            }
        }
    }

    void display() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        glTranslatef(0.0f, 0.0f, -5.0f);
        glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Rotasi terhadap sumbu X
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Rotasi terhadap sumbu Y
        drawDice();

        glutSwapBuffers();
    }

    void reshape(int w, int h) {
        windowWidth = w;
        windowHeight = h;
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 1.0f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            isDragging = true;
            lastMouseX = x;
            lastMouseY = y;
        } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            isDragging = false;
        }
    }

    void mouseMotion(int x, int y) {
        if (isDragging) {
            rotationX += (y - lastMouseY) * 0.5f;
            rotationY += (x - lastMouseX) * 0.5f;
            lastMouseX = x;
            lastMouseY = y;
            glutPostRedisplay();
        }
    }

    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(windowWidth, windowHeight);
        glutCreateWindow("Dadu");

        glEnable(GL_DEPTH_TEST);

        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutMouseFunc(mouse);
        glutMotionFunc(mouseMotion);

        glutMainLoop();

        return 0;
    }
