#pragma once
#include <RenderingPlugin.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>

#include "RenderCommand.h"
#include "TextRenderCommand.h"
#include "Entity.h"
#include "BasicShader.h"
#include "TextShader.h"
#include "HgTexture.h"
#include "Camera.h"
#include "ColourShader.h"

namespace rendering
{
	class RenderingEngine : core::RenderingPlugin
	{

	public:
		//override functions
		virtual std::thread startPlugin() override;
		virtual core::HgError setDirtyEntities(std::vector<core::Entity *> &entities) override;
        virtual core::HgError destroyEntities(std::vector<uint32_t>& entIds) override;
		virtual core::HgError addTexture(core::HgTexture* texture) override;
		virtual core::HgError closePlugin() override;
		virtual core::HgError setCameraPos(glm::vec3 translation) override;
		virtual glm::vec3 getCameraPos() override;
		
		std::function<void(int, int)> m_keyCallback;
		std::function<void(double, double)> m_mouseCallback;
		std::function<void(int, int)> m_mouseButtonCallback;
		bool isInitialized(){std::lock_guard<std::mutex>lock(m_RenderingMutex); return m_isInitialized;}
		
		Camera* getCamera(){return m_camera;}
		RenderingEngine(std::shared_ptr<core::Config> config);
		
		virtual std::shared_ptr<core::Config> getConfig() override{
			return m_config;
		}

	protected:
		virtual core::HgError initPlugin() override;
		void renderloop();
		void handleDirtyEnts();
		void handleDirtyTextures();
        void handleDestroyedEnts();
	private:
		GLFWwindow *m_window;
		std::mutex m_RenderingMutex;
		std::unordered_set<core::Entity *> m_dirtyEntities;
		std::unordered_set<uint32_t> m_entsToDestroy;
		std::unordered_map<uint32_t, RenderCommand> m_renderCommands;
		std::unordered_map<uint32_t, TextRenderCommand> m_TextRenderCommands;
		std::unordered_map<const char*, core::HgTexture*> m_textures;
		std::unordered_set<core::HgTexture*> m_dirtyTextures;
		core::HgError createRenderCommand(core::Entity*);
		core::HgError updateRenderCommand(core::Entity*);
		bool m_isInitialized = false;
		//TODO: better solution for handling shaders
		BasicShader* m_basicShader;
		TextShader* m_textShader;
        ColourShader* m_colourShader;
		Camera* m_camera;
		std::shared_ptr<core::Config> m_config;
	};
}