#pragma once
#include <RenderingPlugin.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <functional>

#include "RenderCommand.h"
#include "Entity.h"
#include "BasicShader.h"

namespace rendering
{
	class RenderingEngine : core::RenderingPlugin
	{

	public:
		virtual std::thread startPlugin() override;
		virtual core::HgError setDirtyEntities(std::vector<core::Entity *> &entities) override;
		virtual core::HgError closePlugin() override;
		std::function<void(int, int)> m_keyCallback;
		std::function<void(double, double)> m_mouseCallback;

	protected:
		virtual core::HgError initPlugin() override;
		void renderloop();
		void handleDirtyEnts();

	private:
		GLFWwindow *m_window;
		std::mutex m_RenderingMutex;
		std::vector<core::Entity *> m_dirtyEntities;
		std::unordered_map<uint32_t, RenderCommand> m_renderCommands;
		core::HgError createRenderCommand(core::Entity*);
		core::HgError updateRenderCommand(core::Entity*);

		//TODO: better solution for handling shaders
		BasicShader* m_basicShader;
	};

}