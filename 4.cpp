//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2013-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization f�ggv�nyt kiv�ve, a lefoglalt adat korrekt felszabad�t�sa n�lk�l
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
 
#define EPSILON 0.01
 
//--------------------------------------------------------
// 3D Vektor
//--------------------------------------------------------
struct Vector {
   float x, y, z;
 
   Vector( ) {
    x = y = z = 0;
   }
   Vector(float x0, float y0, float z0 = 0) {
    x = x0; y = y0; z = z0;
   }
   Vector operator*(float a) const {
    return Vector(x * a, y * a, z * a);
   }
   Vector operator+(const Vector& v) const {
     return Vector(x + v.x, y + v.y, z + v.z);
   }
   Vector operator-(const Vector& v) const {
     return Vector(x - v.x, y - v.y, z - v.z);
   }
   float operator*(const Vector& v) const {     // dot product
    return (x * v.x + y * v.y + z * v.z);
   }
   Vector operator%(const Vector& v) const {     // cross product
    return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
 
   Vector operator/(const float a) const {
       return Vector(x / a, y / a, z / a);
   }
   float Length() const { return sqrt(x * x + y * y + z * z); }
   void normalize() {
       float l = Length();
       x = x/l;
       y = y/l;
       z = z/l;
   }
};
 
//--------------------------------------------------------
// Spektrum illetve szin
//--------------------------------------------------------
 
struct Color {
   float r, g, b;
 
   Color( ) {
    r = g = b = 0;
   }
   Color(float r0, float g0, float b0) {
    r = r0; g = g0; b = b0;
   }
   Color operator*(float a) {
    return Color(r * a, g * a, b * a);
   }
   Color operator*(const Color& c) {
    return Color(r * c.r, g * c.g, b * c.b);
   }
   Color operator+(const Color& c) {
     return Color(r + c.r, g + c.g, b + c.b);
   }
};
 
struct Light {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat pos[4];
    Light(const Color& ambient, const Color& diffuse, const Color& specular, const Vector& pos, bool isPositional) {
        this->ambient[0] = ambient.r;    this->diffuse[0] = diffuse.r;    this->specular[0] = specular.r;
        this->ambient[1] = ambient.g;    this->diffuse[1] = diffuse.g;    this->specular[1] = specular.g;
        this->ambient[2] = ambient.b;    this->diffuse[2] = diffuse.b;    this->specular[2] = specular.b;
        this->ambient[3] = this->diffuse[3] = this->specular[3] =  1.f;
 
        float l = 1.0f;
        if(!isPositional) { l = pos.Length(); }
        this->pos[0] = pos.x/l;
        this->pos[1] = pos.y/l;
        this->pos[2] = pos.z/l;
        this->pos[3] = isPositional ? 1.f : 0.f;
 
    }
    void setGL(GLenum n) const {
        glLightfv(n, GL_AMBIENT, ambient);
        glLightfv(n, GL_DIFFUSE, diffuse);
        glLightfv(n, GL_SPECULAR, specular);
        glLightfv(n, GL_POSITION, pos);
        glEnable(n);
 
    }
 
};
 
struct Camera{
    Vector position;
    Vector up;
    Vector lookAt;
    Camera(const Vector& position, const Vector& up, const Vector& lookAt) :
        position(position), up(up), lookAt(lookAt) {}
    void setGL() {
        glViewport(0, 0, 600, 600);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity( );
        gluPerspective(25, 1, 1, 200);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity( );
        gluLookAt(position.x,
                    position.y,
                    position.z,
                    lookAt.x,
                    lookAt.y,
                    lookAt.z,
                    up.x,
                    up.y,
                    up.z
                    );
    }
};
 
template <typename T>
class CR {
public:
    const float tension;
    int count;
    float t[10];
    T p[10];
    T v[10];
    CR() :
        tension(0.f),
        count(0),
        t(),
        p(),
        v()
    { }
    int index(float t) const {
        for(int i = 0; i < count; i++) {
            if(this->t[i] > t) return i - 1;
        }
        return 0;
    }
    void addControlPoint(const T& point, float param) {
        if(count == 10) return;
 
        p[count] = point;
        t[count] = param;
        count++;
        if(count > 2) {
            v[count - 2] = _v(count - 2);
        }
 
        return;
    }
    T _v(int i) const {
        if(i < 0 || i >= count - 1)
            return T();
        const T& prev_p = p[i - 1];
        const T& this_p = p[i];
        const T& next_p = p[i + 1];
 
        const float& prev_t = t[i - 1];
        const float& this_t = t[i];
        const float& next_t = t[i + 1];
 
        const T prev2this(
                (this_p - prev_p) / (this_t - prev_t)
                );
        const T this2next(
                        (next_p - this_p) / (next_t - this_t)
                        );
        return (prev2this + this2next) * ((1.0f - tension) / 2.0f);
    }
 
 
    T operator()(float param) {
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
 
        const T& this_p = p[this_i];
        const T& next_p = p[next_i];
 
        const T& this_v = v[this_i];
        const T& next_v = v[next_i];
 
        const T& a0 = this_p;
        const T& a1 = this_v;
 
        const float tau = next_t - this_t;
        const T eps = next_p - this_p;
 
        const T a2 = (eps * 3.0f) / (tau * tau) - (next_v + this_v * 2.0f) / tau;
        const T a3 = (eps * -2.0f) / (tau * tau * tau) + (next_v + this_v) / (tau * tau);
 
        const float dt = param - this_t;
 
        return  T(
                    a0 +
                    a1 * dt +
                    a2 * dt * dt +
                    a3 * dt * dt * dt
                    );
    }
    T speed(float param) const {
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
 
        const T& this_p = p[this_i];
        const T& next_p = p[next_i];
 
        const T& this_v = v[this_i];
        const T& next_v = v[next_i];
 
        const T& a1 = this_v;
 
        const float tau = next_t - this_t;
        const T eps = next_p - this_p;
 
        const T a2 = (eps * 3.0f) / (tau * tau) - (next_v + this_v * 2.0f) / tau;
        const T a3 = (eps * -2.0f) / (tau * tau * tau) + (next_v + this_v) / (tau * tau);
 
        const float dt = param - this_t;
        return T(
                a1 +
                a2 * dt * 2 +
                a3 * dt * dt *3
                        );
    }
};
 
struct Ellipsoid {
    float a,b,c;
    Ellipsoid(float a, float b, float c) : a(a), b(b), c(c) { }
    Vector operator()(float u, float v) const {
        return Vector(
                a * cosf(v) * sinf(u),
                b * sinf(v) * sinf(u),
                c * cosf(u)
                );
    }
    Vector normalAt(const Vector& p) const
    {
        Vector n(
                2.f * p.x / a / a,
                2.f * p.y / b / b,
                2.f * p.z / c / c
                );
        n.normalize();
        return n;
    }
};
 
struct Cone {
    Cone() {}
    Vector operator()(float u, float v) const {
        return Vector(
                cosf(u) * v,
                sinf(u) * v,
                v
                );
    }
    Vector normalAt(const Vector& p) const {
        return Vector(
                2.0f * p.x,
                2.0f * p.y,
                -2.0f * p.z
                );
    }
};
 
struct Frog {
    Ellipsoid body, arm, eye;
    Vector body_p[20][30][4];
    Vector body_n[20][30][4];
    Vector arm_n[10][15][4];
    Vector arm_p[10][15][4];
    Vector eye_n[10][15][4];
    Vector eye_p[10][15][4];
    const unsigned int tx;
    GLuint tid;
 
    Frog() :
        body(1.f, .5f, .75f),
        arm(1.f, .1f, .5f),
        eye(1.f, 1.f, 1.f),
        tx(32),
        tid()
    {
        {
            //Test tesszellacio
            float du = M_PI / 20.f;
            float dv = M_PI / 15.f;
            for(unsigned int i = 0; i < 20; ++i)
            {
                float u = (float)i * du;
                for(unsigned int j = 0; j < 30; ++j)
                {
                    float v = (float)j * dv;
                    body_p[i][j][0] = body(u, v);                 body_n[i][j][0] = body.normalAt(body_p[i][j][0]);
                    body_p[i][j][1] = body(u, v + dv);            body_n[i][j][1] = body.normalAt(body_p[i][j][1]);
                    body_p[i][j][2] = body(u + du, v);             body_n[i][j][2] = body.normalAt(body_p[i][j][2]);
                    body_p[i][j][3] = body(u + du, v + dv);        body_n[i][j][3] = body.normalAt(body_p[i][j][3]);
                }
            }
        }
        {
            //Kez tesszellacio
            float du = M_PI / 10.f;
            float dv = M_PI / 7.5f;
            for(unsigned int i = 0; i < 10; ++i)
            {
                float u = (float)i * du;
                for(unsigned int j = 0; j < 15; ++j)
                {
                    float v = (float)j * dv;
                    arm_p[i][j][0] = arm(u, v);                 arm_n[i][j][0] = arm.normalAt(arm_p[i][j][0]);
                    arm_p[i][j][1] = arm(u, v + dv);            arm_n[i][j][1] = arm.normalAt(arm_p[i][j][1]);
                    arm_p[i][j][2] = arm(u + du, v);             arm_n[i][j][2] = arm.normalAt(arm_p[i][j][2]);
                    arm_p[i][j][3] = arm(u + du, v + dv);        arm_n[i][j][3] = arm.normalAt(arm_p[i][j][3]);
                }
            }
        }
        {
            //Szem tesszellacio
            float du = M_PI / 10.f;
            float dv = M_PI / 7.5f;
            for(unsigned int i = 0; i < 10; ++i)
            {
                float u = (float)i * du;
                for(unsigned int j = 0; j < 15; ++j)
                {
                    float v = (float)j * dv;
                    eye_p[i][j][0] = eye(u, v);                 eye_n[i][j][0] = eye.normalAt(eye_p[i][j][0]);
                    eye_p[i][j][1] = eye(u, v + dv);            eye_n[i][j][1] = eye.normalAt(eye_p[i][j][1]);
                    eye_p[i][j][2] = eye(u + du, v);             eye_n[i][j][2] = eye.normalAt(eye_p[i][j][2]);
                    eye_p[i][j][3] = eye(u + du, v + dv);        eye_n[i][j][3] = eye.normalAt(eye_p[i][j][3]);
                }
            }
        }
 
 
    }
    void createTextureGL() {
        if(tid) return;
        {
            float * texture = new float[tx * 3];
 
            for(unsigned int i = 0; i < tx; ++i)
                    for(int k = 0; k < 3; ++k)
                        texture[3 * i + k] = i < 6 ? 0 : 1;
            glGenTextures(1, &tid);
            glBindTexture(GL_TEXTURE_1D, tid);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, tx, 0,
                    GL_RGB, GL_FLOAT, texture);
            glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            delete[] texture;
        }
    }
    void drawGL() const {
        {
            GLfloat kd[] = {0.0, 1.0, 0.0, 1.0};
            GLfloat ks[] = {2.5, 2.5, 2.5, 1.0};
            GLfloat ka[] = {0.2, 0.7, 0.1, 1.0};
            glMaterialfv( GL_FRONT,
                                  GL_AMBIENT, ka);
            glMaterialfv( GL_FRONT,
                                  GL_DIFFUSE, kd);
            glMaterialfv( GL_FRONT,
                                  GL_SPECULAR, ks);
            glMaterialf( GL_FRONT,
                                GL_SHININESS, 20.0);
        }
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glRotatef(30, 0, 0, 1);
        glTranslatef(0,.5f,0);
 
        for(int i = 0; i < 20 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 30; ++j)
                {
                    glNormal3f(body_n[i][j][0].x, body_n[i][j][0].y, body_n[i][j][0].z);
                    glVertex3f(body_p[i][j][0].x, body_p[i][j][0].y, body_p[i][j][0].z);
 
                    glNormal3f(body_n[i][j][1].x, body_n[i][j][1].y, body_n[i][j][1].z);
                    glVertex3f(body_p[i][j][1].x, body_p[i][j][1].y, body_p[i][j][1].z);
 
                    glNormal3f(body_n[i][j][2].x, body_n[i][j][2].y, body_n[i][j][2].z);
                    glVertex3f(body_p[i][j][2].x, body_p[i][j][2].y, body_p[i][j][2].z);
 
                    glNormal3f(body_n[i][j][3].x, body_n[i][j][3].y, body_n[i][j][3].z);
                    glVertex3f(body_p[i][j][3].x, body_p[i][j][3].y, body_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5f,0.2f,0.5f);
        glRotatef(-30,0,0,1);
        glScalef(0.4,0.4,0.4);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glNormal3f(arm_n[i][j][0].x, arm_n[i][j][0].y, arm_n[i][j][0].z);
                    glVertex3f(arm_p[i][j][0].x, arm_p[i][j][0].y, arm_p[i][j][0].z);
 
                    glNormal3f(arm_n[i][j][1].x, arm_n[i][j][1].y, arm_n[i][j][1].z);
                    glVertex3f(arm_p[i][j][1].x, arm_p[i][j][1].y, arm_p[i][j][1].z);
 
                    glNormal3f(arm_n[i][j][2].x, arm_n[i][j][2].y, arm_n[i][j][2].z);
                    glVertex3f(arm_p[i][j][2].x, arm_p[i][j][2].y, arm_p[i][j][2].z);
 
                    glNormal3f(arm_n[i][j][3].x, arm_n[i][j][3].y, arm_n[i][j][3].z);
                    glVertex3f(arm_p[i][j][3].x, arm_p[i][j][3].y, arm_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.5f,0.2f,-0.5f);
        glRotatef(-30,0,0,1);
        glScalef(0.4,0.4,0.4);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glNormal3f(arm_n[i][j][0].x, arm_n[i][j][0].y, arm_n[i][j][0].z);
                    glVertex3f(arm_p[i][j][0].x, arm_p[i][j][0].y, arm_p[i][j][0].z);
 
                    glNormal3f(arm_n[i][j][1].x, arm_n[i][j][1].y, arm_n[i][j][1].z);
                    glVertex3f(arm_p[i][j][1].x, arm_p[i][j][1].y, arm_p[i][j][1].z);
 
                    glNormal3f(arm_n[i][j][2].x, arm_n[i][j][2].y, arm_n[i][j][2].z);
                    glVertex3f(arm_p[i][j][2].x, arm_p[i][j][2].y, arm_p[i][j][2].z);
 
                    glNormal3f(arm_n[i][j][3].x, arm_n[i][j][3].y, arm_n[i][j][3].z);
                    glVertex3f(arm_p[i][j][3].x, arm_p[i][j][3].y, arm_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5f,0.04f,0.7f);
        glRotatef(-30,0,1,0);
        glScalef(0.6,0.6,0.6);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glNormal3f(arm_n[i][j][0].x, arm_n[i][j][0].y, arm_n[i][j][0].z);
                    glVertex3f(arm_p[i][j][0].x, arm_p[i][j][0].y, arm_p[i][j][0].z);
 
                    glNormal3f(arm_n[i][j][1].x, arm_n[i][j][1].y, arm_n[i][j][1].z);
                    glVertex3f(arm_p[i][j][1].x, arm_p[i][j][1].y, arm_p[i][j][1].z);
 
                    glNormal3f(arm_n[i][j][2].x, arm_n[i][j][2].y, arm_n[i][j][2].z);
                    glVertex3f(arm_p[i][j][2].x, arm_p[i][j][2].y, arm_p[i][j][2].z);
 
                    glNormal3f(arm_n[i][j][3].x, arm_n[i][j][3].y, arm_n[i][j][3].z);
                    glVertex3f(arm_p[i][j][3].x, arm_p[i][j][3].y, arm_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-0.5f,0.04f,-0.7f);
        glRotatef(30,0,1,0);
        glScalef(0.6,0.6,0.6);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glNormal3f(arm_n[i][j][0].x, arm_n[i][j][0].y, arm_n[i][j][0].z);
                    glVertex3f(arm_p[i][j][0].x, arm_p[i][j][0].y, arm_p[i][j][0].z);
 
                    glNormal3f(arm_n[i][j][1].x, arm_n[i][j][1].y, arm_n[i][j][1].z);
                    glVertex3f(arm_p[i][j][1].x, arm_p[i][j][1].y, arm_p[i][j][1].z);
 
                    glNormal3f(arm_n[i][j][2].x, arm_n[i][j][2].y, arm_n[i][j][2].z);
                    glVertex3f(arm_p[i][j][2].x, arm_p[i][j][2].y, arm_p[i][j][2].z);
 
                    glNormal3f(arm_n[i][j][3].x, arm_n[i][j][3].y, arm_n[i][j][3].z);
                    glVertex3f(arm_p[i][j][3].x, arm_p[i][j][3].y, arm_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        {
            GLfloat kd[] = {1, 1, 1, 1.0};
            GLfloat ks[] = {1, 1, 1, 1.0};
            GLfloat ka[] = {1, 1, 1, 1.0};
            glMaterialfv( GL_FRONT,
                                  GL_AMBIENT, ka);
            glMaterialfv( GL_FRONT,
                                  GL_DIFFUSE, kd);
            glMaterialfv( GL_FRONT,
                                  GL_SPECULAR, ks);
            glMaterialf( GL_FRONT,
                                GL_SHININESS, 20.0f);
        }
        glPushMatrix();
        glEnable(GL_TEXTURE_1D);
        glBindTexture(GL_TEXTURE_1D, tid);
        glTranslatef(0.4f,1.f,0.3f);
        glScalef(0.2,0.2,0.2);
        glRotatef(90, 0, 1, 0);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][0].x, eye_n[i][j][0].y, eye_n[i][j][0].z);
                    glVertex3f(eye_p[i][j][0].x, eye_p[i][j][0].y, eye_p[i][j][0].z);
 
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][1].x, eye_n[i][j][1].y, eye_n[i][j][1].z);
                    glVertex3f(eye_p[i][j][1].x, eye_p[i][j][1].y, eye_p[i][j][1].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][2].x, eye_n[i][j][2].y, eye_n[i][j][2].z);
                    glVertex3f(eye_p[i][j][2].x, eye_p[i][j][2].y, eye_p[i][j][2].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][3].x, eye_n[i][j][3].y, eye_n[i][j][3].z);
                    glVertex3f(eye_p[i][j][3].x, eye_p[i][j][3].y, eye_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.4f,1.f,-0.3f);
        glScalef(0.2,0.2,0.2);
        glRotatef(90, 0, 1, 0);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][0].x, eye_n[i][j][0].y, eye_n[i][j][0].z);
                    glVertex3f(eye_p[i][j][0].x, eye_p[i][j][0].y, eye_p[i][j][0].z);
 
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][1].x, eye_n[i][j][1].y, eye_n[i][j][1].z);
                    glVertex3f(eye_p[i][j][1].x, eye_p[i][j][1].y, eye_p[i][j][1].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][2].x, eye_n[i][j][2].y, eye_n[i][j][2].z);
                    glVertex3f(eye_p[i][j][2].x, eye_p[i][j][2].y, eye_p[i][j][2].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][3].x, eye_n[i][j][3].y, eye_n[i][j][3].z);
                    glVertex3f(eye_p[i][j][3].x, eye_p[i][j][3].y, eye_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glDisable(GL_TEXTURE_1D);
 
    }
    ~Frog() {
        if(tid) {
            glDeleteTextures(GL_TEXTURE_1D, &tid); //TODO milegyen vele?
            tid = 0;
        }
    }
};
 
struct Plane {
    Vector orig;
    unsigned int size;
    const unsigned int tx;
    const unsigned int ty;
    GLuint tid;
    Plane(const Vector& orig, unsigned int size = 1) :
        orig(orig),
        size(size),
        tx(512),
        ty(512),
        tid()
    {}
    void drawGL() const {
        GLfloat kd[] = {0.5, 0.5, 0.5, 1.0};
        GLfloat ks[] = {2.9, 2.9, 2.9, 1.0};
        GLfloat ka[] = {0.8, 0.8, 0.8, 1.0};
 
        glMaterialfv( GL_FRONT,
                              GL_AMBIENT, ka);
        glMaterialfv( GL_FRONT,
                              GL_DIFFUSE, kd);
        glMaterialfv( GL_FRONT,
                              GL_SPECULAR, ks);
        glMaterialf( GL_FRONT,
                            GL_SHININESS, 11.0);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tid);
        glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
 
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_NORMALIZE);
        glPushMatrix();
 
        glTranslatef(orig.x,orig.y,orig.z);
        for(unsigned int i = 0; i < size ; ++i)
        {
            float du = 0.1f;
            float u = (float)i * du;
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(unsigned int j = 0; j < size; ++j)
                {
                    float dv = 0.1f;
                    float v = (float)j * dv;
                    glTexCoord2f(u/90.0,v/90.0);
                    glNormal3f(0,1,0);
                    glVertex3f(u,0,v);
 
                    glTexCoord2f(u/90.0,(v+dv)/90.0);
                    glNormal3f(0,1,0);
                    glVertex3f(u, 0 ,v + dv);
 
                    glTexCoord2f((u + du)/90.0,(v)/90.0);
                    glNormal3f(0,1,0);
                    glVertex3f(u + du,0,v);
 
 
                    glTexCoord2f((u + du)/90.0,(v + dv)/90.0);
                    glNormal3f(0,1,0);
                    glVertex3f(u + du,0,v+dv);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
 
 
    }
    void createTextureGL() {
        if(tid) return;
        {
            float * texture = new float[tx * ty * 3];
            for(unsigned int i = 0; i < ty; ++i)
                for(unsigned int j = 0; j < tx; ++j)
                {
                    float n = ((1299811 - ((i * j) << 9) * ( 13 * i) ) % 7927) / 7927.0f; //LE ULTIMATE TEXTURAZAJ
                    float f = (1.0 + 0.5*sinf((float)i * n * 0.5)) * (1.0 + 0.5f*sinf((float)j * n * 0.5));
                    texture[3 * tx * i + 3 * j] = 1-f * 0.9;
                    texture[3 * tx * i + 3 * j + 1] = 1-f * 0.7;
                    texture[3 * tx * i + 3 * j + 2] = 0.8;
 
                }
            glGenTextures(1, &tid);
            glBindTexture(GL_TEXTURE_2D, tid);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tx, ty, 0,
                    GL_RGB, GL_FLOAT, texture);
            glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
 
    }
    ~Plane() {
        if(tid) {
            glDeleteTextures(GL_TEXTURE_2D, & tid); //TODO milegyen vele ?
            tid = 0;
        }
    }
};
 
struct Golya {
    CR<Vector> spine;
    CR<float> skin;
    Ellipsoid head;
    Ellipsoid eye;
    Cone beak;
    Vector body_n[50][24][4];
    Vector body_p[50][24][4];
    Vector head_n[10][15][4];
    Vector head_p[10][15][4];
    Vector beak_n[15][4];
    Vector beak_p[15][4];
    Vector eye_n[10][15][4];
    Vector eye_p[10][15][4];
    const unsigned int t1x;
    const unsigned int t1y;
    GLuint t1id;
    const unsigned int t2x;
    GLuint t2id;
 
    Golya() :
        spine(),
        skin(),                 //SKINHEAD GOLYA
        head(1.f, 1.f, 0.74f),
        eye(1.f, 1.f, 1.f),
        beak(),
        body_n(),
        body_p(),
        head_n(),
        head_p(),
        beak_n(),
        beak_p(),
        eye_n(),
        eye_p(),
        t1x(512),
        t1y(512),
        t1id(),
        t2x(32),
        t2id()
 
        {
        spine.addControlPoint(Vector(0,0),0);
        spine.addControlPoint(Vector(1,-3),1);
        spine.addControlPoint(Vector(3,-5),2);
        spine.addControlPoint(Vector(7,-6),3);
        spine.addControlPoint(Vector(11,-7),4);
 
        skin.addControlPoint(0.5,0);
        skin.addControlPoint(0.6,1);
        skin.addControlPoint(1.5,2);
        skin.addControlPoint(1.9,3);
        skin.addControlPoint(0.3,4);
        {
            //test tesszellacioja
            for(unsigned int i = 0; i < 50 ; ++i){
                float du = 0.08f;
                float u = (float)i * du;
                if(u == 0) u = 0.01;
 
                for(unsigned int j = 0; j < 24 ; ++j){
                    float dv = M_PI / 12.f;
                    float v = (float)j * dv;
                    {
                        Vector o = spine(u);
                        float r = skin(u);
                        Vector k(0,0,1);
                        Vector vel = spine.speed(u);
                        Vector n(k % vel);
                        n.normalize();
                        Vector normal = k * r * cos(v) + n * r * sin(v);
                        body_p[i][j][0] = o + normal;
                        normal.normalize();
                        body_n[i][j][0] = normal;
                    }
                    {
                        Vector o = spine(u);
                        float r = skin(u);
                        Vector k(0,0,1);
                        Vector vel = spine.speed(u);
                        Vector n(k % vel);
                        n.normalize();
                        Vector normal = k * r * cos(v + dv) + n * r * sin(v + dv);
                        body_p[i][j][1] = o + normal;
                        normal.normalize();
                        body_n[i][j][1] = normal;
                    }
                    {
                        Vector o = spine(u + du);
                        float r = skin(u + du);
                        Vector k(0,0,1);
                        Vector vel = spine.speed(u + du);
                        Vector n(k % vel);
                        n.normalize();
                        Vector normal = k * r * cos(v) + n * r * sin(v);
                        body_p[i][j][2] = o + normal;
                        normal.normalize();
                        body_n[i][j][2] = normal;
                    }
                    {
                        Vector o = spine(u + du);
                        float r = skin(u + du);
                        Vector k(0,0,1);
                        Vector vel = spine.speed(u + du);
                        Vector n(k % vel);
                        n.normalize();
                        Vector normal = k * r * cos(v + dv) + n * r * sin(v + dv);
                        body_p[i][j][3] = o + normal;
                        normal.normalize();
                        body_n[i][j][3] = normal;
                    }
                }
            }
        }
        {
            //fej tesszellacioja
            float du = M_PI / 10.f;
            float dv = M_PI / 7.5f;
            for(unsigned int i = 0; i < 10; ++i)
            {
                float u = (float)i * du;
                for(unsigned int j = 0; j < 15; ++j)
                {
                    float v = (float)j * dv;
                    head_p[i][j][0] = head(u, v);                 head_n[i][j][0] = head.normalAt(head_p[i][j][0]);
                    head_p[i][j][1] = head(u, v + dv);            head_n[i][j][1] = head.normalAt(head_p[i][j][1]);
                    head_p[i][j][2] = head(u + du, v);             head_n[i][j][2] = head.normalAt(head_p[i][j][2]);
                    head_p[i][j][3] = head(u + du, v + dv);        head_n[i][j][3] = head.normalAt(head_p[i][j][3]);
                }
            }
        }
        {
            //csor tesszellacioja
            float du = M_PI / 7.5f;
            float dv = 1.0f;
            float v = 0.01;
            for(unsigned int j = 0; j < 15; ++j)
            {
                float u = (float)j * du;
                beak_p[j][0] = beak(u, v);                 beak_n[j][0] = beak.normalAt(beak_p[j][0]);
                beak_p[j][1] = beak(u, v + dv);            beak_n[j][1] = beak.normalAt(beak_p[j][1]);
                beak_p[j][2] = beak(u + du, v);         beak_n[j][2] = beak.normalAt(beak_p[j][2]);
                beak_p[j][3] = beak(u + du, v + dv);    beak_n[j][3] = beak.normalAt(beak_p[j][3]);
            }
        }
        {
            //Szem tesszellacio
            float du = M_PI / 10.f;
            float dv = M_PI / 7.5f;
            for(unsigned int i = 0; i < 10; ++i)
            {
                float u = (float)i * du;
                for(unsigned int j = 0; j < 15; ++j)
                {
                    float v = (float)j * dv;
                    eye_p[i][j][0] = eye(u, v);                 eye_n[i][j][0] = eye.normalAt(eye_p[i][j][0]);
                    eye_p[i][j][1] = eye(u, v + dv);            eye_n[i][j][1] = eye.normalAt(eye_p[i][j][1]);
                    eye_p[i][j][2] = eye(u + du, v);             eye_n[i][j][2] = eye.normalAt(eye_p[i][j][2]);
                    eye_p[i][j][3] = eye(u + du, v + dv);        eye_n[i][j][3] = eye.normalAt(eye_p[i][j][3]);
                }
            }
        }
 
    }
    void createTextureGL() {
        if(t1id) return;
        {
            float * texture = new float[t1x * t1y * 3];
 
            for(unsigned int i = 0; i < t1y; ++i)
                for(unsigned int j = 0; j < t1x; ++j)
                {
                    float f = sinf((float)i / 14.0f)*sinf((float)i / 38.0f) * sinf((float)j / 10.0f)*sinf((float)j / 50.0f);
                    for(int k = 0; k < 3; ++k)
                        texture[3 * t1x * i + 3 * j + k] = 1.0 - f;
                }
            glGenTextures(1, &t1id);
            glBindTexture(GL_TEXTURE_2D, t1id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t1x, t1y, 0,
                    GL_RGB, GL_FLOAT, texture);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            delete[] texture;
        }
        if(t2id) return;
        {
            float * texture = new float[t2x * 3];
 
            for(unsigned int i = 0; i < t2x; ++i)
                    for(int k = 0; k < 3; ++k)
                        texture[3 * i + k] = i < 6.f ? 0 : 1;
            glGenTextures(1, &t2id);
            glBindTexture(GL_TEXTURE_1D, t2id);
            glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, t2x, 0,
                    GL_RGB, GL_FLOAT, texture);
            glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            delete[] texture;
        }
 
 
    }
 
    void drawGL()
    {
        {
            GLfloat kd[] = {0.9, 0.9, 0.9, 1.0};
            GLfloat ks[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat ka[] = {0.8, 0.8, 0.8, 1.0};
            glMaterialfv( GL_FRONT,
                                  GL_AMBIENT, ka);
            glMaterialfv( GL_FRONT,
                                  GL_DIFFUSE, kd);
            glMaterialfv( GL_FRONT,
                                  GL_SPECULAR, ks);
            glMaterialf( GL_FRONT,
                                GL_SHININESS, 10.0);
        }
        glEnable(GL_LIGHTING);
 
        glEnable(GL_NORMALIZE);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, t1id);
 
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glScalef(0.3,0.3,0.3);
        glTranslatef(-5.5f,12.0f,0.0f);
 
        for(int i = 0; i < 50 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 24; ++j)
                {
                    glTexCoord2f((float)i/50.0f,(float)j/24.0f);
                    glNormal3f(body_n[i][j][0].x, body_n[i][j][0].y, body_n[i][j][0].z);
                    glVertex3f(body_p[i][j][0].x, body_p[i][j][0].y, body_p[i][j][0].z);
 
                    glTexCoord2f((float)i/50.0f,((float)j+1.0f)/24.0f);
                    glNormal3f(body_n[i][j][1].x, body_n[i][j][1].y, body_n[i][j][1].z);
                    glVertex3f(body_p[i][j][1].x, body_p[i][j][1].y, body_p[i][j][1].z);
 
                    glTexCoord2f(((float)i+1.0f)/50.0f,(float)j/24.0f);
                    glNormal3f(body_n[i][j][2].x, body_n[i][j][2].y, body_n[i][j][2].z);
                    glVertex3f(body_p[i][j][2].x, body_p[i][j][2].y, body_p[i][j][2].z);
 
                    glTexCoord2f(((float)i+1.0f)/50.0f,((float)j+1.0f)/24.0f);
                    glNormal3f(body_n[i][j][3].x, body_n[i][j][3].y, body_n[i][j][3].z);
                    glVertex3f(body_p[i][j][3].x, body_p[i][j][3].y, body_p[i][j][3].z);
                }
            }
            glEnd();
        }
 
        glPopMatrix();
        glDisable(GL_TEXTURE_2D);
        glPushMatrix();
        glTranslatef(-1.6f,3.6f,0);
        glScalef(.27f,.27f,.27f);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glNormal3f(head_n[i][j][0].x, head_n[i][j][0].y, head_n[i][j][0].z);
                    glVertex3f(head_p[i][j][0].x, head_p[i][j][0].y, head_p[i][j][0].z);
 
                    glNormal3f(head_n[i][j][1].x, head_n[i][j][1].y, head_n[i][j][1].z);
                    glVertex3f(head_p[i][j][1].x, head_p[i][j][1].y, head_p[i][j][1].z);
 
                    glNormal3f(head_n[i][j][2].x, head_n[i][j][2].y, head_n[i][j][2].z);
                    glVertex3f(head_p[i][j][2].x, head_p[i][j][2].y, head_p[i][j][2].z);
 
                    glNormal3f(head_n[i][j][3].x, head_n[i][j][3].y, head_n[i][j][3].z);
                    glVertex3f(head_p[i][j][3].x, head_p[i][j][3].y, head_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
 
 
        {
            GLfloat kd[] = {1.0, 0.0, 0.0, 1.0};
            GLfloat ks[] = {1.0, 0.0, 0.0, 1.0};
            GLfloat ka[] = {1.0, 0.0, 0.0, 1.0};
            glMaterialfv( GL_FRONT,
                                  GL_AMBIENT, ka);
            glMaterialfv( GL_FRONT,
                                  GL_DIFFUSE, kd);
            glMaterialfv( GL_FRONT,
                                  GL_SPECULAR, ks);
            glMaterialf( GL_FRONT,
                                GL_SHININESS, 10.0);
        }
        glPushMatrix();
        glTranslatef(-3.2f,3.6f,0);
        glRotatef(90, 0, 1, 0);
        glScalef(0.08f,0.08f,1.4f);
        glBegin(GL_TRIANGLE_STRIP);
        {
            for(int j = 0; j < 15; ++j)
            {
                glNormal3f(beak_n[j][0].x, beak_n[j][0].y, beak_n[j][0].z);
                glVertex3f(beak_p[j][0].x, beak_p[j][0].y, beak_p[j][0].z);
 
                glNormal3f(beak_n[j][1].x, beak_n[j][1].y, beak_n[j][1].z);
                glVertex3f(beak_p[j][1].x, beak_p[j][1].y, beak_p[j][1].z);
 
                glNormal3f(beak_n[j][2].x, beak_n[j][2].y, beak_n[j][2].z);
                glVertex3f(beak_p[j][2].x, beak_p[j][2].y, beak_p[j][2].z);
 
                glNormal3f(beak_n[j][3].x, beak_n[j][3].y, beak_n[j][3].z);
                glVertex3f(beak_p[j][3].x, beak_p[j][3].y, beak_p[j][3].z);
            }
        }
        glEnd();
        glPopMatrix();
 
        //LABAK
        glPushMatrix();
        glTranslatef(0,1.4,0.3);
        glRotatef(-120, 0,0,1);
        glScalef(.05,1.1,0.05);
        {
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(-1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,-1);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(1,0,0);
            glVertex3f(1,1,-1);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,0,-1);
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0,1.4,-0.3);
        glRotatef(-120, 0,0,1);
        glScalef(.05,1.1,0.05);
        {
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(-1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,-1);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(1,0,0);
            glVertex3f(1,1,-1);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,0,-1);
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.33,-0.11,-0.3);
        glRotatef(-30, 0,0,1);
        glScalef(.05,1.1,0.05);
        {
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(-1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,-1);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(1,0,0);
            glVertex3f(1,1,-1);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,0,-1);
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(0.33,-0.11,+0.3);
        glRotatef(-30, 0,0,1);
        glScalef(.05,1.1,0.05);
        {
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,1,0);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(-1,0,0);
            glVertex3f(0,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_STRIP);
            glNormal3f(0,0,-1);
            glVertex3f(0,0,-1);
            glVertex3f(0,1,-1);
            glVertex3f(1,0,-1);
            glVertex3f(1,1,-1);
            glEnd();
            glBegin(GL_TRIANGLE_FAN);
            glNormal3f(1,0,0);
            glVertex3f(1,1,-1);
            glVertex3f(1,1,0);
            glVertex3f(1,0,0);
            glVertex3f(1,0,-1);
            glEnd();
        }
        glPopMatrix();
        {
            GLfloat kd[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat ks[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat ka[] = {1.0, 1.0, 1.0, 1.0};
            glMaterialfv( GL_FRONT,
                                  GL_AMBIENT, ka);
            glMaterialfv( GL_FRONT,
                                  GL_DIFFUSE, kd);
            glMaterialfv( GL_FRONT,
                                  GL_SPECULAR, ks);
            glMaterialf( GL_FRONT,
                                GL_SHININESS, 10.0);
        }
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_TEXTURE_1D);
        glBindTexture(GL_TEXTURE_1D, t2id);
 
        glPushMatrix();
        glTranslatef(-1.66,3.64,0.18);
        glScalef(0.1,0.1,0.1);
 
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][0].x, eye_n[i][j][0].y, eye_n[i][j][0].z);
                    glVertex3f(eye_p[i][j][0].x, eye_p[i][j][0].y, eye_p[i][j][0].z);
 
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][1].x, eye_n[i][j][1].y, eye_n[i][j][1].z);
                    glVertex3f(eye_p[i][j][1].x, eye_p[i][j][1].y, eye_p[i][j][1].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][2].x, eye_n[i][j][2].y, eye_n[i][j][2].z);
                    glVertex3f(eye_p[i][j][2].x, eye_p[i][j][2].y, eye_p[i][j][2].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][3].x, eye_n[i][j][3].y, eye_n[i][j][3].z);
                    glVertex3f(eye_p[i][j][3].x, eye_p[i][j][3].y, eye_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glPushMatrix();
        glTranslatef(-1.66,3.64,-0.18);
        glRotatef(180, 1, 0, 0);
        glScalef(0.1,0.1,0.1);
        for(int i = 0; i < 10 ; ++i)
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for(int j = 0; j < 15; ++j)
                {
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][0].x, eye_n[i][j][0].y, eye_n[i][j][0].z);
                    glVertex3f(eye_p[i][j][0].x, eye_p[i][j][0].y, eye_p[i][j][0].z);
 
                    glTexCoord1f(((float)i)/10.0f);
                    glNormal3f(eye_n[i][j][1].x, eye_n[i][j][1].y, eye_n[i][j][1].z);
                    glVertex3f(eye_p[i][j][1].x, eye_p[i][j][1].y, eye_p[i][j][1].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][2].x, eye_n[i][j][2].y, eye_n[i][j][2].z);
                    glVertex3f(eye_p[i][j][2].x, eye_p[i][j][2].y, eye_p[i][j][2].z);
 
                    glTexCoord1f(((float)i+1.0f)/10.0f);
                    glNormal3f(eye_n[i][j][3].x, eye_n[i][j][3].y, eye_n[i][j][3].z);
                    glVertex3f(eye_p[i][j][3].x, eye_p[i][j][3].y, eye_p[i][j][3].z);
                }
            }
            glEnd();
        }
        glPopMatrix();
        glDisable(GL_TEXTURE_1D);
 
 
    }
    ~Golya() {
        if(t1id) {
            glDeleteTextures(GL_TEXTURE_2D,&t1id); //TODO milegyen?
            t1id = 0;
        }
        if(t2id) {
            glDeleteTextures(GL_TEXTURE_1D,&t2id); //TODO milegyen?
            t2id = 0;
        }
 
    }
};
 
Light janos(
        Color(0,0,0),
        Color(0.8,0.5,0),
        Color(1.3, 1.3, 0),
        Vector (-1.6,3,-1.6),
        true
        );
Light evening_sky(
        Color(0,0,0.1),
        Color(0,0,0),
        Color(0,0,0),
        Vector(0,0,1), // hogy a narmalizalas miatt ne szalljon el
        false
);
Light sundown(
        Color(0,0,0),
        Color(2,0.82,1.39),
        Color(2,0.82,1.39),
        Vector(5,-1,5),
        false
        );
;
Camera cam(
        Vector(22 * sinf(4.1 * M_PI / 3.0f), 9.f, 22 * cosf( 4.1 * M_PI / 3.0f)),
        Vector(0.f, 1.f, 0.f),
        Vector(0,2.f,2.0f)
        );
Frog fiubeka;
Frog lanybeka;
Golya arrogancia;
Plane viz(
        Vector(-10,0,-10),
        600
        );
 
 
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
 
 
    lanybeka.createTextureGL();
    arrogancia.createTextureGL();
    viz.createTextureGL();
    fiubeka.createTextureGL();
}
 
// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 
    cam.setGL();
 
    janos.setGL(GL_LIGHT0);
    evening_sky.setGL(GL_LIGHT1);
    sundown.setGL(GL_LIGHT2);
 
 
 
 
 
    glPushMatrix();
    glTranslatef(-2.0,0,3.0);
    glRotatef(120, 0,1,0);
    lanybeka.drawGL();
    glTranslatef(-1,0.5,0);
    glRotatef(30, 0, 0, 1);
    fiubeka.drawGL();
    glPopMatrix();
    glPushMatrix();
    glScalef(1.6,1.6,1.6);
    arrogancia.drawGL();
    glPopMatrix();
    glPushMatrix();
    viz.drawGL();
    glPopMatrix();
 
    glutSwapBuffers();                     // Buffercsere: rajzolas vege
 
}
 
// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {
    if (key == 'd') glutPostRedisplay( );         // d beture rajzold ujra a kepet
 
}
 
// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {
 
}
 
// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)   // A GLUT_LEFT_BUTTON / GLUT_RIGHT_BUTTON illetve GLUT_DOWN / GLUT_UP
        glutPostRedisplay( );                          // Ilyenkor rajzold ujra a kepet
}
 
// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{
 
}
 
// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
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