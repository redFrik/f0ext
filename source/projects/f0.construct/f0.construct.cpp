/// @file
///	@ingroup 	f0ext
///	@copyright	Copyright 2001-2025 Fredrik Olofsson. All rights reserved.
///	@license	Use of this source code is governed by the GNU GPL License found in the License.md file.

//      ----------------------------------------------------------
//      -- fredrik olofsson 010502                              --
//      -- updated 040225 - for carbon using cw8.0              --
//      -- updated 070103 - for ub using xcode                  --
//      -- updated 130630 - ported to max sdk 6.1.1             --
//      -- updated 250301 - ported to min-devkit (sdk8)         --
//      ----------------------------------------------------------

#include "c74_min.h"

using namespace c74::min;

class f0_construct : public object<f0_construct> {
public:
    MIN_DESCRIPTION	{ "A constructive constructor." };
    MIN_TAGS		{ "f0ext" };
    MIN_AUTHOR		{ "Fredrik Olofsson" };
    MIN_RELATED		{ "lcd" };

    inlet<> m_in1	{ this, "(anything) Bang to draw" };
    outlet<> m_out1	{ this, "(list) [x, y, x+1, y+1" };

    attribute<int, threadsafe::no, limit::clamp> iterations { this, "iterations", 2000, range { 1, 100000 }};
    attribute<int, threadsafe::no, limit::clamp> mapping { this, "mapping", 1, range { 1, 16 }};
    attribute<int, threadsafe::no, limit::clamp> variant { this, "variant", 1, range { 1, 6 }};
    attribute<int, threadsafe::no, limit::clamp> cols { this, "cols", 3, range { 1, 100000 }};
    attribute<int, threadsafe::no, limit::clamp> rows { this, "rows", 2, range { 1, 100000 }};
    attribute<int, threadsafe::no, limit::clamp> width { this, "width", 200, range { 1, 100000 }};
    attribute<int, threadsafe::no, limit::clamp> height { this, "height", 200, range { 1, 100000 }};
    attribute<double> stepX { this, "stepX", 1.0 };
    attribute<double> stepY { this, "stepY", 1.0 };
    attribute<int, threadsafe::no, limit::clamp> movementX { this, "movementX", 95, range { 0, 100 }};
    attribute<int, threadsafe::no, limit::clamp> movementY { this, "movementY", 93, range { 0, 100 }};

    message<> bang { this, "bang",
        MIN_FUNCTION {
            start();
            return {};
        }
    };

	message<> maxclass_setup { this, "maxclass_setup",
        MIN_FUNCTION {
            cout << "f0.construct v3.0; distributed under GNU GPL License" << endl;
            return {};
        }
    };

    message<> number { this, "number",
        MIN_FUNCTION {
            iterations = args[0];
            start();
            return {};
        }
    };

private:
    double m_dx { 1.0 };
    double m_dy { 1.0 };
    double m_x { 10.0 };
    double m_y { 10.0 };

    void start() {
        for (auto i = 0; i < iterations; i++) {
            drawFO();
            m_x += m_dx * stepX;                // flytta position x
            if (m_x < 1) {                      // om vänster kant
                m_dx = myRandom() % 2;          //   vänd eller stå still i x-led
            } else if (m_x > (width / cols)) {  // om höger kant
                m_dx = 0 - myRandom() % 2;      //   vänd eller stå still i x-led
            }
            m_y += m_dy * stepY;                // flytta position y
            if (m_y < 1) {                      // om övre kanten
                m_dy = myRandom() % 2;          //   vänd eller stå still i y-led
            } else if (m_y > (height / rows)) { // om nedre kanten
                m_dy = 0 - myRandom() % 2;      //   vänd eller stå still i y-led
            }
            switch (variant) {
                case 1:
                    variant1();
                    break;
                case 2:
                    variant2();
                    break;
                case 3:
                    variant3();
                    break;
                case 4:
                    variant4();
                    break;
                case 5:
                    variant5();
                    break;
                case 6:
                    variant6();
                    break;
            }
        }
    }
    void variant1() {                                       // 1. slumpa riktningar individuellt
        if ((myRandom() % 100) >= movementX) {
            slumpaRiktningX();
        }
        if ((myRandom() % 100) >= movementY) {
            slumpaRiktningY();
        }
    }
    void variant2() {                                       // 2. slumpa alltid riktningar tillsammans
        slumpaRiktningXY();
    }
    void variant3() {                                       // 3. slumpa riktningar individuellt
        if ((myRandom() % 100) >= movementX) {
            if ((m_dx == 0) && (m_x >= 1)) {                //		dra åt vänster
                m_dx = -1;
            } else {
                m_dx = 0;
            }
        }
        if ((myRandom() % 100) >= movementY) {
            if ((m_dy == 0) && (m_y >= 1)) {                //		dra uppåt
                m_dy = -1;
            } else {
                m_dy = 0;
            }
        }
    }
    void variant4() {                                       // 4. slumpa riktningar individuellt
        if ((myRandom() % 100) >= movementX) {
            if ((m_dx == 0) && (m_x >=1)) {                 //		dra åt vänster
                m_dx = -1;
            } else {
                m_dx = 0;
            }
        }
        if ((myRandom() % 100) >= movementY) {
            if ((m_dy == 0) && (m_y <= (height / rows))) {  //		dra nedåt
                m_dy = 1;
            } else {
                m_dy = 0;
            }
        }
    }
    void variant5() {                                       // 5. slumpa riktningar individuellt
        if ((myRandom() % 100) >= movementX) {
            if ((m_dx == 0) && (m_x <= (width / cols))) {   //		dra åt höger
                m_dx = 1;
            } else {
                m_dx = 0;
            }
        }
        if ((myRandom() % 100) >= movementY) {
            if ((m_dy == 0) && (m_y >= 1)) {                //		dra uppåt
                m_dy = -1;
            } else {
                m_dy = 0;
            }
        }
    }
    void variant6() {                                       // 6. slumpa riktningar individuellt
        if ((myRandom() % 100) >= movementX) {
            if ((m_dx == 0) && (m_x <= (width / cols))) {   //		dra åt höger
                m_dx = 1;
            } else {
                m_dx = 0;
            }
        }
        if ((myRandom() % 100) >= movementY) {
            if ((m_dy == 0) && (m_y <= (height / rows))) {  //		dra nedåt
                m_dy = 1;
            } else {
                m_dy = 0;
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    void slumpaRiktningXY() {
        while ((m_dx == 0) && (m_dy == 0)) {                // kolla att inte båda riktningar blir 0
            m_dx = myRandom() % 3 - 1;
            m_dy = myRandom() % 3 - 1;
        }
    }
    void slumpaRiktningX() {
        m_dx = myRandom() % 3 - 1;
    }
    void slumpaRiktningY() {
        m_dy = myRandom() % 3 - 1;
    }

    //----------------------------------------------------------------------------------------------
    void drawFO() {
        for (auto i = 0; i < cols; i++) {
            for (auto j = 0; j < rows; j++) {
                auto iCol = (width / cols) * i;
                auto jRow = (height / rows) * j;
                switch (mapping) {                      // kolla mapping
                    case 1:
                        mapping1(iCol, jRow);
                        break;
                    case 2:
                        mapping2(iCol, jRow);
                        break;
                    case 3:
                        mapping3(iCol, jRow);
                        break;
                    case 4:
                        mapping4(iCol, jRow);
                        break;
                    case 5:
                        mapping5(i, iCol, jRow);
                        break;
                    case 6:
                        mapping6(i, iCol, jRow);
                        break;
                    case 7:
                        mapping7(i, iCol, jRow);
                        break;
                    case 8:
                        mapping8(j, iCol, jRow);
                        break;
                    case 9:
                        mapping9(j, iCol, jRow);
                        break;
                    case 10:
                        mapping10(j, iCol, jRow);
                        break;
                    case 11:
                        mapping11(i, j, iCol, jRow);
                        break;
                    case 12:
                        mapping12(i, j, iCol, jRow);
                        break;
                    case 13:
                        mapping13(i, j, iCol, jRow);
                        break;
                    case 14:
                        mapping14(i, j, iCol, jRow);
                        break;
                    case 15:
                        mapping15(i, j, iCol, jRow);
                        break;
                    case 16:
                        mapping16(i, j, iCol, jRow);
                        break;
                }
            }
        }
    }

    //----------------------------------------------------------------------------------------------
    void mapping1(int iCol, int jRow) {                   // no flip
        draw(m_x + iCol, m_y + jRow);
    }
    void mapping2(int iCol, int jRow) {                   // flip all x
        draw(mirrorX(m_x) + iCol, m_y + jRow);
    }
    void mapping3(int iCol, int jRow) {                   // flip all y
        draw(m_x + iCol, mirrorY(m_y) + jRow);
    }
    void mapping4(int iCol, int jRow) {                   // flip all xy
        draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
    }
    //--
    void mapping5(int i, int iCol, int jRow) {           // flip odd col x
        if ((i % 2) == 1) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping6(int i, int iCol, int jRow) {           // flip odd col y
        if ((i % 2) == 1) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping7(int i, int iCol, int jRow) {           // flip odd col xy
        if ((i % 2) == 1) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    //--
    void mapping8(int j, int iCol, int jRow) {           // flip odd row x
        if ((j % 2) == 1) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping9(int j, int iCol, int jRow) {           // flip odd row y
        if ((j % 2) == 1) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping10(int j, int iCol, int jRow) {          // flip odd row xy
        if ((j % 2) == 1) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    //--
    void mapping11(int i, int j, int iCol, int jRow) {  // flip odd col & even row x, flip even col & odd row x
        if (((i % 2) == 1) && ((j % 2) == 0)) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else if (((i % 2) == 0) && ((j % 2) == 1)) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping12(int i, int j, int iCol, int jRow) {  // flip odd col & even row y, flip even col & odd row y
        if (((i % 2) == 1) && ((j % 2) == 0)) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else if (((i % 2) == 0) && ((j % 2) == 1)) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping13(int i, int j, int iCol, int jRow) {  // flip odd col & even row xy, flip even col & odd row xy
        if (((i % 2) == 1) && ((j % 2) == 0)) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else if (((i % 2) == 0) && ((j % 2) == 1)) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    //--
    void mapping14(int i, int j, int iCol, int jRow) {  // flip even col & even row x, flip odd col & odd row x
        if (((i % 2) == 0) && ((j % 2) == 0)) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else if (((i % 2) == 1) && ((j % 2) == 1)) {
            draw(mirrorX(m_x) + iCol, m_y + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping15(int i, int j, int iCol, int jRow) {  // flip even col & even row y, flip odd col & odd row y
        if (((i % 2) == 0) && ((j % 2) == 0)) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else if (((i % 2) == 1) && ((j % 2) == 1)) {
            draw(m_x + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    void mapping16(int i, int j, int iCol, int jRow) {  // flip even col & even row xy, flip odd col & odd row xy
        if (((i % 2) == 0) && ((j % 2) == 0)) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else if (((i % 2) == 1) && ((j % 2) == 1)) {
            draw(mirrorX(m_x) + iCol, mirrorY(m_y) + jRow);
        } else {
            draw(m_x + iCol, m_y + jRow);
        }
    }
    int mirrorX(int ox) {
        return round(width / cols - ox);
    }
    int mirrorY(int oy) {
        return round(height / rows - oy);
    }
    int myRandom() {
        return lib::math::random(0.0, 100000.0);
    }
    void draw(int ox, int oy) {
        atoms lista(4);
        lista[0] = ox;
        lista[1] = oy;
        lista[2] = ox + 1;
        lista[3] = oy + 1;
        m_out1.send(lista);
    }

};

MIN_EXTERNAL(f0_construct);
