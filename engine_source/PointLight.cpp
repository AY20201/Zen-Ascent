#include"../engine_headers/Light.h"
#include"../engine_headers/LightHandler.h"

class PointLight : public Light
{
	public:

		glm::vec3 position;

		PointLight(glm::vec3 position, glm::vec3 color, float intensity)
		{
			PointLight::position = position;
			Light::color = color;
			Light::intensity = intensity;
			Light::type = Light::LightType::Point;

			index = static_cast<int>(LightHandler::Instance.pointLights.size());
			LightHandler::Instance.AddLight(this);
		}

		void SetUniforms(Shader& shader) override
		{
			shader.Activate();
			glUniform3f(glGetUniformLocation(shader.ID, ("pointLights[" + std::to_string(index) + "].position").c_str()), position.x, position.y, position.z);
			glUniform3f(glGetUniformLocation(shader.ID, ("pointLights[" + std::to_string(index) + "].color").c_str()), color.x, color.y, color.z);
			glUniform1f(glGetUniformLocation(shader.ID, ("pointLights[" + std::to_string(index) + "].intensity").c_str()), intensity);
		}
};