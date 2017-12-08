#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include <QVector3D>
#include <QMatrix4x4>

float vertices_first_cube[] = {
    // vertex               // normals              // tex
    // front
   -0.5f,  0.5f,   0.5f,      0.0, 0.0, 1.0,      0.0f, 1.0f,// top left                  0
    0.5f,  0.5f,   0.5f,      0.0, 0.0, 1.0,      1.0f, 1.0f,// top right                 1
   -0.5f, -0.5f,   0.5f,      0.0, 0.0, 1.0,      0.0f, 0.0f,// bottom left               2
    0.5f, -0.5f,   0.5f,      0.0, 0.0, 1.0,      1.0f, 0.0f,// bottom right              3

    // rear
    -0.5f,  0.5f, -0.5f,      0.0, 0.0, -1.0,     1.0f, 1.0f, // top left rear           4
     0.5f,  0.5f, -0.5f,      0.0, 0.0, -1.0,     0.0f, 1.0f, // top right rear          5
    -0.5f, -0.5f, -0.5f,      0.0, 0.0, -1.0,     1.0f, 0.0f, // bottom left rear        6
     0.5f, -0.5f, -0.5f,      0.0, 0.0, -1.0,     0.0f, 0.0f, // bottom right rear       7

    // left
    -0.5f, -0.5f,  0.5f,      -1.0, 0.0, 0.0,     1.0f, 0.0f, // bottom left              2/8
    -0.5f, -0.5f, -0.5f,      -1.0, 0.0, 0.0,     0.0f, 0.0f, // bottom left rear         6/9
    -0.5f,  0.5f,  0.5f,      -1.0, 0.0, 0.0,     1.0f, 1.0f, // top left                 0/10
    -0.5f,  0.5f, -0.5f,      -1.0, 0.0, 0.0,     0.0f, 1.0f, // top left rear            4/11

    // right
    0.5f,  0.5f,   0.5f,      1.0, 0.0, 0.0,      0.0f, 1.0f,// top right                 1/12
    0.5f, -0.5f,   0.5f,      1.0, 0.0, 0.0,      0.0f, 0.0f,// bottom right              3/13
    0.5f, -0.5f,  -0.5f,      1.0, 0.0, 0.0,      1.0f, 0.0f,// bottom right rear        7/14
    0.5f,  0.5f,  -0.5f,      1.0, 0.0, 0.0,      1.0f, 1.0f,// top right rear           5/15

    // top
    -0.5f,  0.5f,  0.5f,      0.0, 1.0, 0.0,      0.0f, 1.0f,// top left                  0/16
     0.5f,  0.5f,  0.5f,      0.0, 1.0, 0.0,      1.0f, 1.0f,// top right                 1/17
    -0.5f,  0.5f, -0.5f,      0.0, 1.0, 0.0,      0.0f, 0.0f,// top left rear           4/18
     0.5f,  0.5f, -0.5f,      0.0, 1.0, 0.0,      1.0f, 0.0f,// top right rear          5/19

    //bottom
    -0.5f, -0.5f,  0.5f,      0.0, -1.0, 0.0,     0.0f, 1.0f, // bottom left               2/20
     0.5f, -0.5f,  0.5f,      0.0, -1.0, 0.0,     1.0f, 1.0f, // bottom right              3/21
    -0.5f, -0.5f, -0.5f,      0.0, -1.0, 0.0,     0.0f, 0.0f, // bottom left rear        6/22
     0.5f, -0.5f, -0.5f,      0.0, -1.0, 0.0,     1.0f, 0.0f, // bottom right rear       7/23
};


/*
 *   4____5
 *  /|   /|
 * 0_|__1 |
 * | |  | |
 * | 6__|_7
 * |/___|/
 * 2    3
 *
 */

unsigned int indices_first_cube[] = {
    0, 1, 2, // front
    2, 3, 1,

    4, 5, 6, // rear
    6, 7, 5,

    8, 9, 10, // left
    10, 11, 9,

    12, 13, 14, // right
    14, 15, 12,

    16, 17, 18, // top
    18, 19, 17,

    20, 21, 22, // bottom
    22, 23, 21,

};

QVector3D cubesPositions[] = {
    QVector3D( 0.0f, 0.0f, 0.0f),
    QVector3D( 2.0f, 5.0f, -15.0f),
    QVector3D(-1.5f, -2.2f, -2.5f),
    QVector3D(-3.8f, -2.0f, -12.3f),
    QVector3D( 2.4f, -0.4f, -3.5f),
    QVector3D(-1.7f, 3.0f, -7.5f),
    QVector3D( 1.3f, -2.0f, -2.5f),
    QVector3D( 1.5f, 2.0f, -2.5f),
    QVector3D( 1.5f, 0.2f, -1.5f),
    QVector3D(-1.3f, 1.0f, -1.5f)
};

QMatrix4x4 cubesModelMatrices[sizeof(cubesPositions)/sizeof(cubesPositions[0])];





#endif // VERTEX_DATA_H
