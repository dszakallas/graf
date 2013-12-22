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
// Nev    : Szakallas David
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
   Vector operator*(float a)const  {
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
   Vector operator%(const Vector& v)const  {     // cross product
    return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
   }
   float Length() { return sqrt(x * x + y * y + z * z); }
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
 
const int screenWidth = 600;    // alkalmaz�s ablak felbont�sa
const int screenHeight = 600;
 
namespace Scene {
    namespace Slingshot{
        Vector launchVelocity;
        Vector launchPosition(-0.333f,-0.333f,0.0f);
        float launchTime;
    }
    //TYPES
    class Bird {
        private:
            Vector position;
            Color color;
            float ellipseParameterA;
            float ellipseParameterB;
            float facingX;
            float facingY;
        public:
            enum Facing{
                LEFT,
                RIGHT,
                UPSIDE_DOWN
            };
        public:
            Bird(const Vector& position, const Color& color, Facing facing) :
                position(position),
                color(color),
                ellipseParameterA(0.1f),
                ellipseParameterB(0.2f)
            {
                switch(facing){
                case LEFT:
                    facingX = -1;
                    facingY = 1;
                    break;
                case RIGHT:
                    facingX = 1;
                    facingY = 1;
                    break;
                case UPSIDE_DOWN:
                    facingX = 1;
                    facingY = -1;
                    break;
                }
            }
            void setPosition(const Vector& position) {
                this->position = position;
            }
            void setColor(const Color& color) {
                this->color = color;
            }
            const Vector& getPosition() const {
                return position;
            }
            void draw() const {
                {
 
                    glColor3f(0.0f,0.0f,0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2f(position.x - 0.7 * ellipseParameterA * facingX, position.y);
                        glVertex2f(position.x - 1.2 * ellipseParameterA * facingX, position.y + ellipseParameterB * 0.8f * facingY);
                        glVertex2f(position.x - 1.2 * ellipseParameterA * facingX, position.y + ellipseParameterB * 0.15f * facingY);
                    glEnd();
                    glBegin(GL_TRIANGLES);
                        glVertex2f(position.x - 0.7 * ellipseParameterA * facingX, position.y);
                        glVertex2f(position.x - 1.2 * ellipseParameterA * facingX, position.y - ellipseParameterB * 0.8f * facingY);
                        glVertex2f(position.x - 1.2 * ellipseParameterA * facingX, position.y - ellipseParameterB * 0.15f * facingY);
                    glEnd();
 
                    glColor3f(color.r,color.g,color.b);
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = position.x + cosf(2 * t * M_PI) * ellipseParameterA;
                        float y = position.y + sinf(2 * t * M_PI) * ellipseParameterB;
                        glVertex2f(x,y);
                    }
                    glEnd();
 
                    glColor3f(1.0f,1.0f,1.0f);
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = position.x + cosf(2 * t * M_PI) * ellipseParameterA*0.4 + ellipseParameterA *0.5 * facingX;
                        float y = position.y + sinf(2 * t * M_PI) * ellipseParameterB*0.4 + ellipseParameterB * 0.5 * facingY;
                        glVertex2f(x,y);
                    }
                    glEnd();
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = position.x + cosf(2 * t * M_PI) * ellipseParameterA*0.4 - ellipseParameterA *0.1 * facingX;
                        float y = position.y + sinf(2 * t * M_PI) * ellipseParameterB*0.4 + ellipseParameterB * 0.5 * facingY;
                        glVertex2f(x,y);
                    }
                    glEnd();
 
                    glColor3f(0.0f,0.0f,0.0f);
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = position.x + cosf(2 * t * M_PI) * ellipseParameterA*0.2 + ellipseParameterA *0.6 * facingX;
                        float y = position.y + sinf(2 * t * M_PI) * ellipseParameterB*0.2 + ellipseParameterB * 0.6 * facingY;
                        glVertex2f(x,y);
                    }
                    glEnd();
                    glBegin(GL_POLYGON);
                    for(float t = 0; t < 1; t+=0.05f) {
                        float x = position.x + cosf(2 * t * M_PI) * ellipseParameterA*0.2;
                        float y = position.y + sinf(2 * t * M_PI) * ellipseParameterB*0.2 + ellipseParameterB * 0.6 * facingY;
                        glVertex2f(x,y);
                    }
                    glEnd();
 
                    glColor3f(1.0f,0.8f,0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2f(position.x + ellipseParameterA * 0.2 * facingX, position.y + ellipseParameterB * 0.1 * facingY);
                        glVertex2f(position.x + ellipseParameterA * 0.2 * facingX, position.y - ellipseParameterB * 0.1 * facingY);
                        glVertex2f(position.x + ellipseParameterA * facingX, position.y);
                    glEnd();
 
                    glColor3f(0.0f,0.0f,0.0f);
                    glBegin(GL_TRIANGLES);
                        glVertex2f(position.x + ellipseParameterA * 0.6f * facingX, position.y + ellipseParameterB* 0.8f * facingY);
                        glVertex2f(position.x + ellipseParameterA * 1.4f * facingX, position.y + ellipseParameterB* 0.6f * facingY);
                        glVertex2f(position.x + ellipseParameterA * 0.6f * facingX, position.y + ellipseParameterB* 1.0f * facingY);
                        glVertex2f(position.x - ellipseParameterA * 0.1f * facingX, position.y + ellipseParameterB* 1.0f * facingY);
                        glVertex2f(position.x - ellipseParameterA * 0.9f * facingX, position.y + ellipseParameterB* 0.6f * facingY);
                        glVertex2f(position.x - ellipseParameterA * 0.1f * facingX, position.y + ellipseParameterB* 0.8f * facingY);
                    glEnd();
                }
            }
            bool intersect(const Bird& anotherBird) const {
                float d =     (position.x - anotherBird.getPosition().x) * (position.x - anotherBird.getPosition().x) / (ellipseParameterA * ellipseParameterA) +
                            (position.y - anotherBird.getPosition().y) * (position.y - anotherBird.getPosition().y) / (ellipseParameterB * ellipseParameterB);
                if (d > 4) return false;
                return true;
            }
            bool intersect(const Vector& vertex) const {
                float d =     (position.x - vertex.x) * (position.x - vertex.x) / (ellipseParameterA * ellipseParameterA) +
                            (position.y - vertex.y) * (position.y - vertex.y) / (ellipseParameterB * ellipseParameterB);
                if (d > 1) return false;
                 return true;
            }
        };
    enum State{
                STANDBY,
                GRABBED,
                LAUNCHING,
                YEEHAA,
                CRASHED
            };
    State state(Scene::STANDBY);
    Vector mousePosition;
    Bird target(
            Vector(0.75f,0,0),
            Color(0,0.5f,0),
            Bird::LEFT);
    Bird missile(
            Scene::Slingshot::launchPosition,
            Color(0.5f,0,0),
            Bird::RIGHT);
    Vector gravity(0.0f,-14.0f,0.0f);
    float time;
 
    void drawBackground(){
        glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
 
        Vector a(-0.26f,-0.1f,0.0f);
        Vector b(-0.34f,-0.6f,0.0f);
        Vector c(-0.31f,-0.6f,0.0f);
        Vector d(-0.23f,-0.1f,0.0f);
        glColor3f(0.76f,0.043f,0.043f);
        glBegin(GL_TRIANGLES);
                glVertex2f(a.x,a.y);
                glVertex2f(b.x, b.y);
                glVertex2f(d.x, d.y);
                glVertex2f(d.x, d.y);
                glVertex2f(b.x, b.y);
                glVertex2f(c.x, c.y);
        glEnd();
 
        glColor3f(0.1f, 1.0f, 0.1f);
        glBegin(GL_TRIANGLES);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(1.0f, -0.8f);
            glVertex2f(-1.0f, -0.8f);
            glVertex2f(1.0f, -0.8f);
            glVertex2f(-1.0f, -1.0f);
            glVertex2f(1.0f, -1.0f);
        glEnd();
 
        Vector center(Scene::Slingshot::launchPosition);
        if(    Scene::state == Scene::STANDBY ||
            Scene::state == Scene::GRABBED ||
            Scene::state == Scene::LAUNCHING)
            center = Scene::missile.getPosition();
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.27f,-0.333f);
            glVertex2f(center.x - 0.1f, center.y + 0.04);
            glVertex2f(center.x - 0.1f, center.y - 0.04);
            glVertex2f(-0.27f,-0.333f);
            glVertex2f(-0.27f,-0.300f);
            glVertex2f(center.x - 0.1f, center.y + 0.04);
        glEnd();
 
    }
    void drawForeground() {
 
        Vector center(Scene::Slingshot::launchPosition);
        if(    Scene::state == Scene::STANDBY ||
            Scene::state == Scene::GRABBED ||
            Scene::state == Scene::LAUNCHING)
            center = Scene::missile.getPosition();
        glColor3f(0,0,0);
        glBegin(GL_TRIANGLES);
            glVertex2f(-0.333f,-0.333f);
            glVertex2f(center.x - 0.1f, center.y + 0.04);
            glVertex2f(center.x - 0.1f, center.y - 0.04);
            glVertex2f(-0.333f,-0.333f);
            glVertex2f(-0.333f,-0.300f);
            glVertex2f(center.x - 0.1f, center.y + 0.04);
        glEnd();
 
        glColor3f(0.9f,0.05f,0.05f);
        Vector a(-0.340f,-0.6f,0.0f);
        Vector b(-0.340f,-1.0f,0.0f);
        Vector c(-0.310f,-1.0f,0.0f);
        Vector d(-0.310f,-0.6f,0.0f);
        glBegin(GL_TRIANGLES);
                glVertex2f(a.x,a.y);
                glVertex2f(b.x, b.y);
                glVertex2f(d.x, d.y);
                glVertex2f(d.x, d.y);
                glVertex2f(b.x, b.y);
                glVertex2f(c.x, c.y);
        b = a;
        c = d;
        a = Vector(-0.4f,-0.1f,0.0f);
        d = Vector(-0.35f,-0.1f,0.0f);
                glVertex2f(a.x,a.y);
                glVertex2f(b.x, b.y);
                glVertex2f(d.x, d.y);
                glVertex2f(d.x, d.y);
                glVertex2f(b.x, b.y);
                glVertex2f(c.x, c.y);
        glEnd();
    }
}
 
// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization( ) {
    glViewport(0, 0, screenWidth, screenHeight);
}
/////////////////////////////////////////////////////////////////////////////////////////
// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay( ) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Scene::drawBackground();
    Scene::target.draw();
    Scene::missile.draw();
    Scene::drawForeground();
    glutSwapBuffers();
}
 
// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y) {}
 
// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {}
 
// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {
    Scene::mousePosition.x = ((float)x) / (((float)screenWidth) / 2.0f ) - 1.0f;
    Scene::mousePosition.y = -1.0f * (((float)y) / (((float)screenHeight) / 2.0f) - 1.0f);
 
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        if(Scene::state == Scene::STANDBY)
            if(Scene::missile.intersect(Scene::mousePosition))
                Scene::state = Scene::GRABBED;
 
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        if(Scene::state == Scene::GRABBED) {
            Scene::Slingshot::launchVelocity = (Scene::Slingshot::launchPosition - Scene::missile.getPosition()) * 7.0f;
            Scene::state = Scene::LAUNCHING;
        }
    glutPostRedisplay();                          // Ilyenkor rajzold ujra a kepet
}
 
// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y)
{
    Scene::mousePosition.x = ((float)x) / (((float)screenWidth) / 2.0f ) - 1.0f;
    Scene::mousePosition.y = -1.0f * (((float)y) / (((float)screenHeight) / 2.0f) - 1.0f);
 
    if(Scene::state == Scene::GRABBED )
            Scene::missile.setPosition(Scene::mousePosition);
 
    glutPostRedisplay();
}
 
// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {
    float ct = (float)glutGet(GLUT_ELAPSED_TIME)/1000.0f;
    float lt = Scene::time;
    float dt = 0.001;
    for(float it = lt; it < ct;) {
 
        it = fminf(it + dt, ct);
 
        if(Scene::state != Scene::CRASHED)
            Scene::target.setPosition(
                Vector(0.75f, 0.0f, 0.0f) +
                Vector(0.0f, sinf(2.0f * M_PI * it * 0.5f) * 0.7f, 0.0f)
                );
 
        if(Scene::state == Scene::LAUNCHING){
            Scene::missile.setPosition(Scene::missile.getPosition() + Scene::Slingshot::launchVelocity * dt);
            Vector missileFromLaunchPosition(Scene::Slingshot::launchPosition - Scene::missile.getPosition());
            if(missileFromLaunchPosition.Length() < 0.005){
                Scene::Slingshot::launchTime = float(glutGet(GLUT_ELAPSED_TIME)/1000.0f);
                Scene::state = Scene::YEEHAA;
            }
        }
 
        if(Scene::state == Scene::YEEHAA ){
            float mdt = it - Scene::Slingshot::launchTime;
            Scene::missile.setPosition(Vector(
                Scene::Slingshot::launchVelocity.x * mdt + Scene::Slingshot::launchPosition.x,
                Scene::gravity.y * 0.5 * mdt * mdt +
                Scene::Slingshot::launchVelocity.y * mdt +
                Scene::Slingshot::launchPosition.y,
                0.0f
                ));
            if(Scene::missile.intersect(Scene::target) == true)
            {
                Scene::missile.setColor(Color(1.0f,1.0f,0.0f));
                Scene::state = Scene::CRASHED;
                break;
            }
            if(Scene::missile.getPosition().x > 1.1f || Scene::missile.getPosition().x < -1.1f || Scene::missile.getPosition().y < -1.2f ) {
                Scene::state = Scene::STANDBY;
                Scene::missile = Scene::Bird(Scene::Slingshot::launchPosition,Color(0.5f,0,0),Scene::Bird::UPSIDE_DOWN);
                break;
            }
        }
    }
    Scene::time = ct;
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