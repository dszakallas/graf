//=============================================================================================
// Szamitogepes grafika hazi feladat keret. Ervenyes 2013-tol.
// A //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// sorokon beluli reszben celszeru garazdalkodni, mert a tobbit ugyis toroljuk.
// A beadott program csak ebben a fajlban lehet, a fajl 1 byte-os ASCII karaktereket tartalmazhat.
// Tilos:
// - mast "beincludolni", illetve mas konyvtarat hasznalni
// - faljmuveleteket vegezni (printf is fajlmuvelet!)
// - new operatort hivni az onInitialization függvényt kivéve, a lefoglalt adat korrekt felszabadítása nélkül
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
 
#define EPSILON 0.001f
#ifndef FLT_MAX
    #define FLT_MAX 100000
#endif
 
class Color {
public:
    float r;
    float g;
    float b;
public:
    Color() : r(), g(), b() {}
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
    Color& operator=(const Color& color) {
        r = color.r;
        g = color.g;
        b = color.b;
        return *this;
    }
    Color operator+(const Color& color) const {
        return Color(
            r + color.r,
            g + color.g,
            b + color.b
            );
    }
    Color operator*(const Color& color) const {
        return Color(
            r * color.r,
            g * color.g,
            b * color.b
            );
    }
    Color operator*(float f) const{
        return Color(
            r * f,
            g * f,
            b * f
            );
    }
    Color operator/(float d) const {
        return Color(r/d, g/d, b/d);
    }
    bool operator==(const Color& color) const {
        return (r == color.r) && (g == color.g) && (b == color.b);
    }
    bool operator!=(const Color& color) const {
        return (r != color.r) || (g != color.g) || (b != color.b);
    }
    float average() const {
        return (r + g + b)/3.0f;
    }
};
 
class Vector {
public:
    float x;
    float y;
    float z;
    Vector () : x(), y(), z() { }
    Vector(float x, float y, float z) : x(x), y(y), z(z) { }
    Vector(const Vector& other) : x(other.x), y(other.y), z(other.z) {}
    Vector& operator=(const Vector& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    Vector operator+(const Vector& v) const {
        return Vector(x + v.x, y + v.y, z + v.z);
    }
    Vector operator-(const Vector& v) const {
        return Vector(x - v.x, y - v.y, z - v.z);
    }
    Vector operator*(float f) const {
        return Vector(x * f, y * f, z * f);
    }
    Vector operator/(float f) const {
            return Vector(x / f, y / f, z / f);
        }
    float operator*(const Vector& v) const {
        return (x * v.x + y * v.y + z * v.z);
    }
    bool operator==(const Vector& o) const {
        return (x == o.x && y == o.y && z == o.z);
    }
    bool operator!=(const Vector& o) const {
        return !(operator==(o));
    }
    Vector cross(const Vector& v) const {
        return Vector(y*v.z-z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }
    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }
    void normalize() {
        float l = length();
        if(l == 0) return;
        x = x/l;
        y = y/l;
        z = z/l;
    }
};
 
const Vector nullvector;
 
class Matrix {
public:
    float m[4][4];
    Matrix() : m() {
        for(int i = 0; i < 16; i++)
                m[i / 4][i % 4] = 0;
    }
    explicit Matrix(const float * val) {
        for(int i = 0; i < 16; i++)
            m[i / 4][i % 4] = val[i];
    }
    void clear() {
        for(int i = 0; i < 16; i++)
            m[i / 4][i % 4] = 0;
    }
    void setIdentity() {
        clear();
        m[3][3] = m[2][2] = m[1][1] = m[0][0] = 1;
    }
    Matrix& operator=(const float * val) {
        for(int i = 0; i < 16; i++)
            m[i / 4][i % 4] = val[i];
        return *this;
    }
    Matrix operator+(const Matrix& mat) const {
        Matrix result;
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                result.m[i][j] = m[i][j] + mat.m[i][j];
        return result;
    }
    Matrix operator*(const Matrix& mat) const {
        Matrix result;
        result.clear();
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                for(int k = 0; k<4; ++k)
                    result.m[i][j] += m[i][k] * mat.m[k][j];
        return result;
    }
};
 
Vector operator*(const Vector& v, const Matrix& m) {
    float h = m.m[0][3] * v.x + m.m[1][3] * v.y + m.m[2][3] * v.z + m.m[3][3];
    Vector result(
        (m.m[0][0] * v.x + m.m[1][0] * v.y + m.m[2][0] * v.z + m.m[3][0])/h,
        (m.m[0][1] * v.x + m.m[1][1] * v.y + m.m[2][1] * v.z + m.m[3][1])/h,
        (m.m[0][2] * v.x + m.m[1][2] * v.y + m.m[2][2] * v.z + m.m[3][2])/h
        );
    return result;
}
 
float fmin3(float a, float b, float c) {
    return fmin(fmin(a,b),c);
}
float fmin4(float a, float b, float c, float d) {
    return fmin(fmin3(a,b,c),d);
}
 
 
class RayTracing {
public:
 
    class Ray {
    public:
        Vector source;
        Vector direction;
        Ray(const Vector& source, const Vector& direction) : source(source), direction(direction) {}
    };
 
    class Camera {
        const Vector position;
        const Vector side;
        const Vector look;
        const Vector up;
        const float angle;
        const float h;
        const float v;
        const int resolution_x;
        const int resolution_y;
    public:
        Camera(const Vector& position, const Vector& look, const Vector& up, float angle, int resolution_x, int resolution_y)
            : position(position), side(up.cross(look)), look(look), up(up), angle(angle),
              h(tanf(angle) / (resolution_x * 0.5)), v(tanf(angle) / (resolution_y * 0.5)),
              resolution_x(resolution_x), resolution_y(resolution_y)
        {    }
        const Vector& getPosition() const { return position; }
        const Vector& getLook() const { return look; }
        const Vector& getUp() const { return up; }
        const Vector& getSide() const { return side; }
        const float getH() const { return h; }
        const float getV() const { return v; }
        Ray lookAt(int screen_x, int screen_y) const {
            const float pix_x = -h * resolution_x / 2.0f + screen_x * h;
            const float pix_y = -v * resolution_y / 2.0f + screen_y * v;
 
            Vector direction(    side * pix_x +
                                up * pix_y +
                                look);
            direction.normalize();
            return Ray(position, direction);
        }
    };
 
    class Object;
 
    class Material {
    public:
        Color ambient;
        Color diffuse;
        Color specular;
        float shininess;
        Color n;
        Color k;
        Material(
            Color ambient,
            Color diffuse,
            Color specular,
            float shininess,
            Color n,
            Color k) :
            ambient(ambient),
            diffuse(diffuse),
            specular(specular),
            shininess(shininess),
            n(n),
            k(k) {}
 
        Color getBrdf(const Vector& in, const Vector& out, const Vector& normal) const {
            float cosIn = (in * normal) * -1;
            if(cosIn <= EPSILON) return Color(0,0,0);
            Color result(diffuse);
            // TODO: spekul�risat visszarakni
            /*
            Vector reflDir(normal * (2.0f * cosIn) + in);
            float cosReflOut = reflDir * out;
            if(cosReflOut > EPSILON && specular != Color(0,0,0))
                result = result + specular * powf(cosReflOut, shininess) / cosIn;
            */
            return result;
        }
        Color fresnel(float cosOfAngleOfIncidence) const {
            if(fabs(cosOfAngleOfIncidence) < EPSILON)
                return Color();
            const float coeff = powf((1 - cosOfAngleOfIncidence),5) * 4.0f;
            return Color(
            ((n.r - 1.0f) * (n.r - 1.0f) + (k.r) * (k.r) + coeff * n.r) / (((n.r + 1.0f) * (n.r + 1.0f)) + (k.r) * (k.r)),
            ((n.g - 1.0f) * (n.g - 1.0f) + (k.g) * (k.g) + coeff * n.g) / (((n.g + 1.0f) * (n.g + 1.0f)) + (k.g) * (k.g)),
            ((n.b - 1.0f) * (n.b - 1.0f) + (k.b) * (k.b) + coeff * n.b) / (((n.b + 1.0f) * (n.b + 1.0f)) + (k.b) * (k.b))
            );
        }
 
    };
 
 
    class Hit{
    public:
        Vector intersection;
        Vector normal;
        const Object* target;
        float t;
    };
 
    class RollPitchTranslate {
        Matrix transform;
        Matrix inv_transform;
    public:
        RollPitchTranslate(float roll, float pitch, const Vector& translation) {
            const float tr[] = {
                    cosf(roll) * cosf(pitch),    sinf(roll),        -sinf(pitch) * cosf(roll),     0,
                    -cosf(pitch) * sinf(roll),    cosf(roll),        sinf(pitch) * sinf(roll),     0,
                    sinf(pitch),                0,                cosf(pitch),                0,
                    translation.x,                translation.y,    translation.z,                1
            };
            transform = tr;
            const float itr_x = -(translation.x * cosf(roll) * cosf(pitch)) - (sinf(roll) * translation.y) + (sinf(pitch) * cosf(roll) * translation.z);
            const float itr_y = (translation.x * sinf(roll) * cosf(pitch)) -(translation.y * cosf(roll)) - sinf(roll) * sinf(pitch) * translation.z;
            const float itr_z = -(translation.x * sinf(pitch)) - (translation.z * cosf(pitch));
 
            const float inv[] = {
                    cosf(roll) * cosf(pitch),    -cosf(pitch) * sinf(roll),    sinf(pitch),    0,
                    sinf(roll),                    cosf(roll),                    0,                0,
                    -sinf(pitch) * cosf(roll),    sinf(pitch) * sinf(roll),    cosf(pitch),    0,
                    itr_x,                        itr_y,                        itr_z,            1
            };
            inv_transform = inv;
        }
        const Matrix& get() const { return transform; }
        const Matrix& getInverse() const { return inv_transform; }
    };
 
    class Object{
        public:
            virtual const Material& getMaterial() const = 0;
            virtual bool intersect(const Ray& ray, Hit& hit) const = 0;
            virtual ~Object() {}
    };
 
    class Cylinder : public Object {
        RollPitchTranslate rpt;
        float radius;
        float height;
        bool empty;
        Material material;
    public:
        Cylinder(const Vector& origin, float roll, float pitch, float radius, float height, bool empty, const Material& material)
            : rpt(roll, pitch, origin), radius(radius), height(height), empty(empty), material(material)
        { }
        virtual bool intersect(const Ray& ray, Hit& hit) const  {
 
            Ray trRay(    ray.source * rpt.getInverse(),
                        ray.direction * rpt.getInverse() - nullvector * rpt.getInverse());
 
            Vector plane_front_i;
            Vector plane_front_n(1,0,0);
            float plane_front_t = FLT_MAX;
 
            Vector plane_back_i;
            Vector plane_back_n(-1,0,0);
            float plane_back_t = FLT_MAX;
 
            Vector cyl_near_i;
            Vector cyl_near_n;
            float cyl_near_t = FLT_MAX;
 
            Vector cyl_far_i;
            Vector cyl_far_n;
            float cyl_far_t = FLT_MAX;
 
            if(empty == false) {
                if(trRay.direction.x != 0) {
                    float _t = ((height / 2.0f) - trRay.source.x ) / trRay.direction.x;
                    if(_t > EPSILON) {
                        Vector _i = trRay.source + trRay.direction * _t;
                        if((_i + Vector(- height/2.0f, 0, 0)).length() < radius) {
                            plane_front_t = _t;
                            plane_front_i = _i;
                        }
                    }
                    _t = ((- height / 2.0f) - trRay.source.x ) / trRay.direction.x;
                    if(_t > EPSILON) {
                        Vector _i = trRay.source + trRay.direction * _t;
                        if((_i + Vector(height/2.0f, 0, 0)).length() < radius) {
                            plane_back_t = _t;
                            plane_back_i = _i;
                        }
                    }
                }
            }
 
            const float a = trRay.direction.y * trRay.direction.y + trRay.direction.z * trRay.direction.z;
            const float b = 2.0f * ( trRay.direction.z * trRay.source.z + trRay.direction.y * trRay.source.y);
            const float c = trRay.source.z * trRay.source.z + trRay.source.y * trRay.source.y - radius * radius;
 
            const float discriminant = b * b - 4.0f * a * c;
            if(discriminant < 0 )
                return false;
 
            const float sqrtOfDiscriminant = sqrtf(discriminant);
            float _t = (-b + sqrtOfDiscriminant) / (2.0f * a);
            if(_t > EPSILON) {
                cyl_far_i = trRay.source + trRay.direction * _t;
                if(fabs(cyl_far_i.x) < (height / 2.0f)){
                    cyl_far_t = _t;
                    cyl_far_n = cyl_far_i;
                    cyl_far_n.x = 0;
                    cyl_far_n.normalize();
                }
            }
            _t = (-b - sqrtOfDiscriminant) / (2.0f * a);
            if(_t > EPSILON) {
                cyl_near_i = trRay.source + trRay.direction * _t;
                if(fabs(cyl_near_i.x) < (height / 2.0f)){
                    cyl_near_t = _t;
                    cyl_near_n = cyl_near_i;
                    cyl_near_n.x = 0;
                    cyl_near_n.normalize();
                }
            }
            float min = fmin4(cyl_near_t,cyl_far_t,plane_back_t,plane_front_t);
            if(min >= FLT_MAX)
                return false;
 
            Vector intersection;
            Vector normal;
            float t;
 
            if(plane_front_t == min) { intersection = plane_front_i; normal = plane_front_n; t = plane_front_t; }
            else {
                if(plane_back_t == min) { intersection = plane_back_i; normal = plane_back_n; t = plane_back_t; }
                else {
                    if(cyl_near_t == min) { intersection = cyl_near_i; normal = cyl_near_n; t = cyl_near_t; }
                    else {
                        intersection = cyl_far_i; normal = cyl_far_n * -1; t = cyl_far_t;
                    }
                }
            }
 
            hit.intersection = ray.source + ray.direction * t;
            hit.normal = normal * rpt.get() - nullvector * rpt.get();
            hit.normal.normalize();
            hit.t = t;
            hit.target = this;
            return true;
        }
        virtual const Material& getMaterial() const {
            return material;
        }
        virtual ~Cylinder() {}
    };
 
    class Plane : public Object {
        RollPitchTranslate rpt;
        Vector origin;
        Material material;
    public:
        Plane(const Vector& origin, float roll, float pitch, const Material& material)
            : rpt(roll, pitch, origin), origin(origin), material(material)
        { }
        virtual const Material& getMaterial() const { return material; }
        virtual bool intersect(const Ray& ray, Hit& hit) const {
            Ray trRay(    ray.source * rpt.getInverse(),
                        ray.direction * rpt.getInverse() - nullvector * rpt.getInverse());
 
            if(trRay.direction.x == 0) return false;
            const float t = - trRay.source.x / trRay.direction.x;
            if(t < EPSILON) return false;
 
            hit.t = t;
            Vector normal(1,0,0);
            Vector intersection(trRay.source + trRay.direction * t);
            hit.intersection = intersection * rpt.get();
            hit.normal = normal * rpt.get() - nullvector * rpt.get();
            hit.target = this;
            return true;
        }
    };
 
    /*
     * Swirling round with this familiar parable.
     * Spinning, weaving round each new experience.
     * Recognize this as a holy gift and celebrate this
     * chance to be alive and breathing,
     * chance to be alive and breathing.
     * http://www.toolband.com/album/lyrics/lateralus.html
     */
    class Paraboloid : public Object {
        RollPitchTranslate rpt;
        float cut;
        Material material;
    public:
        Paraboloid(const Vector& origin, float roll, float pitch, float x_cut, const Material& material) :
            rpt(roll, pitch, origin), cut(x_cut), material(material) { }
        virtual const Material& getMaterial() const { return material; }
        virtual bool intersect(const Ray& ray, Hit& hit) const {
            Ray trRay(ray.source * rpt.getInverse(), ray.direction * rpt.getInverse() - nullvector * rpt.getInverse());
 
            Vector para_near_i;
            Vector para_near_n;
            float para_near_t = FLT_MAX;
 
            Vector para_far_i;
            Vector para_far_n;
            float para_far_t = FLT_MAX;
 
            const float a = trRay.direction.z * trRay.direction.z + trRay.direction.y * trRay.direction.y;
            const float b = - trRay.direction.x + 2.0f * (trRay.direction.y * trRay.source.y + trRay.direction.z * trRay.source.z);
            const float c = -trRay.source.x + trRay.source.y * trRay.source.y + trRay.source.z * trRay.source.z;
 
            const float discriminant = b * b - 4.0f * a * c;
            if(discriminant < 0 )
                return false;
 
            const float sqrtOfDiscriminant = sqrtf(discriminant);
            float _t = (-b + sqrtOfDiscriminant) / (2.0f * a);
            if(_t > EPSILON) {
                para_far_i = trRay.source + trRay.direction * _t;
                if(para_far_i.x < cut && para_far_i.x > 0){
                    para_far_t = _t;
                    para_far_n = para_far_i;
                    para_far_n.x = 0;
                    para_far_n.normalize();
                }
            }
            _t = (-b - sqrtOfDiscriminant) / (2.0f * a);
            if(_t > EPSILON) {
                para_near_i = trRay.source + trRay.direction * _t;
                if(para_near_i.x < cut && para_near_i.x > 0){
                    para_near_t = _t;
                    para_near_n = para_near_i;
                    para_near_n.x = 0;
                    para_near_n.normalize();
                }
            }
            float min = fmin(para_near_t,para_far_t);
            if(min >= FLT_MAX)
                return false;
 
            Vector intersection;
            Vector normal;
            float t;
 
            if(para_near_t == min) { intersection = para_near_i; normal = para_near_n; t = para_near_t; }
            else {
                intersection = para_far_i; normal = para_far_n * -1; t = para_far_t;
            }
 
            hit.intersection = ray.source + ray.direction * t;
            hit.normal = normal * rpt.get() - nullvector * rpt.get();
            hit.normal.normalize();
            hit.t = t;
            hit.target = this;
            return true;
        }
    };
 
    class Light{
        Vector origin;
        Color emission;
    public:
        Light(const Vector& origin, const Color& emission) : origin(origin), emission(emission) {}
        virtual const Vector& getOrigin() const {
            return origin;
        }
        virtual const Color& getEmission() const {
            return emission;
        }
        virtual ~Light() {}
 
    };
    struct PhotonMap{
        unsigned int s;
        Color c[250];
        Vector v[250];
        PhotonMap() : s(), c(), v() { }
        Color collect(const Vector& w) {
            Color result;
            for(unsigned int i = 0; i < s; ++i){
                float dist = (v[i] - w).length();
                if(dist < 10.0f)
                    result = result + c[i] * expf(-0.7*dist);
            }
            return result;
        }
        bool full() { return s == 250; }
        void add(const Color& c, const Vector& v) {
            if(s < 250) {
                this->c[s] = c;
                this->v[s] = v;
                ++s;
            }
        }
    };
 
private:
    static const int depth_max = 14;
    const Color ambient_light;
    const Camera camera;
public:
    PhotonMap pmap;
    unsigned int lights_s;
    const Light* lights_v[10];
    unsigned int objects_s;
    const Object* objects_v[10];
 
 
public:
 
    RayTracing(const Camera& camera) :
        ambient_light(Color(1,1,1)),
        camera(camera),
        pmap(),
        lights_s(0),
        lights_v(),
        objects_s(0),
        objects_v()
        {
        }
 
    bool intersect(const Ray& ray, Hit& hit){
        hit.target = 0;
        float min = FLT_MAX;
        Hit localHit;
        for(unsigned int i = 0; i < objects_s; ++i) {
            if(objects_v[i]->intersect(ray, localHit) == false)
                continue;
            if(localHit.t < min) {
                min = localHit.t;
                hit = localHit;
            }
        }
        return (hit.target != 0);
    }
 
    Color getDiffuse(const Ray& ray, Hit& hit){
        Color sum(0,0,0);
        for(unsigned int i = 0; i < lights_s; i++){
            Ray rayToLight(hit.intersection,lights_v[i]->getOrigin() - hit.intersection);
            float distanceFromLight = rayToLight.direction.length();
            rayToLight.direction.normalize();
            float cosTheta = rayToLight.direction * hit.normal;
            if(cosTheta <= 0) continue;
            Hit hitToLight;
            if (intersect(rayToLight, hitToLight) == true)
                if((hitToLight.intersection - hit.intersection).length() < distanceFromLight)
                    continue;
            Color brdf(
                hit.target->getMaterial().getBrdf(ray.direction,rayToLight.direction,hit.normal)
                );
            sum = sum + brdf * lights_v[i]->getEmission() * cosTheta;
        }
        if(hit.target->getMaterial().n == Color(0,0,0)){
            sum = sum + pmap.collect(hit.intersection);
        }
        return sum;
    }
 
    Color trace(const Ray& ray, int depth = 0){
 
        if(depth > depth_max)
            return ambient_light;
 
        Hit hit;
        if(intersect(ray, hit) == false)
            return ambient_light;
 
        Color ambient(hit.target->getMaterial().ambient * ambient_light);
 
        Color diff_spec(getDiffuse(ray,hit));
 
        Color ideal(0,0,0);
        if(hit.target->getMaterial().n != Color(0,0,0)) {
            Vector reflectionDirection(hit.normal * ((ray.direction * hit.normal) * -2.0f) + ray.direction);
            float cosTh = (ray.direction * -1.0f) * hit.normal;
            ideal = hit.target->getMaterial().fresnel(cosTh) * trace(Ray(hit.intersection, reflectionDirection), depth + 1);
        }
        return ambient + diff_spec + ideal;
    }
 
    void addLight(const Light * light) {
        if(lights_s < 10) {
            lights_v[lights_s] = light;
            ++lights_s;
        }
    }
 
    void addObject(const Object * object) {
        if(objects_s < 10) {
            objects_v[objects_s] = object;
            ++objects_s;
        }
    }
 
    void collect(const Color& power, const Ray& ray, int depth = 0) {
        if(depth > depth_max || pmap.full())
            return;
        Hit hit;
        if (intersect(ray, hit)) {
            if (hit.target->getMaterial().n != Color(0,0,0))
            {
                Vector reflectionDirection(hit.normal * ((ray.direction * hit.normal) * -2.0f) + ray.direction);
                float cosTh = (ray.direction * -1.0f) * hit.normal;
                collect(hit.target->getMaterial().fresnel(cosTh) * power, Ray(hit.intersection, reflectionDirection), depth + 1);
            }
            else
            {
                if( depth > 0)
                    pmap.add(power, hit.intersection);
            }
        }
 
    }
 
    void shoot() {
        srand(glutGet(GLUT_ELAPSED_TIME));
        for(unsigned int i = 0; i < lights_s; ++i)
        {
            for(unsigned int j = 0; j < 50; ++j) {
 
                Vector r(     2.0*(float)rand()/RAND_MAX-1.0,
                            2.0*(float)rand()/RAND_MAX-1.0,
                            2.0*(float)rand()/RAND_MAX-1.0);
                r.normalize();
                collect(lights_v[i]->getEmission(), Ray(lights_v[i]->getOrigin(),r));
            }
        }
    }
 
 
 
    const Camera& getCamera() const { return camera; }
};
 
const int screenWidth = 600;
const int screenHeight = 600;
Color image[screenWidth*screenHeight];
 
void onInitialization( ) {
    glViewport(0, 0, screenWidth, screenHeight);
    ///////////ANYAGTULAJDONSAGOK KEZDODNEK
    /*
     * Az anyagtulajdonsagokat innen szedtem: http://devernay.free.fr/cours/opengl/materials.html
     */
    RayTracing::Material gold(
            Color(0.247,0.192,0.192), //A
            Color(0.752,0.606,0.226), //D
            Color(0.628,0.556,0.366), //S
            0.4,
            Color(0.17,0.35,1.5),
            Color(3.1,2.7,1.9)
            );
    RayTracing::Material copper(
            Color(0.191,0.074,0.023), //A
            Color(0.7038,0.27048,0.0828), //D
            Color(0.2568,0.137622,0.086014), //S
            0.1,
            Color(0.2,1.1,1.2),
            Color(3.6,2.6,2.3)
            );
    RayTracing::Material silver(
            Color(0.19225,0.19225,0.19225), //A
            Color(0.50754,0.50754,0.50754), //D
            Color(0.508273,0.508273,0.508273), //S
            0.4,
            Color(0.14,0.16,0.13),
            Color(4.1,2.3,3.1));
    RayTracing::Material diffuseRed(
            Color(0.4,0.2,0.2), //A
            Color(1.8,0.6,0.6), //D
            Color(1.8,0.6,0.6), //S
            1.4,
            Color(),
            Color()
    );
    RayTracing::Material diffuseGreen(
            Color(0.2,0.4,0.2), //A
            Color(0.6,1.8,0.6), //D
            Color(0.6,1.8,0.6), //S
            1.4,
            Color(),
            Color()
    );
    //////////ANYAGTULAJDONSAGOK VEGE
 
 
    /////////OBJEKTUMOK KEZDODNEK
    RayTracing::Cylinder goldenCyl(
        Vector(0,2.5,0),
        M_PI_2,
        0,
        15,
        10,
        true,
        gold
    );
    RayTracing::Cylinder diffuse(
        Vector(-3,1,2),
        M_PI_4,
        M_PI_4,
        1,
        5,
        false,
        diffuseRed
    );
    RayTracing::Cylinder copperCyl(
        Vector(0,-2,0),
        0,
        M_PI_2 / 3.0f,
        0.5,
        10,
        false,
        copper
    );
    RayTracing::Cylinder silverCyl(
        Vector(3,1,2),
        -M_PI_4,
        -M_PI_4,
        1,
        5,
        false,
        silver
    );
 
    RayTracing::Plane pl0(
            Vector(0,-2.5,0),
            M_PI_2,
            0,
            diffuseGreen
            );
    RayTracing::Paraboloid par0(
                    Vector(0,1,-0.5),
                    -M_PI_2,
                    0,
                    2,
                    silver
                    );
    RayTracing::Light light0(
                Vector(0.0,0.0,0.0),
                Color(0.3,0.3,0.3)
                );
    RayTracing::Light light1(
                    Vector(4,0,-7),
                    Color(0.4,0.4,0.4)
                    );
    RayTracing::Light light2(
                        Vector(-4,0,-7),
                        Color(0.4,0.4,0.4)
                        );
 
    //// OBJEKTUMOK VEGE
 
    RayTracing rayTracing(
            RayTracing::Camera(    Vector(0,0,-14),
                                Vector(0,0,1),
                                Vector(0,1,0),
                                M_PI_4/2.0f,
                                screenWidth,
                                screenHeight)
            );
    rayTracing.addObject(&pl0);
    rayTracing.addObject(&goldenCyl);
    rayTracing.addObject(&silverCyl);
    rayTracing.addObject(&copperCyl);
    rayTracing.addObject(&diffuse);
    rayTracing.addObject(&par0);
    rayTracing.addLight(&light0);
    rayTracing.addLight(&light1);
    rayTracing.addLight(&light2);
 
    rayTracing.shoot();
 
    for(int Y = 0; Y < screenHeight; Y++)
        for(int X = 0; X < screenWidth; X++)
            image[Y*screenWidth + X] = Color(rayTracing.trace(rayTracing.getCamera().lookAt(X,Y)));
 
    float max = 1.0f;
    for(int Y = 0; Y < screenHeight; Y++)
        for(int X = 0; X < screenWidth; X++)
            if(image[Y*screenWidth + X].average() > max) max = image[Y*screenWidth + X].average();
    for(int Y = 0; Y < screenHeight; Y++)
        for(int X = 0; X < screenWidth; X++)
            image[Y*screenWidth + X] = image[Y*screenWidth + X] / max;
}
 
void onDisplay( ) {
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawPixels(screenWidth, screenHeight, GL_RGB, GL_FLOAT, image);
    glutSwapBuffers();
}
void onKeyboard(unsigned char key, int x, int y) { }
void onKeyboardUp(unsigned char key, int x, int y) { }
void onMouse(int button, int state, int x, int y) { }
void onMouseMotion(int x, int y) { }
void onIdle( ) { }
 
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