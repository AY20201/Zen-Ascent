#include"../engine_headers/Light.h"

class PointLight : public Light
{
	public:

		glm::vec3 position;
		PointLight(glm::vec3 position, glm::vec3 color)
		{
			PointLight::position = position;
			Light::color = color;
			Light::type = Light::LightType::Point;
		}

		void SetUniforms(Shader& shader) override
		{
			shader.Activate();
			glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), position.x, position.y, position.z);
			glUniform3f(glGetUniformLocation(shader.ID, "lightColor"), color.x, color.y, color.z);
		}
};