#include "ada/tools/geom.h"

#include <algorithm>
#include <iostream>

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif
#ifndef TAU
#define TAU 6.2831853071795864769252867665590
#endif

namespace ada {

void getBoundingBox(const std::vector<glm::vec3> &_pts, glm::vec3 &_min, glm::vec3 &_max) {
    _min = glm::vec3(10000.0, 10000.0, 10000.0);
    _max = glm::vec3(-10000.0, -10000.0, -10000.0);

    return expandBoundingBox(_pts, _min, _max);
}

void expandBoundingBox(const std::vector<glm::vec3> &_pts, glm::vec3 &_min, glm::vec3 &_max) {
    for (unsigned int i = 0; i < _pts.size(); i++) {
        expandBoundingBox(_pts[i], _min, _max);
    }
}

void expandBoundingBox(const glm::vec3 &_pt, glm::vec3 &_min, glm::vec3 &_max) {
    if ( _pt.x < _min.x)
        _min.x = _pt.x;

    if ( _pt.y < _min.y)
        _min.y = _pt.y;

    if ( _pt.z < _min.z)
        _min.z = _pt.z;

    if ( _pt.x > _max.x)
        _max.x = _pt.x;

    if ( _pt.y > _max.y)
        _max.y = _pt.y;

    if ( _pt.z > _max.z)
        _max.z = _pt.z;
}

glm::vec3 getCentroid(const std::vector<glm::vec3> &_pts) {
    glm::vec3 centroid;
    for (uint32_t i = 0; i < _pts.size(); i++) {
        centroid += _pts[i] / (float)_pts.size();
    }
    return centroid;
}

void calcNormal(const glm::vec3& _v0, const glm::vec3& _v1, const glm::vec3& _v2, glm::vec3& _N) {
    glm::vec3 v10 = _v1 - _v0;
    glm::vec3 v20 = _v2 - _v0;

    _N.x = v20.x * v10.z - v20.z * v10.y;
    _N.y = v20.z * v10.x - v20.x * v10.z;
    _N.z = v20.x * v10.y - v20.y * v10.x;
    
    _N = glm::normalize(_N);
}

Mesh line (const glm::vec3 &_a, const glm::vec3 &_b) {
    glm::vec3 linePoints[2];
    linePoints[0] = glm::vec3(_a.x,_a.y,_a.z);
    linePoints[1] = glm::vec3(_b.x,_b.y,_b.z);;

    Mesh mesh;
    mesh.addVertices(linePoints,2);
    mesh.setDrawMode(GL_LINES);
    return mesh;
};

Mesh lineTo(const glm::vec3 &_a, const glm::vec3 &_dir, float _size) {
    return line(_a, _a + normalize(_dir) * _size );
}

Mesh cross (const glm::vec3 &_pos, float _width) {
    glm::vec3 linePoints[4] = { glm::vec3(_pos.x,_pos.y,_pos.z),
                                glm::vec3(_pos.x,_pos.y,_pos.z),
                                glm::vec3(_pos.x,_pos.y,_pos.z),
                                glm::vec3(_pos.x,_pos.y,_pos.z) };

    linePoints[0].x -= _width;
    linePoints[1].x += _width;
    linePoints[2].y -= _width;
    linePoints[3].y += _width;

    Mesh mesh;
    mesh.setDrawMode(GL_LINES);
    mesh.addVertices(linePoints, 4);

    // mesh.add( line(linePoints[0] , linePoints[1]) );
    // mesh.add( line(linePoints[2] , linePoints[3]) );

    return mesh;
}


// Billboard
//============================================================================
Mesh rect (float _x, float _y, float _w, float _h) {
    float x = _x-1.0f;
    float y = _y-1.0f;
    float w = _w*2.0f;
    float h = _h*2.0f;

    Mesh mesh;
    mesh.addVertex(glm::vec3(x, y, 0.0));
    mesh.addColor(glm::vec4(1.0));
    mesh.addNormal(glm::vec3(0.0, 0.0, 1.0));
    mesh.addTexCoord(glm::vec2(0.0, 0.0));

    mesh.addVertex(glm::vec3(x+w, y, 0.0));
    mesh.addColor(glm::vec4(1.0));
    mesh.addNormal(glm::vec3(0.0, 0.0, 1.0));
    mesh.addTexCoord(glm::vec2(1.0, 0.0));

    mesh.addVertex(glm::vec3(x+w, y+h, 0.0));
    mesh.addColor(glm::vec4(1.0));
    mesh.addNormal(glm::vec3(0.0, 0.0, 1.0));
    mesh.addTexCoord(glm::vec2(1.0, 1.0));

    mesh.addVertex(glm::vec3(x, y+h, 0.0));
    mesh.addColor(glm::vec4(1.0));
    mesh.addNormal(glm::vec3(0.0, 0.0, 1.0));
    mesh.addTexCoord(glm::vec2(0.0, 1.0));

    mesh.addIndex(0);   mesh.addIndex(1);   mesh.addIndex(2);
    mesh.addIndex(2);   mesh.addIndex(3);   mesh.addIndex(0);

    return mesh;
}

Mesh cube(float _size) {
    float vertices[] = {
        -_size,  _size,  _size,
        -_size, -_size,  _size,
        _size, -_size,  _size,
        _size,  _size,  _size,
        -_size,  _size, -_size,
        -_size, -_size, -_size,
        _size, -_size, -_size,
        _size,  _size, -_size,
    };

    INDEX_TYPE indices[] = {
        0, 1, 2,
        0, 2, 3,
        3, 2, 6,
        3, 6, 7,
        0, 4, 7,
        0, 7, 3,
        4, 6, 7,
        4, 6, 5,
        0, 5, 4,
        0, 5, 1,
        1, 6, 5,
        1, 6, 2,
    };

    Mesh mesh;
    mesh.addVertices(reinterpret_cast<glm::vec3*>(vertices), 8);
    mesh.addIndices(indices, 36);
    return mesh;
}

Mesh cubeCorners(const glm::vec3 &_min_v, const glm::vec3 &_max_v, float _size) {
    float size = glm::min(glm::length(_min_v), glm::length(_max_v)) * _size *  0.5;

    //    D ---- A
    // C ---- B  |
    // |  |   |  |
    // |  I --|- F
    // H .... G

    glm::vec3 A = _max_v;
    glm::vec3 H = _min_v;

    glm::vec3 B = glm::vec3(A.x, A.y, H.z);
    glm::vec3 C = glm::vec3(H.x, A.y, H.z);
    glm::vec3 D = glm::vec3(H.x, A.y, A.z);

    glm::vec3 F = glm::vec3(A.x, H.y, A.z);
    glm::vec3 G = glm::vec3(A.x, H.y, H.z);
    glm::vec3 I = glm::vec3(H.x, H.y, A.z);

    Mesh mesh;
    mesh.setDrawMode(GL_LINES);
    mesh.add( lineTo(A, normalize(D-A), size) );
    mesh.add( lineTo(A, normalize(B-A), size) );
    mesh.add( lineTo(A, normalize(F-A), size) );

    mesh.add( lineTo(B, normalize(A-B), size) );
    mesh.add( lineTo(B, normalize(C-B), size) );
    mesh.add( lineTo(B, normalize(G-B), size) );

    mesh.add( lineTo(C, normalize(D-C), size) );
    mesh.add( lineTo(C, normalize(B-C), size) );
    mesh.add( lineTo(C, normalize(H-C), size) );
    
    mesh.add( lineTo(D, normalize(A-D), size) );
    mesh.add( lineTo(D, normalize(C-D), size) );
    mesh.add( lineTo(D, normalize(I-D), size) );

    mesh.add( lineTo(F, normalize(G-F), size) );
    mesh.add( lineTo(F, normalize(A-F), size) );
    mesh.add( lineTo(F, normalize(I-F), size) );

    mesh.add( lineTo(G, normalize(H-G), size) );
    mesh.add( lineTo(G, normalize(F-G), size) );
    mesh.add( lineTo(G, normalize(B-G), size) );

    mesh.add( lineTo(H, normalize(I-H), size) );
    mesh.add( lineTo(H, normalize(G-H), size) );
    mesh.add( lineTo(H, normalize(C-H), size) );

    mesh.add( lineTo(I, normalize(F-I), size) );
    mesh.add( lineTo(I, normalize(H-I), size) );
    mesh.add( lineTo(I, normalize(D-I), size) );

    return mesh;
}

Mesh cubeCorners(const std::vector<glm::vec3> &_pts, float _size) {
    glm::vec3 min_v;
    glm::vec3 max_v;
    getBoundingBox( _pts, min_v, max_v);
    return cubeCorners(min_v, max_v, _size);
}

Mesh axis(float _size, float _y) {
    Mesh mesh;
    mesh.setDrawMode(GL_LINES);

    mesh.add( line(glm::vec3(_size,_y,0.0), glm::vec3(-_size,_y,0.0)));
    mesh.add( line(glm::vec3(0.0, _size, 0.0), glm::vec3(0.0, -_size, 0.0)));
    mesh.add( line(glm::vec3(0.0, _y, _size), glm::vec3(0.0, _y, -_size)));

    return mesh;
}

Mesh grid (float _width, float _height, int _columns, int _rows, float _y) {
    Mesh mesh;
    mesh.setDrawMode(GL_LINES);

    // the origin of the plane is at the center //
    float halfW = _width  * 0.5f;
    float halfH = _height * 0.5f;

    //  . --- A
    //  |     |
    //  B --- .

    glm::vec3 A = glm::vec3(halfW, _y, halfH);
    glm::vec3 B = glm::vec3(-halfW, _y, -halfH);

    // add the vertexes //
    for(int iy = 0; iy != _rows; iy++) {
        float pct = ((float)iy/((float)_rows-1));

        glm::vec3 left = glm::mix(A, B, glm::vec3(0.0, _y, pct));
        glm::vec3 right = glm::mix(A, B, glm::vec3(1.0, _y, pct));

        mesh.add( line(left, right) );
    }

    for(int ix = 0; ix != _columns; ix++) {
        float pct = ((float)ix/((float)_columns-1));

        glm::vec3 top = glm::mix(A, B, glm::vec3(pct, _y, 0.0));
        glm::vec3 down = glm::mix(A, B, glm::vec3(pct, _y, 1.0));

        mesh.add( line(top, down) );
    }

    return mesh;
}

Mesh grid(float _size, int _segments, float _y) {
    return grid(_size, _size, _segments, _segments, _y);
}

Mesh floor(float _area, int _subD, float _y) {

    int N = pow(2,_subD);

    Mesh mesh;
    float w = _area/float(N);
    float h = _area/2.0;
    for (int z = 0; z <= N; z++){
        for (int x = 0; x <= N; x++){
            mesh.addVertex(glm::vec3(x * w - h, _y, z * w - h));
            mesh.addColor(glm::vec4(0.251, 0.251, 0.251, 1.0));
            mesh.addNormal(glm::vec3(0.0, 1.0, 0.0));
            mesh.addTexCoord(glm::vec2(float(x)/float(N), float(z)/float(N)));
        }
    }
    
    //
    // 0 -- 1 -- 2      A -- B
    // |    |    |      |    | 
    // 3 -- 4 -- 5      C -- D
    // |    |    |
    // 6 -- 7 -- 8
    //
    for (int y = 0; y < N; y++){
        for (int x=0; x < N; x++){
            mesh.addIndex(  x   +   y   * (N+1));   // A
            mesh.addIndex((x+1) +   y   * (N+1));   // B
            mesh.addIndex((x+1) + (y+1) * (N+1));   // D

            mesh.addIndex((x+1) + (y+1) * (N+1));   // D
            mesh.addIndex(  x   + (y+1) * (N+1));   // C
            mesh.addIndex(  x   +   y   * (N+1));  // A
        }
    }

    return mesh;
}

Mesh sphere(int _resolution, float _radius ) {
    Mesh mesh;

    float doubleRes = _resolution*2.f;
    float polarInc = PI/(_resolution); // ringAngle
    float azimInc = TAU/(doubleRes); // segAngle //
    
    glm::vec3 vert;
    glm::vec2 tcoord;

    for (float i = 0; i < _resolution + 1; i++) {

        float tr = sin( PI-i * polarInc );
        float ny = cos( PI-i * polarInc );

        tcoord.y = 1.f - (i / _resolution);

        for (float j = 0; j <= doubleRes; j++) {

            float nx = tr * sin(j * azimInc);
            float nz = tr * cos(j * azimInc);

            tcoord.x = j / (doubleRes);

            vert = {nx, ny, nz};
            mesh.addNormal(vert);

            vert *= _radius;
            mesh.addVertex(vert);
            mesh.addTexCoord(tcoord);
        }
    }

    int nr = doubleRes+1;
    

    int index1, index2, index3;
    for (float iy = 0; iy < _resolution; iy++) {
        for (float ix = 0; ix < doubleRes; ix++) {

            // first tri //
            if (iy > 0) {
                index1 = (iy+0) * (nr) + (ix+0);
                index2 = (iy+0) * (nr) + (ix+1);
                index3 = (iy+1) * (nr) + (ix+0);

                mesh.addIndex(index1);
                mesh.addIndex(index2);
                mesh.addIndex(index3);
            }

            if (iy < _resolution - 1 ) {
                // second tri //
                index1 = (iy+0) * (nr) + (ix+1);
                index2 = (iy+1) * (nr) + (ix+1);
                index3 = (iy+1) * (nr) + (ix+0);

                mesh.addIndex(index1);
                mesh.addIndex(index2);
                mesh.addIndex(index3);

            }
        }
    }

    return mesh;
}

}
