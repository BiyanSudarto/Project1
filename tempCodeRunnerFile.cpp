#include <GL/glut.h>
#include <cmath>
#include <cstdlib>

int windowWidth = 600;
int windowHeight = 600;
int lastMouseX, lastMouseY;
float rotationX = 45.0f;
float rotationY = 45.0f;
bool isDragging = false;
float dicePositionX = 0.0f;
float dicePositionY = 0.0f;
float diceVelocityX;
float diceVelocityY;
float diceBounds = 0.3f;
float autoRotationSpeed = 0.05f;  // Kecepatan rotasi otomatis

void drawDice();
void display();
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotion(int x, int y);

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Dadu");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    
    GLfloat light_position[] = {0.0f, 1.0f, -2.0f, 1.0f};  // Sumber cahaya di pojok kanan atas
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Menetapkan nilai awal untuk kecepatan pergerakan diagonal dadu
    diceVelocityX = 0.002f * (rand() % 100) / 100.0f;
    diceVelocityY = 0.002f * (rand() % 100) / 100.0f;

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutIdleFunc(display);

    glutMainLoop();

    return 0;
}

void drawDice() {
    // Gambar dadu berwarna merah solid pada sisi kubus
    glColor3f(1.0f, 0.0f, 0.0f);  // Warna merah solid
    glutSolidCube(1.0);

    // Gambar garis-garis hitam pada rusuk-rusuk dadu
    glColor3f(0.0f, 0.0f, 0.0f);  // Warna hitam (RGB: 0, 0, 0)
    glutWireCube(1.0);

    // Gambar lingkaran hitam pada tiap sisi dadu
    glColor3f(0.0f, 0.0f, 0.0f);  // Warna hitam (RGB: 0, 0, 0)
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
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j <= i; j++) {
            float *posisi = posisiLingkaran[i][j];

            glPushMatrix();
            glTranslatef(posisi[0], posisi[1], posisi[2]);
            glBegin(GL_TRIANGLE_FAN);
            glVertex3f(0.0f, 0.0f, 0.0f);  // Titik tengah
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

    // Menambahkan translasi ke sumbu X dan Y
    glTranslatef(dicePositionX, dicePositionY, -5.0f);

    // Menambahkan rotasi otomatis pada sumbu tertentu
    rotationX += autoRotationSpeed;
    rotationY += autoRotationSpeed;

    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    drawDice();

    // Memperbarui posisi dadu dengan nilai acak untuk pergerakan diagonal dan tidak beraturan
    dicePositionX += diceVelocityX;
    dicePositionY += diceVelocityY;

    // Memantulkan dadu jika mencapai batas pada sumbu X
    if (dicePositionX > diceBounds || dicePositionX < -diceBounds) {
        diceVelocityX *= -1.0f;
    }

    // Memantulkan dadu jika mencapai batas pada sumbu Y
    if (dicePositionY > diceBounds || dicePositionY < -diceBounds) {
        diceVelocityY *= -1.0f;
    }

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
