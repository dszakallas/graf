//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2013-tol.          
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk. 
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat. 
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization f?ggv?nyt kiv?ve, a lefoglalt adat korrekt felszabad?t?sa n?lk?l 
// - felesleges programsorokat a beadott programban hagyni
// - tovabbi kommenteket a beadott programba irni a forrasmegjelolest kommentjeit kiveve
// ---------------------------------------------------------------------------------------------
// A feladatot ANSI C++ nyelvu forditoprogrammal ellenorizzuk, a Visual Studio-hoz kepesti elteresekrol
// es a leggyakoribb hibakrol (pl. ideiglenes objektumot nem lehet referencia tipusnak ertekul adni)
// a hazibeado portal ad egy osszefoglalot.
// ---------------------------------------------------------------------------------------------
// A feladatmegoldasokban csak olyan gl/glu/glut fuggvenyek hasznalhatok, amelyek
// 1. Az oran a feladatkiadasig elhangzottak ES (logikai AND muvelet)
// 2. Az alabbi listaban szerepelnek:  
// Rendering pass: glBegin, glVertex[2|3]f, glColor3f, glNormal3f, glTexCoord2f, glEnd, glDrawPixels
// Transzformaciok: glViewport, glMatrixMode, glLoadIdentity, glMultMatrixf, gluOrtho2D, 
// glTranslatef, glRotatef, glScalef, gluLookAt, gluPerspective, glPushMatrix, glPopMatrix,
// Illuminacio: glMaterialfv, glMaterialfv, glMaterialf, glLightfv
// Texturazas: glGenTextures, glBindTexture, glTexParameteri, glTexImage2D, glTexEnvi, 
// Pipeline vezerles: glShadeModel, glEnable/Disable a kovetkezokre:
// GL_LIGHTING, GL_NORMALIZE, GL_DEPTH_TEST, GL_CULL_FACE, GL_TEXTURE_2D, GL_BLEND, GL_LIGHT[0..7]
//
// NYILATKOZAT
// ---------------------------------------------------------------------------------------------
// Nev    : SZAKALLAS DAVID
// Neptun : ******
// ---------------------------------------------------------------------------------------------
// ezennel kijelentem, hogy a feladatot magam keszitettem, es ha barmilyen segitseget igenybe vettem vagy 
// mas szellemi termeket felhasznaltam, akkor a forrast es az atvett reszt kommentekben egyertelmuen jeloltem. 
// A forrasmegjeloles kotelme vonatkozik az eloadas foliakat es a targy oktatoi, illetve a 
// grafhazi doktor tanacsait kiveve barmilyen csatornan (szoban, irasban, Interneten, stb.) erkezo minden egyeb 
// informaciora (keplet, program, algoritmus, stb.). Kijelentem, hogy a forrasmegjelolessel atvett reszeket is ertem, 
// azok helyessegere matematikai bizonyitast tudok adni. Tisztaban vagyok azzal, hogy az atvett reszek nem szamitanak
// a sajat kontribucioba, igy a feladat elfogadasarol a tobbi resz mennyisege es minosege alapjan szuletik dontes.  
// Tudomasul veszem, hogy a forrasmegjeloles kotelmenek megsertese eseten a hazifeladatra adhato pontokat 
// negativ elojellel szamoljak el es ezzel parhuzamosan eljaras is indul velem szemben.
//=============================================================================================
 
#include <math.h>
#include <stdlib.h>
 
#if defined(__APPLE__)                                                                                                                                                                                                            
#include <OpenGL/gl.h>                                                                                                                                                                                                            
#include <OpenGL/glu.h>                                                                                                                                                                                                           
#include <GLUT/glut.h>                                                                                                                                                                                                            
#else                                                                                                                                                                                                                             
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)                                                                                                                                                                       
#include <windows.h>                                                                                                                                                                                                              
#endif                                                                                                                                                                                                                            
#include <GL/gl.h>                                                                                                                                                                                                                
#include <GL/glu.h>                                                                                                                                                                                                               
#include <GL/glut.h>                                                                                                                                                                                                              
#endif          
 
 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Innentol modosithatod...
 
 
struct Color {
   float r, g, b;
 
   Color( ) { 
    r = g = b = 0;
   }
   Color(float r0, float g0, float b0) { 
    r = r0; g = g0; b = b0;
   }
   Color operator*(float a) const {
    return Color(r * a, g * a, b * a); 
   }
   Color operator*(const Color& c) const {
    return Color(r * c.r, g * c.g, b * c.b); 
   }
   Color operator+(const Color& c) const {
     return Color(r + c.r, g + c.g, b + c.b); 
   }
};
 
struct float4{
    float a, b, c, d;
    float4() : a(), b(), c(), d() {}
    float4(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) { }
    float4 operator/(float t) const {
       return float4(a / t, b / t, c / t, d / t );
    }
    float4 operator*(float t) const {
        return float4(a * t, b * t, c * t, d * t);
    }
    float4 operator+(const float4& v) const {
         return float4(a + v.a, b + v.b, c + v.c, d + v.d);
    }
    float4 operator-(const float4& v) const {
         return float4(a - v.a, b - v.b, c - v.c, d - v.d);
    }
    float operator[](int i) const {
        if(i == 0) return a;
        if(i == 1) return b;
        if(i == 2) return c;
        if(i == 3) return d;
        else throw "InvalidArgumentException";
    }
};
 
class Rect {
public:
    float left;
    float width;
    float bottom;
    float height;
    Rect(float left, float width, float bottom, float height) :
        left(left), width(width), bottom(bottom), height(height)
    { }
    float top() const {
        return bottom + height;
    }
    float right() const {
        return left + width;
    }
};
 
class Camera {
public:
    Rect viewport;
    Rect window;
    Rect boundingWindow;
    Camera() : viewport(0, 600, 0, 600), window(0, 50, 0, 50), boundingWindow(0, 100, 0, 100) { }
    void setGL() {
        glViewport(    viewport.left, viewport.bottom,
                viewport.width, viewport.height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(    window.left, window.right(),
                    window.bottom, window.top());
    }
    float4 screenToWorld(int x, int y) {
        float sx = window.width / viewport.width;
        float sy = window.height / viewport.height;
 
        return float4(     (x - viewport.left) * sx + window.left,
                        (y - viewport.bottom) * sy + window.bottom,
                        0, 0);
    }
};
 
class ModelTransform {
public:
    float trans_x;
    float trans_y;
    float scale_x;
    float scale_y;
    float phi;
    ModelTransform() :
        trans_x(0),
        trans_y(0),
        scale_x(1),
        scale_y(1),
        phi(0)
    { }
    void setGL() const {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(trans_x, trans_y, 0);
        glRotatef(phi, 0, 0, 1);
        glScalef(scale_x, scale_y, 1);
    }
};
 
class TCR {
public:
    const float tension;
    int count;
    float t[10];
    float4 p[10];
    float4 v[10];
    ModelTransform transform;
    TCR() :
        tension(-0.5f),
        count(0),
        t(),
        p(),
        v(),
        transform()
    { }
    int index(float t) const {
        for(int i = 0; i < count; i++) {
            if(this->t[i] > t) return i - 1;
        }
        return 0;
    }
    void addControlPoint(const float4& point, float param) {
        if(count == 10) return;
 
        p[count] = point;
        t[count] = param;
        count++;
        if(count > 2) {
            v[count - 2] = velocity(count - 2);
        }
 
        return;
    }
    float4 velocity(int i) const {
        if(i < 0 || i >= count - 1)
            return float4();
        const float4& prev_p = p[i - 1];
        const float4& this_p = p[i];
        const float4& next_p = p[i + 1];
 
        const float& prev_t = t[i - 1];
        const float& this_t = t[i];
        const float& next_t = t[i + 1];
 
        const float4 prev2this(
                (this_p - prev_p) / (this_t - prev_t)
                );
        const float4 this2next(
                        (next_p - this_p) / (next_t - this_t)
                        );
        return (prev2this + this2next) * ((1.0f - tension) / 2.0f);
    }
 
    float4 weight(float param) {
        if(count == 0)
            throw "InvalidOperationException";
        if (count == 1)
            return float4(0,1,0,0);
        if(param <= t[0])
            return float4(0,1,0,0);
        int last_index = count - 1;
        if(param >= t[last_index])
            return float4(0,0,1,0);
 
        int this_i = index(param);
        int prev_i = this_i - 1;
        int next_i = this_i + 1;
        int nn_i = this_i + 2;
 
        const float& this_t = t[this_i];
        const float& next_t = t[next_i];
 
        const float4 prev_p(1,0,0,0);
        const float4 this_p(0,1,0,0);
        const float4 next_p(0,0,1,0);
        const float4 nn_p(0,0,0,1);
 
        const float4 prev2this(
                prev_i < 0 ?
                        float4() :
                        (this_p - prev_p) / (this_t - t[prev_i])
                );
        const float4 this2next(
                        (next_p - this_p) / (next_t - this_t)
                        );
 
        const float4 next2nn(
                    nn_i > count - 1 ?
                            float4() :
                            (nn_p - next_p) / (t[nn_i] - next_t)
                );
 
        const float4 this_v =
                prev_i < 0 ?
                        float4() :
                        float4(prev2this + this2next) * ((1.0f - tension) / 2.0f);
        const float4 next_v =
                nn_i > count - 1 ?
                        float4() :
                        float4(this2next + next2nn) * ((1.0f - tension) / 2.0f);
 
        const float4& a0 = this_p;
        const float4& a1 = this_v;
 
        const float tau = next_t - this_t;
        const float4 eps = next_p - this_p;
 
        const float4 a2 = (eps * 3.0f) / (tau * tau) - (next_v + this_v * 2.0f) / tau;
        const float4 a3 = (eps * -2.0f) / (tau * tau * tau) + (next_v + this_v) / (tau * tau);
 
        const float dt = param - this_t;
 
        return float4(
            a0 +
            a1 * dt +
            a2 * dt * dt +
            a3 * dt * dt * dt
            );
 
    }
 
    void drawGL(int animStart) {
        if (count == 0)
            return;
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        const int resolution = 500;
        const float t_start = t[0];
        const float t_end = t[count - 1];
        for(int i = 0; i < resolution; i++){
            float t = (t_end - t_start) * ((float) i / resolution) + t_start;
            float4 p(this->operator()(t));
            glVertex2f(p.a, p.b);
        }
        glEnd();
        if(animStart == 0) {
            glColor3f(1,0,0);
            for(int i = 0; i < count; i++){
                glBegin(GL_POLYGON);
                for(float t = 0; t < 1; t+=0.05f) {
                    float x = p[i].a + cosf(2 * t * M_PI);
                    float y = p[i].b + sinf(2 * t * M_PI);
                    glVertex2f(x,y);
                }
                glEnd();
            }
        }
        else {
            float dt = (t[count - 1] - t[0]);
            float ct = (float)(glutGet(GLUT_ELAPSED_TIME) - animStart) / 1000.0f;
            float t_n = (ct / dt) - floorf(ct / dt);
            float tt = t[0] + t_n * dt;
 
            float4 w = weight(tt);
            int i = index(tt);
            int indices[4] = { i - 1, i, i + 1, i + 2 };
 
            for(int j = 0; j < 4; j++){
                if(indices[j] >= 0 || indices[j] < count) {
                    int index = indices[j];
                    if (w[j] < 0) glColor3f(0,1,1);
                    else glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = p[index].a + cosf(2 * t * M_PI) * w[j];
                        float y = p[index].b + sinf(2 * t * M_PI) * w[j];
                        glVertex2f(x,y);
                    }
                    glEnd();
                }
            }
 
            float4 r(this->operator()(tt));
            glColor3f(1,1,0);
            glBegin(GL_POLYGON);
            for(float t = 0; t < 1; t+=0.05f) {
                float x = r.a + cosf(2 * t * M_PI);
                float y = r.b + sinf(2 * t * M_PI);
                glVertex2f(x,y);
            }
            glEnd();
        }
    }
    float4 operator()(float param) {
        if(count == 0)
            throw "InvalidOperationException";
        if (count == 1)
            return p[0];
        if(param <= t[0])
            return p[0];
        int last_index = count - 1;
        if(param >= t[last_index])
            return p[last_index];
 
        int this_i = index(param);
        int next_i = this_i + 1;
 
        const float& this_t = t[this_i];
        const float& next_t = t[next_i];
 
        const float4& this_p = p[this_i];
        const float4& next_p = p[next_i];
 
        const float4& this_v = v[this_i];
        const float4& next_v = v[next_i];
 
        const float4& a0 = this_p;
        const float4& a1 = this_v;
 
        const float tau = next_t - this_t;
        const float4 eps = next_p - this_p;
 
        const float4 a2 = (eps * 3.0f) / (tau * tau) - (next_v + this_v * 2.0f) / tau;
        const float4 a3 = (eps * -2.0f) / (tau * tau * tau) + (next_v + this_v) / (tau * tau);
 
        const float dt = param - this_t;
 
        return float4(
                a0 +
                a1 * dt +
                a2 * dt * dt +
                a3 * dt * dt * dt
                );
    }
};
 
class Bezier {
public:
    int count;
    float4 p[10];
    ModelTransform transform;
    Bezier() :
        count(),
        p(),
        transform()
    { }
    float weight(int i, float t) const {
            float choose = 1;
 
            for(int j = 1; j <= i; j++)
                choose *= (float)(count - j) / j;
 
            return choose * pow(t, i) * pow(1 - t, count - 1 - i);
        }
    float4 operator()(float t) const {
        if(count == 0) throw "InvalidOperationException";
        if(t < 0 || t > 1)
            return p[0];
        else{
            float4 result;
            for(int i = 0; i < count; i++)
                result = result + p[i] * weight(i, t);
            return result;
        }
    }
    void drawGL(float dt, int startTime) {
        if (count == 0)
            return;
        glColor3f(0, 0, 1);
        glBegin(GL_LINE_STRIP);
        const int resolution = 500;
        for(int i = 0; i < resolution; i++){
            float t = (float) i / resolution;
            float4 p(this->operator()(t));
            glVertex2f(p.a, p.b);
        }
        glEnd();
 
        float ct = (float)(glutGet(GLUT_ELAPSED_TIME) - startTime) / 1000.0f;
        float t_n = (ct / dt) - floorf(ct / dt);
 
        glColor3f(1,0,0);
 
        for(int i = 0; i < count; i++){
            float w = weight(i, t_n);
            glBegin(GL_POLYGON);
            for(float t = 0; t < 1; t+=0.05f) {
                float x = p[i].a + cosf(2 * t * M_PI) * w;
                float y = p[i].b + sinf(2 * t * M_PI) * w;
                glVertex2f(x,y);
            }
            glEnd();
        }
 
 
 
        glColor3f(1,1,0);
 
        float4 r(this->operator()(t_n));
        glColor3f(1,1,0);
        glBegin(GL_POLYGON);
        for(float t = 0; t < 1; t+=0.05f) {
            float x = r.a + cosf(2 * t * M_PI);
            float y = r.b + sinf(2 * t * M_PI);
            glVertex2f(x,y);
        }
        glEnd();
    }
};
 
class Scene{
public:
    enum State {
        EDIT,
        ANIMATION,
    };
    State state;
    int animStart;
    Camera camera;
    TCR tcr;
    Bezier bezier;
    void renderGL() {
        camera.setGL();
        if(tcr.count != 0) {
            tcr.transform.setGL();
            tcr.drawGL(animStart);
        }
 
        if(bezier.count != 0) {
            bezier.transform.setGL();
            bezier.drawGL(tcr.t[tcr.count - 1] -tcr.t[0], animStart);
        }
 
    }
    void onLeftButton(int scr_x, int scr_y) {
        if(state == Scene::EDIT) {
            float4 mousePosition(camera.screenToWorld(scr_x, scr_y));
            tcr.addControlPoint(mousePosition,(float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
        }
    }
    void onS() {
        camera.window.left += 10;
        camera.window.bottom += 20;
        if(    camera.window.right() >= camera.boundingWindow.right() ||
            camera.window.top() >= camera.boundingWindow.top()) {
            camera.window.left = 0;
            camera.window.bottom = 0;
        }
    }
    void onSpace() {
        if(tcr.count == 0) return;
        if(state == Scene::EDIT) {
            for(int i = 0; i < tcr.count; i++)
                bezier.p[i] = tcr.p[i];
 
            bezier.count = tcr.count;
            bezier.transform.phi = 60;
 
            float4 origin;
            float4 mass_p;
            for(int i = 0; i < tcr.count; i++){
                mass_p = mass_p + tcr.p[i];
            }
            mass_p = mass_p / tcr.count;
 
            float4 mass_p1(
                    mass_p.a * cosf (M_PI / -3.0f) + mass_p.b * sinf(M_PI / -3.0f),
                    mass_p.a * (-1.0f) * sinf(M_PI / -3.0f) + mass_p.b * cosf(M_PI / -3.0f),
                    0,
                    0
                    );
            bezier.transform.trans_x = mass_p.a - mass_p1.a;
            bezier.transform.trans_y = mass_p.b - mass_p1.b;
 
            animStart = glutGet(GLUT_ELAPSED_TIME);
            state = Scene::ANIMATION;
        }
    }
    static Scene& getInstance() {
        static Scene scene;
        return scene;
    }
private:
    Scene () : state(Scene::EDIT), animStart(), camera(), tcr(), bezier() {
 
    }
    Scene(const Scene&);
    void operator=(const Scene&);
};
 
Scene& scene = Scene::getInstance();
 
void onInitialization( ) {
 
}
 
void onDisplay( ) {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.renderGL();
    glutSwapBuffers();
}
 
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        scene.onS();
        glutPostRedisplay( );
    }
    else{
        if(key == ' '){
            scene.onSpace();
            glutPostRedisplay();
        }
    }
}
 
void onKeyboardUp(unsigned char key, int x, int y) {
 
}
 
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            scene.onLeftButton(x, 600 - y);
            glutPostRedisplay( );
    }
}
 
void onMouseMotion(int x, int y) {
 
}
void onIdle( ) {
     glutPostRedisplay();
}
 
// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 
// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv);                 // GLUT inicializalasa
    glutInitWindowSize(600, 600);            // Alkalmazas ablak kezdeti merete 600x600 pixel 
    glutInitWindowPosition(100, 100);            // Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);    // 8 bites R,G,B,A + dupla buffer + melyseg buffer
 
    glutCreateWindow("Grafika hazi feladat");        // Alkalmazas ablak megszuletik es megjelenik a kepernyon
 
    glMatrixMode(GL_MODELVIEW);                // A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);            // A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
 
    onInitialization();                    // Az altalad irt inicializalast lefuttatjuk
 
    glutDisplayFunc(onDisplay);                // Esemenykezelok regisztralasa
    glutMouseFunc(onMouse); 
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);
 
    glutMainLoop();                    // Esemenykezelo hurok
    
    return 0;
}