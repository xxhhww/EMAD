#include "DirectLight.h"

void DirectLight::genCtrlGui() noexcept
{
	if (ImGui::Begin("DirectLight")) {
		ImGui::Text("Direction");
		ImGui::SliderFloat("Direct-X", &mDirection.x, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Direct-Y", &mDirection.y, -10.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("Direct-Z", &mDirection.z, -10.0f, 10.0f, "%.1f");

		ImGui::Text("Light");
		ImGui::ColorEdit3("Diffuse", &mColor.x);
		ImGui::SliderFloat("Ambient", &mAmbient, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("Specular", &mSpecular, 0.0f, 1.0f, "%.1f");
	}
	ImGui::End();
}
