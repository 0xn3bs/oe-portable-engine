/*****************************************************************************************
 * Copyright (c) 2008 Jonathan 'Bladezor' Bastnagel.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser Public License v2.1
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * 
 * Contributors:
 *     Jonathan 'Bladezor' Bastnagel - initial implementation and documentation
 *****************************************************************************************/
#include "Color.h"

void Odorless::Engine::Tools::Colors::Color::IntToRGB(const unsigned int &RGB,
                unsigned int *colorComponents)
{
        colorComponents[0] = (RGB >> 16) & 255; //      Red
        colorComponents[1] = (RGB >> 8) & 255; //       Green
        colorComponents[2] = (RGB) & 255; //    Blue
}

unsigned int *Odorless::Engine::Tools::Colors::Color::IntToRGB(const unsigned int &RGB)
{
        unsigned int *colorComponents = new unsigned int [3];
        colorComponents[0] = (RGB >> 16) & 255; //      Red
        colorComponents[1] = (RGB >> 8) & 255; //       Green
        colorComponents[2] = RGB & 255; //      Alpha
        return colorComponents;
}

void Odorless::Engine::Tools::Colors::Color::RGBToInt(int &RGB, const unsigned int &R,
                const unsigned int &G, const unsigned int &B)
{
        RGB = RGBToInt(R, G, B);
}

unsigned int Odorless::Engine::Tools::Colors::Color::RGBToInt(const unsigned int &R, const unsigned int &G, const unsigned int &B)
{
        return ((R << 16) + (G << 8) + (B));
}

void Odorless::Engine::Tools::Colors::Color::IntToRGBA(const unsigned int &RGBA,
                unsigned int *colorComponents)
{
        colorComponents[0] = (RGBA >> 24) & 255; //     Red
        colorComponents[1] = (RGBA >> 16) & 255; //     Green
        colorComponents[2] = (RGBA >> 8) & 255; //      Blue
        colorComponents[3] = RGBA & 255; //     Alpha
}

unsigned int *Odorless::Engine::Tools::Colors::Color::IntToRGBA(const unsigned int &RGBA)
{
        unsigned int *colorComponents = new unsigned int [4];
        colorComponents[0] = (RGBA >> 24) & 255; //     Red
        colorComponents[1] = (RGBA >> 16) & 255; //     Green
        colorComponents[2] = (RGBA >> 8) & 255; //      Blue
        colorComponents[3] = RGBA & 255; //     Alpha
        return colorComponents;
}

void Odorless::Engine::Tools::Colors::Color::RGBAToInt(unsigned int &RGBA, const unsigned int &R,
                const unsigned int &G, const unsigned int &B, const unsigned int &A)
{
        RGBA = RGBAToInt(R, G, B, A);
}

unsigned int Odorless::Engine::Tools::Colors::Color::RGBAToInt(const unsigned int &R, const unsigned int &G, const unsigned int &B,
                const unsigned int &A)
{
        return ((R << 24) + (G << 16) + (B << 8) + (A));
}
