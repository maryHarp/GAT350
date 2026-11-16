#pragma once
#include "Uniforms.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

namespace nu {
	class Texture;

	class Renderer
	{
	public:
		bool Initialize(const char* name, int width, int height);
		void Shutdown();

		void Clear() const;
		void Present() const;

		bool BeginFrame();
		bool EndFrame();
		void EndRenderPass();

		void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
		void setColor(float r, float g, float b, float a = 1.0f) const;

		void DrawPoint(float x, float y) const;
		void DrawFillRect(float x, float y, float w, float h) const;
		void DrawRect(float x, float y, float w, float h) const;
		void DrawLine(float x1, float y1, float x2, float y2) const;

		int GetWidth() const { return m_width;  }
		int GetHeight() const { return m_height; }


		//SDL GPU

		SDL_GPUDevice* GetGPUDevice() const { return m_gpuDevice; }
		SDL_Window* GetWindow() const { return m_window; }
		SDL_GPUCommandBuffer* GetGPUCommandBuffer() const { return m_commandBuffer; }
		SDL_GPUTexture* GetGPUSwapchainTexture() const { return m_swapchainTexture; }

		void SetPipeline(const class Pipeline& pipeline) const;
		void SetVertexBuffer(const class VertexBuffer& vertexBuffer)const;
		void SetIndexBuffer(const class IndexBuffer& indexBuffer)const;

		void Draw(uint32_t vertexCount) const;
		void DrawIndex(const class IndexBuffer& indexBuffer) const;

		void setVertexUniform(UniformSlot slot, const void* data, uint32_t size) const;
		void setFragmentUniform(UniformSlot slot, const void* data, uint32_t size) const;
		void SetFragmentTexture(uint32_t slot, const class Texture& texture);
		void SetMaterial(const class MaterialInstance& materialInstance) const;

		void CreateDepthBuffer();

		friend class Text;
		friend class Texture;
		friend class Shader;
		friend class VertexBuffer;
		friend class IndexBuffer;



	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;

		SDL_GPUDevice* m_gpuDevice = nullptr;
		SDL_GPUCommandBuffer* m_commandBuffer = nullptr;
		SDL_GPURenderPass* m_renderPass = nullptr;
		SDL_GPUTexture* m_depthTexture = nullptr;
		SDL_GPUTexture* m_swapchainTexture = nullptr;

		bool m_cameraEnabled = true;

		int m_width = 0;
		int m_height = 0;
	};
}




