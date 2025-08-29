#include <GL/glut.h>
#include <iostream>

bool usePerspective = true;
float angle = 0.0f;

void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (usePerspective) {
        gluPerspective(60.0, aspect, 1.0, 100.0);
    } else {
        if (w >= h)
            glOrtho(-10.0 * aspect, 10.0 * aspect, -10.0, 10.0, -100.0, 100.0);
        else
            glOrtho(-10.0, 10.0, -10.0 / aspect, 10.0 / aspect, -100.0, 100.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawScene() {
    // Draw multiple cubes at different distances
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(i * 3.0f - 6.0f, 0.0f, -5.0f * i - 5.0f); // deeper into screen
        glRotatef(angle, 1, 1, 0);
        glutSolidCube(2.0);
        glPopMatrix();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(8.0, 8.0, 20.0,   // Camera pos
              0.0, 0.0, -20.0,  // Look center
              0.0, 1.0, 0.0);   // Up

    // Draw cubes
    glColor3f(0.0, 0.7, 1.0);
    drawScene();

    // ---- Draw text ----
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0, 1.0, 1.0);
    if (usePerspective)
        renderBitmapString(10, 10, GLUT_BITMAP_HELVETICA_18, "Projection Mode: Perspective (far objects appear smaller)");
    else
        renderBitmapString(10, 10, GLUT_BITMAP_HELVETICA_18, "Projection Mode: Orthographic (size does not change with distance)");

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glutSwapBuffers();
}

void timer(int) {
    angle += 1.0f;
    if (angle > 360.0f) angle -= 360.0f;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void keyboard(unsigned char key, int, int) {
    if (key == 'p' || key == 'P') {
        usePerspective = true;
        std::cout << "Switched to Perspective Projection\n";
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
    if (key == 'o' || key == 'O') {
        usePerspective = false;
        std::cout << "Switched to Orthographic Projection\n";
        reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    }
    if (key == 27) exit(0);
}

int main(int argc, char** argv) {
    std::cout << "Controls:\n";
    std::cout << " Press 'P' → Switch to Perspective Projection\n";
    std::cout << " Press 'O' → Switch to Orthographic Projection\n";
    std::cout << " Press 'ESC' → Exit\n";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Projection Demo - Orthographic vs Perspective");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    initGL();
    glutMainLoop();
    return 0;
}
