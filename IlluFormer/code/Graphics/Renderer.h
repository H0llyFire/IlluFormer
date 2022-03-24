#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
private:
public:
	static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	static void Clear();
};
