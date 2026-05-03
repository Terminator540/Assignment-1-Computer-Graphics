#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <cmath>

// ===============================
//  3D JET ANIMATION
// Massive jet, real shape, wings, tail,
// rotating engines, afterburner,
// clouds, sky, takeoff movement
// ===============================

float jetX = -18.0f;
float jetY = -3.0f;
float engineRotate = 0.0f;
float cloudMove = 0.0f;

// -------------------------------
// Draw text helper
// -------------------------------
void drawText(float x, float y, const char* text) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-10, 10, -7, 7);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);

    while (*text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
}

// -------------------------------
// Clouds
// -------------------------------
void drawCloud(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glColor3f(1, 1, 1);

    glutSolidSphere(0.8, 20, 20);

    glTranslatef(0.8, 0.2, 0);
    glutSolidSphere(0.7, 20, 20);

    glTranslatef(-1.6, 0.0, 0);
    glutSolidSphere(0.7, 20, 20);

    glPopMatrix();
}

// -------------------------------
// Jet body
// -------------------------------
void drawJet() {
    glPushMatrix();

    glTranslatef(jetX, jetY, -25.0f);

    // Slight takeoff angle
    glRotatef(-10, 0, 0, 1);

    // ============================
    // Nose cone (Blue)
    // ============================
    glColor3f(0.0f, 0.2f, 0.8f);
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glutSolidCone(1.2, 4.0, 30, 30);
    glPopMatrix();

    // ============================
    // Main fuselage (White)
    // ============================
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(-4.0f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    GLUquadric* body = gluNewQuadric();
    gluCylinder(body, 1.2, 1.0, 8.0, 30, 30);
    glPopMatrix();

    // ============================
    // Rear fuselage (Red)
    // ============================
    glColor3f(0.8f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-12.0f, 0.0f, 0.0f);
    glRotatef(90, 0, 1, 0);
    gluCylinder(body, 1.0, 0.6, 3.0, 30, 30);
    glPopMatrix();

    // ============================
    // Main Wings
    // ============================
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_TRIANGLES);

    // Left wing
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f(-9.0f, -0.2f, 8.0f);
    glVertex3f(-2.0f, 0.0f, 1.5f);

    // Right wing
    glVertex3f(-5.0f, 0.0f, 0.0f);
    glVertex3f(-9.0f, -0.2f, -8.0f);
    glVertex3f(-2.0f, 0.0f, -1.5f);

    glEnd();

    // ============================
    // Tail wings
    // ============================
    glBegin(GL_TRIANGLES);

    // Left tail wing
    glVertex3f(-12.0f, 0.5f, 0.0f);
    glVertex3f(-14.0f, 0.0f, 4.0f);
    glVertex3f(-11.0f, 0.2f, 0.5f);

    // Right tail wing
    glVertex3f(-12.0f, 0.5f, 0.0f);
    glVertex3f(-14.0f, 0.0f, -4.0f);
    glVertex3f(-11.0f, 0.2f, -0.5f);

    glEnd();

    // Vertical tail fin
    glBegin(GL_TRIANGLES);
    glVertex3f(-12.5f, 1.0f, 0.0f);
    glVertex3f(-15.0f, 4.0f, 0.0f);
    glVertex3f(-13.0f, 0.5f, 0.0f);
    glEnd();

    // ============================
    // Rotating Engines
    // ============================
    for (int i = -1; i <= 1; i += 2) {
        glPushMatrix();
        glTranslatef(-6.0f, -1.0f, i * 3.0f);

        glColor3f(0.2f, 0.2f, 0.2f);
        glRotatef(90, 0, 1, 0);
        gluCylinder(body, 0.7, 0.7, 3.0, 20, 20);

        // Fan blades
        glPushMatrix();
        glTranslatef(0, 0, 0.2f);
        glRotatef(engineRotate, 1, 0, 0);

        glBegin(GL_LINES);
        glColor3f(0, 0, 0);
        for (int a = 0; a < 360; a += 45) {
            float rad = a * 3.14159f / 180.0f;
            glVertex3f(0, 0, 0);
            glVertex3f(cos(rad) * 0.6f, sin(rad) * 0.6f, 0);
        }
        glEnd();
        glPopMatrix();

        glPopMatrix();
    }

    // ============================
    // Afterburner flames
    // ============================
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.5f, 0.0f);

    glVertex3f(-15.0f, 0.0f, 0.5f);
    glVertex3f(-18.0f, 0.3f, 0.0f);
    glVertex3f(-15.0f, 0.0f, -0.5f);

    glEnd();

    gluDeleteQuadric(body);

    glPopMatrix();
}

// -------------------------------
// Display
// -------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(0, 2, 8, 0, 0, -20, 0, 1, 0);

    // Clouds
    drawCloud(-8 + cloudMove, 5, -30);
    drawCloud(2 + cloudMove, 4, -28);
    drawCloud(10 + cloudMove, 6, -35);

    // Jet
    drawJet();

    // Banner
    drawText(-4.5f, 6.0f, "");

    glutSwapBuffers();
}

// -------------------------------
// Update animation
// -------------------------------
void update(int value) {
    jetX += 0.15f;

    // Takeoff rise
    if (jetY < 2.5f)
        jetY += 0.015f;

    if (jetX > 22.0f) {
        jetX = -18.0f;
        jetY = -3.0f;
    }

    engineRotate += 20.0f;
    cloudMove -= 0.03f;

    if (cloudMove < -20)
        cloudMove = 20;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// -------------------------------
// Init
// -------------------------------
void init() {
    glEnable(GL_DEPTH_TEST);

    // Sky blue
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.4, 1, 100);

    glMatrixMode(GL_MODELVIEW);
}

// -------------------------------
// Main
// -------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1200, 800);
    glutCreateWindow("Enhanced 3D French Jet Animation");

    init();

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
