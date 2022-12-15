#pragma once


#include "renderer.h"

int LoadTexture(const char* fileName);
void UninitTexture(void);

ID3D11ShaderResourceView** GetTexture(int index);
