#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <algorithm>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int GRID_SIZE = 100;
const float CELL_SIZE = 5.0f;
const float TIME_STEP = 0.1f;
const float DIFFUSION = 0.0001f; // diffusion rate
const float VISCOSITY = 0.0001f; // viscosity of the fluid

struct Fluid {
    std::vector<float> s;  // source density
    std::vector<float> density; // current density
    std::vector<float> Vx; // velocity in x direction
    std::vector<float> Vy; // velocity in y direction
    std::vector<float> Vx0; // previous velocity in x direction
    std::vector<float> Vy0; // previous velocity in y direction

    Fluid() : s(GRID_SIZE* GRID_SIZE, 0.0f),
        density(GRID_SIZE* GRID_SIZE, 0.0f),
        Vx(GRID_SIZE* GRID_SIZE, 0.0f),
        Vy(GRID_SIZE* GRID_SIZE, 0.0f),
        Vx0(GRID_SIZE* GRID_SIZE, 0.0f),
        Vy0(GRID_SIZE* GRID_SIZE, 0.0f) {}
};

Fluid fluid;

int IX(int x, int y) {
    return x + y * GRID_SIZE;
}

void addSource(std::vector<float>& x, const std::vector<float>& s, float dt) {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        x[i] += dt * s[i];
    }
}

void setBounds(int b, std::vector<float>& x) {
    for (int i = 1; i < GRID_SIZE - 1; i++) {
        x[IX(0, i)] = b == 1 ? -x[IX(1, i)] : x[IX(1, i)];
        x[IX(GRID_SIZE - 1, i)] = b == 1 ? -x[IX(GRID_SIZE - 2, i)] : x[IX(GRID_SIZE - 2, i)];
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, GRID_SIZE - 1)] = b == 2 ? -x[IX(i, GRID_SIZE - 2)] : x[IX(i, GRID_SIZE - 2)];
    }
    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, GRID_SIZE - 1)] = 0.5f * (x[IX(1, GRID_SIZE - 1)] + x[IX(0, GRID_SIZE - 2)]);
    x[IX(GRID_SIZE - 1, 0)] = 0.5f * (x[IX(GRID_SIZE - 2, 0)] + x[IX(GRID_SIZE - 1, 1)]);
    x[IX(GRID_SIZE - 1, GRID_SIZE - 1)] = 0.5f * (x[IX(GRID_SIZE - 2, GRID_SIZE - 1)] + x[IX(GRID_SIZE - 1, GRID_SIZE - 2)]);
}

void linSolve(int b, std::vector<float>& x, const std::vector<float>& x0, float a, float c) {
    for (int k = 0; k < 20; k++) {
        for (int j = 1; j < GRID_SIZE - 1; j++) {
            for (int i = 1; i < GRID_SIZE - 1; i++) {
                x[IX(i, j)] = (x0[IX(i, j)] + a * (x[IX(i + 1, j)] + x[IX(i - 1, j)] + x[IX(i, j + 1)] + x[IX(i, j - 1)])) / c;
            }
        }
        setBounds(b, x);
    }
}

void diffuse(int b, std::vector<float>& x, const std::vector<float>& x0, float diff, float dt) {
    float a = dt * diff * (GRID_SIZE - 2) * (GRID_SIZE - 2);
    linSolve(b, x, x0, a, 1 + 4 * a);
}

void advect(int b, std::vector<float>& d, const std::vector<float>& d0, const std::vector<float>& velX, const std::vector<float>& velY, float dt) {
    float dt0 = dt * (GRID_SIZE - 2);
    for (int j = 1; j < GRID_SIZE - 1; j++) {
        for (int i = 1; i < GRID_SIZE - 1; i++) {
            float x = i - dt0 * velX[IX(i, j)];
            float y = j - dt0 * velY[IX(i, j)];
            if (x < 0.5f) x = 0.5f;
            if (x > GRID_SIZE - 2 + 0.5f) x = GRID_SIZE - 2 + 0.5f;
            int i0 = (int)x;
            int i1 = i0 + 1;
            if (y < 0.5f) y = 0.5f;
            if (y > GRID_SIZE - 2 + 0.5f) y = GRID_SIZE - 2 + 0.5f;
            int j0 = (int)y;
            int j1 = j0 + 1;
            float s1 = x - i0;
            float s0 = 1 - s1;
            float t1 = y - j0;
            float t0 = 1 - t1;
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) + s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }
    setBounds(b, d);
}

void project(std::vector<float>& velX, std::vector<float>& velY, std::vector<float>& p, std::vector<float>& div) {
    for (int j = 1; j < GRID_SIZE - 1; j++) {
        for (int i = 1; i < GRID_SIZE - 1; i++) {
            div[IX(i, j)] = -0.5f * (velX[IX(i + 1, j)] - velX[IX(i - 1, j)] + velY[IX(i, j + 1)] - velY[IX(i, j - 1)]) / GRID_SIZE;
            p[IX(i, j)] = 0;
        }
    }
    setBounds(0, div);
    setBounds(0, p);

    linSolve(0, p, div, 1, 4);

    for (int j = 1; j < GRID_SIZE - 1; j++) {
        for (int i = 1; i < GRID_SIZE - 1; i++) {
            velX[IX(i, j)] -= 0.5f * GRID_SIZE * (p[IX(i + 1, j)] - p[IX(i - 1, j)]);
            velY[IX(i, j)] -= 0.5f * GRID_SIZE * (p[IX(i, j + 1)] - p[IX(i, j - 1)]);
        }
    }
    setBounds(1, velX);
    setBounds(2, velY);
}

void step(Fluid& fluid, float dt) {
    addSource(fluid.Vx, fluid.Vx0, dt);
    addSource(fluid.Vy, fluid.Vy0, dt);

    std::swap(fluid.Vx0, fluid.Vx);
    diffuse(1, fluid.Vx, fluid.Vx0, VISCOSITY, dt);

    std::swap(fluid.Vy0, fluid.Vy);
    diffuse(2, fluid.Vy, fluid.Vy0, VISCOSITY, dt);

    project(fluid.Vx, fluid.Vy, fluid.Vx0, fluid.Vy0);

    std::swap(fluid.Vx0, fluid.Vx);
    std::swap(fluid.Vy0, fluid.Vy);
    advect(1, fluid.Vx, fluid.Vx0, fluid.Vx0, fluid.Vy0, dt);
    advect(2, fluid.Vy, fluid.Vy0, fluid.Vx0, fluid.Vy0, dt);

    project(fluid.Vx, fluid.Vy, fluid.Vx0, fluid.Vy0);

    addSource(fluid.density, fluid.s, dt);
    std::swap(fluid.s, fluid.density);
    diffuse(0, fluid.density, fluid.s, DIFFUSION, dt);
    std::swap(fluid.s, fluid.density);
    advect(0, fluid.density, fluid.s, fluid.Vx, fluid.Vy, dt);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    for (int j = 0; j < GRID_SIZE - 1; j++) {
        for (int i = 0; i < GRID_SIZE - 1; i++) {
            float x = i * CELL_SIZE;
            float y = j * CELL_SIZE;
            float d = fluid.density[IX(i, j)];
            glColor3f(d, d, d); // density as grayscale color
            glVertex2f(x, y);
            glVertex2f(x + CELL_SIZE, y);
            glVertex2f(x + CELL_SIZE, y + CELL_SIZE);
            glVertex2f(x, y + CELL_SIZE);
        }
    }
    glEnd();

    glutSwapBuffers();
}

void update(int value) {
    step(fluid, TIME_STEP);
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void addDensity(int x, int y, float amount) {
    fluid.s[IX(x, y)] += amount;
}

void addVelocity(int x, int y, float amountX, float amountY) {
    fluid.Vx[IX(x, y)] += amountX;
    fluid.Vy[IX(x, y)] += amountY;
}

void mouseMotion(int x, int y) {
    int i = x / CELL_SIZE;
    int j = (WINDOW_HEIGHT - y) / CELL_SIZE;
    if (i > 0 && i < GRID_SIZE - 1 && j > 0 && j < GRID_SIZE - 1) {
        addDensity(i, j, 100.0f);
        addVelocity(i, j, 10.0f, 10.0f);
    }
}

void initFluid() {
    // initial density and velocity for visualization
    for (int j = 20; j < 30; j++) {
        for (int i = 20; i < 30; i++) {
            fluid.density[IX(i, j)] = 1.0f;
            fluid.Vx[IX(i, j)] = 1.0f;
            fluid.Vy[IX(i, j)] = 1.0f;
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("2D Water Simulation");

    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

    glutDisplayFunc(display);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(16, update, 0);

    initFluid();

    glutMainLoop();

    return 0;
}
