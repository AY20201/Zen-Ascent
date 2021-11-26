#include"../engine_headers/Light.h"

class DirectionalLight : public Light
{
	public:

		glm::vec3 direction;
		DirectionalLight(glm::vec3 direction, glm::vec3 color)
		{
			DirectionalLight::direction = direction;
			Light::color = color;
			Light::type = Light::LightType::Directional;
		}

		void SetUniforms(Shader& shader) override
		{
			shader.Activate();
			glUniform3f(glGetUniformLocation(shader.ID, "lightDir"), direction.x, direction.y, direction.z);
			glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), color.x, color.y, color.z);
		}
};