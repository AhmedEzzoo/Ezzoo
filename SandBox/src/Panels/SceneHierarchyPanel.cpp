#include "SceneHierarchyPanel.h"
#include "Scripting/ScriptEngine/ScriptEngine.h"


#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include "Scene/Component.h"

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>

namespace Ezzoo {

	extern const std::filesystem::path s_AssetsDirectory;

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = {};
	}


	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("SceneHeirarchyPanel");
		if (m_Context) 
		{
			m_Context->m_Registry.each([&](auto entityID) {


				Entity ent{ entityID, m_Context.get() };
				DrawEntityNode(ent); });

			if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0))
			{
				m_SelectedEntity = {};
			}

			if (ImGui::BeginPopupContextWindow(0, 1))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}

		}
		
		ImGui::End();



		ImGui::Begin("Properities");

		if (m_SelectedEntity)
			DrawComponents(m_SelectedEntity);

		ImGui::End();

/*

		static bool open = true;
		ImGui::ShowDemoWindow(&open);*/
		


	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().TagName;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		
		if (ImGui::IsItemClicked()) m_SelectedEntity = entity;

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
		}

	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0, float columnWidth = 100.0f )
	{
		ImGuiIO& io = ImGui::GetIO();

		auto boldFont = io.Fonts->Fonts[0];

		//resetValue = (label == "Scale") ? 1.0f : 0.0f;

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHieght = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHieght + 3.0f, lineHieght };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X"))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.3f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y"))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z"))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}


	template <typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;


		if (entity.HasComponent<T>())
		{

			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });

			float lineHieght = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();


			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());

			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegion.x - lineHieght * 0.5f);

			if (ImGui::Button("+", ImVec2{ lineHieght, lineHieght }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}
			
			if (opened)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();

			

		}
	}


	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().TagName;
			char buf[256];
			memset(buf, 0, sizeof(buf));
			strncpy(buf, tag.c_str(), sizeof(buf));
			
			if (ImGui::InputText("##Tag", buf, sizeof(buf)))
			{
				tag = std::string(buf);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("Add Component");

		if (ImGui::BeginPopup("Add Component"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<RigidBodyComponent>("Rigid Body");
			DisplayAddComponentEntry<BoxColliderComponent>("Box Collider");
			DisplayAddComponentEntry<CircleComponent>("Circle");
			DisplayAddComponentEntry<CircleColliderComponent>("Circle Collider");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component) {
						
				DrawVec3Control("Translate", component.Translation);

				glm::vec3 rotation = glm::degrees(component.Rotation);

				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);

				DrawVec3Control("Scale", component.Scale, 1.0f);
					

			});

		DrawComponent<ScriptComponent>("Script", entity, [scene = m_Context, entity](auto& component) {

			    static char buf[64];
			    strcpy(buf, component.ClassName.c_str());
			    if (ImGui::InputText("Class", buf, sizeof(buf)))
			    {
			    	component.ClassName = buf;
			    }
			    
				if (scene->m_IsRunning)
				{
					const Ref<ScriptInstance> scriptInstance = ScriptEngine::GetInstance(entity);

					if (scriptInstance)
					{
						const auto& fieldMap = scriptInstance->GetScriptClass()->GetFields();

						for (const auto& [name, type] : fieldMap)
						{
							if (type.Field == ScriptFields::Float)
							{
								float val = scriptInstance->GetFieldValue<float>(name);
								if (ImGui::DragFloat(name.c_str(), &val, 0.01f, 0.0f, 1.0f))
								{
									scriptInstance->SetFieldValue<float>(name, val);
								}
							}
						}
					}
			    }
				else
				{

					Ref<ScriptClass> scriptClass = ScriptEngine::GetEntityClass(component.ClassName);
					if (scriptClass)
					{
						
						const auto& fields = scriptClass->GetFields();
						auto& fieldMap = ScriptEngine::GetEntityFieldMap(entity);


						for (const auto& [name, field] : fields)
						{
							if (fieldMap.find(name) != fieldMap.end())
							{
								ScriptFieldInstance& scriptField = fieldMap.at(name);

								// Display control to set it maybe
								if (field.Field == ScriptFields::Float)
								{
									float data = scriptField.GetValue<float>();
									if (ImGui::DragFloat(name.c_str(), &data))
										scriptField.SetValue(data);
								}
							}
							else
							{
								if (field.Field == ScriptFields::Float)
								{
									float data = 0.0f;
									if (ImGui::DragFloat(name.c_str(), &data))
									{
										ScriptFieldInstance& fieldInstance = fieldMap[name];
										fieldInstance.Field = field;
										fieldInstance.SetValue(data);
									}
								}
							}
						}
					}
			
				}

			});


		DrawComponent<SpriteRendererComponent>("Sprite", entity, [](auto& component) {
				
			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));

			ImGui::Button("Text", ImVec2(100.0f, 0.0f));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Path"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path item = path;
					component.Texture = Texture2D::CreateTexture2D(item.string());

				}


				ImGui::EndDragDropTarget();
			}

			ImGui::DragFloat("##Tiling", &component.TilingFactor);
				
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
				
			auto& camera = component.Camera;
			ImGui::Checkbox("Primary", &component.PrimaryCamera);

			const char* projectionTypePreview[] = { "Prespective", "Orthographic" };
			const char* currentProjectionTypePreview = projectionTypePreview[(int)camera.GetProjectionType()];


			if (ImGui::BeginCombo("Projection", currentProjectionTypePreview))
			{
				for (int i = 0; i < 2; i++)
				{

					bool isSelected = currentProjectionTypePreview == projectionTypePreview[i];
					if (ImGui::Selectable(projectionTypePreview[i], isSelected))
					{
						currentProjectionTypePreview = projectionTypePreview[i];
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}


			// Prespective Camera
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Prespective)
			{
				float prespectiveFov = glm::degrees(camera.GetPrespectiveFov());
				if (ImGui::DragFloat("FOV", &prespectiveFov))
					camera.SetPrespectiveFov(glm::radians(prespectiveFov));

				float prespectiveNear = camera.GetPrespectiveNear();
				if (ImGui::DragFloat("Near", &prespectiveNear))
					camera.SetPrespectiveNear(prespectiveNear);

				float prespectiveFar = camera.GetPrespectiveFar();
				if (ImGui::DragFloat("Far", &prespectiveFar))
					camera.SetPrespectiveFar(prespectiveFar);
			}


			// Orthographic Camera
			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &orthoSize))
					camera.SetOrthographicSize(orthoSize);

				float orthoNear = camera.GetOrthographicNear();
				if (ImGui::DragFloat("Near", &orthoNear))
					camera.SetOrthographicNear(orthoNear);

				float orthoFar = camera.GetOrthographicFar();
				if (ImGui::DragFloat("Far", &orthoFar))
					camera.SetOrthographicFar(orthoFar);
				ImGui::Checkbox("Fixed Aspect Ration", &component.FixedAspectRation);
			}

		
				
			
			});

		DrawComponent<RigidBodyComponent>("RigidBody 2D", entity, [](auto& component) {


			const char* bodyTypePreview[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentBodyTypePreview = bodyTypePreview[(int)component.Type];


			if (ImGui::BeginCombo("Type", currentBodyTypePreview))
			{
				for (int i = 0; i < 3; i++)
				{

					bool isSelected = currentBodyTypePreview == bodyTypePreview[i];
					if (ImGui::Selectable(bodyTypePreview[i], isSelected))
					{
						currentBodyTypePreview = bodyTypePreview[i];
						component.Type = (RigidBodyComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}


				ImGui::EndCombo();
			}
				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});





		DrawComponent<BoxColliderComponent>("BoxCollider 2D", entity, [](auto& component) {

			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
			ImGui::DragFloat2("Size",  glm::value_ptr(component.Size));
			ImGui::DragFloat("Denisty", &component.Denisty, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Ristitution", &component.Ristitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("RistitutionThreshold", &component.RistitutionThreshold, 0.01f, 0.0f);

			});

		DrawComponent<CircleComponent>("Circle", entity, [](auto& component) {

			ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
			ImGui::DragFloat("Thickness", &component.Thickness, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Fade", &component.Fade, 0.01f, 0.0f, 1.0f);

			});

		DrawComponent<CircleColliderComponent>("CircleCollider 2D", entity, [](auto& component) {

			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f, -0.5f, 1.0f);
			ImGui::DragFloat("Raduis", &component.Raduis);
			ImGui::DragFloat("Denisty", &component.Denisty, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Ristitution", &component.Ristitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("RistitutionThreshold", &component.RistitutionThreshold, 0.01f, 0.0f);

			});
	}

}